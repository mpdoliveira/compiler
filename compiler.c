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

int print_segment(int start, int end, char *str)
{
    for (int i = start; i < end; i++)
    {
        printf("%c\n", str[i]);
    }
}

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
    return NULL;
}

void advance_char(Lexer *lexer)
{
    if (peek_char(lexer) != NULL)
    {
        lexer->position++;
    }
}

int issymb(char c)
{
    if (c == '=' || c == '!' || c == '>' || c == '<' || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '{' || c == '}' || c == ':' || c == ';' || c == ',')
    {
        return 1;
    }
    return 0;
}

// Lexeme scanning functions
TokenType scan_identifier(Lexer *lexer)
{
    while (true)
    {
        char c = peek_char(lexer);
        if (isalnum(c) || c == '_')
        {
            advance_char(lexer);
        }
        else
        {
            return 1;
        }
    }
}

TokenType scan_number(Lexer *lexer)
{
    TokenType num_type = INT_CONST;
    while (true)
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
            if (!isgit(peek_char(lexer)))
            {
                return TOKEN_ERROR;
            }
            num_type = FLOAT_CONST;
            advance_char(lexer);
        }
        else
        {
            return num_type;
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
int scan_lexeme(Lexer *lexer)
{
    char c = current_char(lexer);
    if (isalpha(c))
    {
        int start = lexer->position;
        scan_identifier(lexer);
        print_segment(start, lexer->position, lexer->source);
    }
    else if (isdigit(c))
    {
        int start = lexer->position;
        scan_number(lexer);
        print_segment(start, lexer->position, lexer->source);
    }
    else if (issymb(c))
    {
        scan_symbol(lexer);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Proper usage: compiler.c path.txt");
        return 1;
    }

    FILE *fptr;

    fptr = fopen(argv[1], "r");
    if (fptr == NULL)
    {
        printf("Failed to find %s", argv[1]);
        return 1;
    }

    char source_code[100];
    fgets(source_code, 100, fptr);

    Lexer *lexer = malloc(sizeof(Lexer));
}