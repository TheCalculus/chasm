#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "../include/templator_lexer.h"
#include "../include/templator_parser.h"
#include "../include/rainbow_output.h"

char* token_reps[END_OF_FILE + 1] = {
    "CHASM_KWD_CAST", "CHASM_KWD_LOOP",  "CHASM_KWD_IF", "LITERAL",
    "VARIABLE",       "LEFT_BRACE",      "RIGHT_BRACE",  "HTML_OPEN",
    "HTML_CLOSE",     "HTML_CLOSE_CAST", "EQUAL_SIGN", "END_OF_FILE",
};

int prepare_input() {
    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
    lexer->size  = INITIAL_SIZE;
    lexer->token = (Token*)malloc(sizeof(Token) * lexer->size);

    Parser* parser = (Parser*)malloc(sizeof(Parser));
    parser->size   = INITIAL_SIZE;
    parser->nodes  = (Node*)malloc(sizeof(Node) * parser->size);

    if ((lexer->buffer = fopen("chasm.ch", "r")) == NULL) {
        perror(RED "error opening file");
        return 1;
    }

    printf(GRN "file opened\n" RESET);

    lexer->active = fgetc(lexer->buffer);
    ungetc(lexer->active, lexer->buffer);

    tokenize(lexer, parser); // templator_lexer.c
    // iterateTokens(lexer);    // templator_lexer.c
    freeResources(lexer);    // templator_lexer.c

    return 0;
}

int main() {
    printf(GRN "chasm.c copies\n" RESET);
    return prepare_input();
}
