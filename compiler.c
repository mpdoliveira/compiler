#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum TokenType
{
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

typedef struct Lexer
{
    char *source;
    int length;
    int position;
    int line;
} Lexer;

typedef struct Token
{
    char *lexeme;
    TokenType type;
    int line;
} Token;

typedef enum TypeGroup
{
    NONE,
    IDENTIFIER,
    INTEGER,
    FLOAT_NUMBER,
    CHAR_LITERAL_GROUP,
    FMT_STRING_GROUP,
    SYMBOL
} TypeGroup;

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