#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "templator_lexer.h" // tokenize(), free_resources(), Lexer
#include "string_reps.h"     // token_reps, errors
#include "rainbow_output.h"  // colours in io

// define string representations at entry-point

char* token_reps[END_OF_FILE + 1] = {
    "CHASM_KWD_CAST",
    "CHASM_KWD_LOOP",
    "CHASM_KWD_IF",
    "LITERAL",
    "VARIABLE",
    "LEFT_BRACE", "RIGHT_BRACE",
    "HTML_OPEN", "HTML_CLOSE", "HTML_CLOSE_CAST",
    "END_OF_FILE",
};

char* errors[FINAL_ERROR] = {
    "EXIT_SUCCESS",
    "EXIT_FILE_NOT_FOUND",
    "MAX_DEPTH_REACHED",
};


int prepare_input() {
    printf(GRN "sanitising input\n" RESET);

    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));

    lexer->token_pos = 0;
    lexer->size      = INITIAL_SIZE;

    if ((lexer->buffer = fopen("chasm.ch", "r")) == NULL) {
        perror(RED "error opening file");
        return 1;
    }

    printf(GRN "file opened\n" RESET);

    lexer->active = fgetc(lexer->buffer);
    lexer->token  = (Token*)malloc(sizeof(Token) * lexer->size);

    tokenize(lexer);       // templator_lexer.c
    iterate_tokens(lexer); // templator_lexer.c
    free_resources(lexer); // templator_lexer.c

    return 0;
}

int main() {
    printf(GRN "chasm.c copies\n" RESET);
    return prepare_input();
}
