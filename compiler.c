#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum TokenType
{
    TOKEN_ERROR,
    FUNCTION,
    MAIN,
    LET,
    INT,
    FLOAT,
    CHAR,
    IF,
    ELSE,
    WHILE,
    PRINTLN,
    RETURN,
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,
    ARROW,
    COLON,
    SEMICOLON,
    COMMA,
    ASSIGN,
    EQ,
    NE,
    GT,
    GE,
    LT,
    LE,
    PLUS,
    MINUS,
    MULT,
    DIV,
    ID,
    INT_CONST,
    FLOAT_CONST,
    CHAR_LITERAL,
    FMT_STRING
} TokenType;

typedef struct Token
{
    char *lexeme;
    TokenType type;
    int line;
} Token;

typedef struct Lexer
{
    char *source;
    int length;
    int position;
    int line;
} Lexer;

// Helper char dealing functions
char current_char(Lexer *lexer)
{
    return lexer->source[lexer->position];
}

char peek_char(Lexer *lexer)
{
    if (lexer->position + 1 < lexer->length)
    {
        return lexer->source[lexer->position + 1];
    }
    return '\0';
}

void advance_char(Lexer *lexer)
{
    lexer->position++;
}

int issymb(char c)
{
    if (c == '=' || c == '!' || c == '>' || c == '<' || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '{' || c == '}' || c == ':' || c == ';' || c == ',')
    {
        return 1;
    }
    return 0;
}

int issep(char c)
{
    if (issymb(c) || isspace(c) || c == '\0')
    {
        return 1;
    }
    return 0;
}

// Lexeme scanning functions
TokenType scan_identifier(Lexer *lexer)
{
    while (1)
    {
        char c = peek_char(lexer);

        if (isalnum(c) || c == '_')
        {
            advance_char(lexer);
        }
        else if (issep(c))
        {
            return ID;
        }
        else
        {
            return TOKEN_ERROR;
        }
    }
}

TokenType scan_number(Lexer *lexer)
{
    TokenType num_type = INT_CONST;
    while (1)
    {
        char c = peek_char(lexer);
        if (isdigit(c))
        {
            advance_char(lexer);
        }
        else if (c == '.')
        {
            if (num_type == FLOAT_CONST)
            {
                return TOKEN_ERROR;
            }

            advance_char(lexer);
            if (!isdigit(peek_char(lexer)))
            {
                return TOKEN_ERROR;
            }
            num_type = FLOAT_CONST;
            advance_char(lexer);
        }
        else if (issep(c))
        {
            return num_type;
        }
        else
        {
            return TOKEN_ERROR;
        }
    }
}

TokenType scan_symbol(Lexer *lexer)
{
    char c = current_char(lexer);
    char next_c = peek_char(lexer);

    switch (c)
    {
    case '=':
        if (next_c == '=')
        {
            advance_char(lexer);
            return EQ;
        }
        return ASSIGN;

    case '!':
        if (next_c == '=')
        {
            advance_char(lexer);
            return NE;
        }
        return TOKEN_ERROR;

    case '>':
        if (next_c == '=')
        {
            advance_char(lexer);
            return GE;
        }
        return GT;

    case '<':
        if (next_c == '=')
        {
            advance_char(lexer);
            return LE;
        }
        return LT;

    case '+':
        return PLUS;

    case '-':
        if (next_c == '>')
        {
            advance_char(lexer);
            return ARROW;
        }
        return MINUS;

    case '*':
        return MULT;

    case '/':
        return DIV;

    case '{':
        return LBRACE;

    case '}':
        return RBRACE;

    case '(':
        return LBRACKET;

    case ')':
        return RBRACKET;

    case ':':
        return COLON;

    case ';':
        return SEMICOLON;

    case ',':
        return COMMA;

    default:
        return TOKEN_ERROR;
    }
}
TokenType scan_lexeme(Lexer *lexer)
{
    char c = current_char(lexer);
    if (isalpha(c))
        return scan_identifier(lexer);
    if (isdigit(c))
        return scan_number(lexer);
    if (issymb(c))
        return scan_symbol(lexer);

    return TOKEN_ERROR;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Proper usage: compiler path.txt\n");
        return 1;
    }

    FILE *fptr = fopen(argv[1], "r");

    if (fptr == NULL)
    {
        printf("Failed to find %s\n", argv[1]);
        return 1;
    }

    char source_code[100];
    fgets(source_code, sizeof(source_code), fptr);

    fclose(fptr);

    Lexer lexer = {
        .source = source_code,
        .length = strlen(source_code),
        .position = 0,
        .line = 1};

    while (lexer.position < lexer.length)
    {
        if (isspace(current_char(&lexer)))
        {
            advance_char(&lexer);
        }
        advance_char(&lexer);
    }
    return 0;
}