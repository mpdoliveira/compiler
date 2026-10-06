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

typedef enum TypeGroup {
    TG_NONE,
    TG_ALPHA,
    TG_DIGIT,
    TG_UNDERSCORE,
    TG_DOT,
    TG_SINGLE_QUOTE,
    TG_DOUBLE_QUOTE,
    TG_EQUAL,
    TG_EXCLAMATION,
    TG_GREATER,
    TG_LESS,
    TG_MINUS,
    TG_SIMPLE_SYMBOL,
    TG_WHITESPACE,
    TG_OTHER
} TypeGroup;

int print_segment(int start, int end, char* str) {
    for (int i = start; i < end; i++) {
        printf("%c\n", str[i]);
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

    int lexeme_start = -1;

    TypeGroup type_group = NONE;

    int len = strlen(source_code);
    for (int i = 0; i < len; i++)
    {
        if (isalpha(source_code[i])) {
            if (type_group == NONE) {
                lexeme_start = i;
                type_group = ALPHA;
            }
            else if (type_group == DIGIT) {
                print_segment(lexeme_start, i-1, source_code);
                type_group = ALPHA;
                lexeme_start = i;
            }
            else if (type_group == SYMBOL) {
                print_segment(lexeme_start, i-1, source_code);
                type_group = ALPHA;
                lexeme_start = i;
            }
        }
        else if (isdigit(source_code[i])) {
            if (type_group == NONE) {
                lexeme_start = i;
                type_group = DIGIT;
            }
            else if (type_group == SYMBOL) {
                print_segment(lexeme_start, i-1, source_code);
                type_group = DIGIT;
                lexeme_start = i;
            }
        }
        else if (source_code[i] == '_') {
            if (type_group != ALPHA) {
                print_segment(lexeme_start, i-1, source_code);
                type_group = ALPHA;
                lexeme_start = i;
            }
        }
    }

    return 0;
}