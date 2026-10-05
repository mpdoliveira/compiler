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

typedef struct Token
{
    char *lexeme;
    TokenType type;
    int line;
} Token;

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

    int len = strlen(source_code);
    char lexeme[10];
    int lexeme_i = 0;
    for (int i = 0; i < len; i++)
    {
        if (isalnum(source_code[i]))
        {
            lexeme[lexeme_i] = source_code[i];
            lexeme_i++;
        }
        else
        {
            lexeme[lexeme_i] = '\0';
            printf("%s ", lexeme);
            lexeme_i = 0;
        }
    }

    return 0;
}