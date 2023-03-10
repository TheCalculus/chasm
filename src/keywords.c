#include <stdlib.h>
#include <stdio.h>

#define true  1
#define false 0
typedef int   bool;

struct Token;
typedef struct Token Token;

typedef enum {
    C_COMMENT,
    C_VARIABLE,
    C_BLOCK,
    HTML_ATTRIBUTE,
    HTML_CLOSE_TAG,
    HTML_COMMENT,
    HTML_OPEN_TAG,
    JS_COMMENT,
    JS_VARIABLE,
    JS_BLOCK,
    TEMPLATE_CLOSE_EXPRESSION,
    TEMPLATE_OPEN_EXPRESSION,
} TokenType;

struct Token {
    char*     value;
    TokenType type;
    Token*    parentnode;
    Token*    childnodes;
    int       childcount;
};

Token* PARSETREE;
int    PARSER_SIZE  = 10;
int    PARSER_INDEX = 0;

void PARSETREE_ADD(Token* token) {
    if (PARSER_INDEX++ != PARSER_SIZE) {
        PARSETREE[PARSER_INDEX] = token;
        return;
    }
    PARSER_INDEX--;
    PARSER_SIZE += 10;
    PARSETREE = realloc((void*)&PARSETREE_COPY, sizeof(Token) * PARSER_SIZE);
}

int main() {
    PARSETREE = (Token*)malloc(sizeof(Token) * PARSER_SIZE);
    char* request = 
        "{{{}{}}}{}{}{}}";
}
