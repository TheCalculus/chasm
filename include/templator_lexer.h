#ifndef TEMPLATOR_LEXER_H
#define TEMPLATOR_LEXER_H

#define NUM_KEYWORDS 5
#define KWDHASH_LOOP 6385446751 /* equivalent to hash("loop") */
#define KWDHASH_IF   5863476    /* equivalent to hash("if")   */
#define LEXER_INITIAL_SIZE 25

#include "stdio.h"
#include "stdlib.h"
#include "templator_parser.h"

unsigned long hash(const char* str);

typedef enum {
    CHASM_KWD_CAST, /* @... */
    CHASM_KWD_LOOP, /* @loop 10 */
    CHASM_KWD_IF,   /* @if {variable} */
    LITERAL,        /* <...>literal</...> */
    VARIABLE,       /* <...>{variable}</...> */
    LEFT_BRACE, RIGHT_BRACE,
    HTML_OPEN, HTML_CLOSE, HTML_CLOSE_CAST,
    EQUAL_SIGN,
    END_OF_FILE,
} TOK_TYPE;

typedef struct {
    TOK_TYPE type;
    char*    value;
    size_t   size;
} Token;

typedef struct {
    FILE*  buffer;     /* buffer of chasm templator */
    char   active;     /* active character in lexer */
    Token* token;      /* token sequence outputted from lexer */
    size_t size;       /* current size of token sequence */
    size_t position;   /* current position in token sequence */
} Lexer;

void tokenResize  (Lexer* lexer);
void tokenize     (Lexer* lexer, Parser* parser);
void iterateTokens(Lexer* lexer);
void freeLexer(Lexer* lexer);

#endif
