#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "../include/templator_lexer.h"
#include "../include/templator_parser.h"
#include "../include/rainbow_output.h"

#include "../include/server.h"

char* token_reps[END_OF_FILE + 1] = {
    "CHASM_KWD_CAST", "CHASM_KWD_LOOP",  "CHASM_KWD_IF", "LITERAL",
    "VARIABLE",       "LEFT_BRACE",      "RIGHT_BRACE",  "HTML_OPEN",
    "HTML_CLOSE",     "HTML_CLOSE_CAST", "EQUAL_SIGN",   "END_OF_FILE",
};

int prepare_input() {
    Lexer*  lexer  = malloc(sizeof(Lexer));
    Parser* parser = malloc(sizeof(Parser));

    *lexer  = (Lexer)  {.size = LEXER_INITIAL_SIZE,  .token = malloc(sizeof(Token) * lexer->size)};
    *parser = (Parser) {.size = PARSER_INITIAL_SIZE, .nodes = malloc(sizeof(Node)  * parser->size)};

    if ((lexer->buffer = fopen("chasm.ch", "r")) == NULL) {
        perror(RED "error opening file");
        return 1;
    }

    fseek(lexer->buffer, 0, SEEK_END);
    lexer->bufsiz = ftell(lexer->buffer);
    fseek(lexer->buffer, 0, SEEK_SET);

    printf(GRN "file opened\n" RESET);

    fflush(stdout); // flush stdout for correct colouring in case of errors following this point
    lexer->active = fgetc(lexer->buffer);
    ungetc(lexer->active, lexer->buffer);

    size_t out;
    
    generateNodeTree(lexer, parser); // templator_parlex.c
    parseTreeToHTML(parser, &out, lexer->bufsiz);

    // iterateTokens(lexer);    // templator_parlex.c
    freeLexer(lexer);        // templator_parlex.c
    freeParser(parser);      // templator_parlex.c

    return 0;
}

int main() {
    printf(GRN "chasm.c copies\n" RESET);
    // webserver();
    return prepare_input();
}
