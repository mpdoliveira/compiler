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

typedef struct ReservedWord
{
    char *literal;
    TokenType type;
} ReservedWord;

ReservedWord RESERVED_WORDS[11] =
{
    {"fn", FUNCTION},
    {"if", IF},
    {"let", LET},
    {"int", INT},
    {"main", MAIN},
    {"char", CHAR},
    {"else", ELSE},
    {"float", FLOAT},
    {"while", WHILE},
    {"return", RETURN},
    {"println", PRINTLN}
};

typedef struct Lexer
{
    char *source;
    int length;
    int position;
    int line;
} Lexer;

// Helper char dealing functions
char curr_char(Lexer *lexer)
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

// Helper char cathegorization functions
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

// Helper printing functions
const char *token_type_name(TokenType type)
{
    switch (type)
    {
    case TOKEN_ERROR:
        return "TOKEN_ERROR";

    case FUNCTION:
        return "FUNCTION";

    case MAIN:
        return "MAIN";

    case LET:
        return "LET";

    case INT:
        return "INT";

    case FLOAT:
        return "FLOAT";

    case CHAR:
        return "CHAR";

    case IF:
        return "IF";

    case ELSE:
        return "ELSE";

    case WHILE:
        return "WHILE";

    case PRINTLN:
        return "PRINTLN";

    case RETURN:
        return "RETURN";

    case LBRACKET:
        return "LBRACKET";

    case RBRACKET:
        return "RBRACKET";

    case LBRACE:
        return "LBRACE";

    case RBRACE:
        return "RBRACE";

    case ARROW:
        return "ARROW";

    case COLON:
        return "COLON";

    case SEMICOLON:
        return "SEMICOLON";

    case COMMA:
        return "COMMA";

    case ASSIGN:
        return "ASSIGN";

    case EQ:
        return "EQ";

    case NE:
        return "NE";

    case GT:
        return "GT";

    case GE:
        return "GE";

    case LT:
        return "LT";

    case LE:
        return "LE";

    case PLUS:
        return "PLUS";

    case MINUS:
        return "MINUS";

    case MULT:
        return "MULT";

    case DIV:
        return "DIV";

    case ID:
        return "ID";

    case INT_CONST:
        return "INT_CONST";

    case FLOAT_CONST:
        return "FLOAT_CONST";

    case CHAR_LITERAL:
        return "CHAR_LITERAL";

    case FMT_STRING:
        return "FMT_STRING";

    default:
        return "UNKNOWN_TOKEN";
    }
}

// Lexeme scanning functions
TokenType scan_reserved_word(Lexer* lexer, int start)
{
    int length = lexer->position - start + 1;

    // Early return 
    if (length < 2 || length > 7) {
        return ID;
    }

    switch (length)
    {
    case 2:
        if (strncmp(&lexer->source[start], "fn", 2) == 0)
            return FUNCTION;
        if (strncmp(&lexer->source[start], "if", 2) == 0)
            return IF;
        break;
    case 3:
        if (strncmp(&lexer->source[start], "let", 3) == 0)
            return LET;
        if (strncmp(&lexer->source[start], "int", 3) == 0)
            return INT;
        break;
    case 4:
        if (strncmp(&lexer->source[start], "main", 4) == 0)
            return MAIN;
        if (strncmp(&lexer->source[start], "char", 4) == 0)
            return CHAR;
        if (strncmp(&lexer->source[start], "else", 4) == 0)
            return ELSE;
        break;
    case 5:
        if (strncmp(&lexer->source[start], "float", 5) == 0)
            return FLOAT;
        if (strncmp(&lexer->source[start], "while", 5) == 0)
            return WHILE;
        break;
    case 6:
        if (strncmp(&lexer->source[start], "return", 6) == 0)
            return RETURN;
        break;
    case 7:
        if (strncmp(&lexer->source[start], "println", 7) == 0)
            return PRINTLN;
        break;
    }
    return ID;
}

TokenType scan_identifier(Lexer *lexer)
{
    int start = lexer->position;
    int possible_reserved = 1;

    while (1)
    {
        char c = peek_char(lexer);

        if (isdigit(c) || c == '_')
        {
            possible_reserved = 0;
            advance_char(lexer);
        }
        else if (isalpha(c))
        {
            advance_char(lexer);
        }
        else if (issep(c))
        {
            if (possible_reserved)
            {
                return scan_reserved_word(lexer, start);
            }
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
    char c = curr_char(lexer);
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
    char c = curr_char(lexer);

    if (isalpha(c))
    {
        return scan_identifier(lexer);
    }
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

    TokenType types[20];
    int i = 0;
    while (lexer.position < lexer.length)
    {
        if (isspace(curr_char(&lexer)))
        {
            advance_char(&lexer);
        }
        else
        {
            types[i] = scan_lexeme(&lexer);
            printf("<%s>\n", token_type_name(types[i]));
            i++;
            advance_char(&lexer);
        }
    }

    return 0;
}