#ifndef TEMPLATOR_LEXER_H
#define TEMPLATOR_LEXER_H

#define NUM_KEYWORDS 5
#define KWDHASH_LOOP 6385446751 /* equivalent to hash("loop") */
#define KWDHASH_IF   5863476    /* equivalent to hash("if")   */
#define INITIAL_SIZE 25

unsigned long hash (const char* str);

typedef enum {
    ANCHOR,       /* <a>...</a> */
    BOLD, STRONG, /* <bold/strong></...> */
    ITALIC,       /* <i>...</i> */
    SUB, SUP,     /* <sub/sup></...> */
    PARAGRAPH,    /* <p>...</p> */
    UNORD_LIST,   /* <ul>...</ul> */
    LIST_ITEM,    /* <..><li></li></..> */
    HEADING,      /* <h[n]>...</h[n]> */
} DOM_OBJ_TYPE;

typedef enum {
    CHASM_KWD_CAST, /* @... */
    CHASM_KWD_LOOP, /* @loop 10 */
    CHASM_KWD_IF,   /* @if {variable} */
    LITERAL,        /* <...>literal</...> */
    VARIABLE,       /* <...>{variable}</...> */
    LEFT_BRACE, RIGHT_BRACE,
    HTML_OPEN, HTML_CLOSE, HTML_CLOSE_CAST,
    END_OF_FILE,
} TOK_TYPE;

typedef struct {
    TOK_TYPE type;
    char*    value;
    int      size;
} Token;

typedef enum {
    EXIT_SUCCESS_CH,
    EXIT_FILE_NOT_FOUND,
    MAX_DEPTH_REACHED,
    FINAL_ERROR,
} LEXER_ERR;

typedef struct DOM_OBJ DOM_OBJ;
typedef struct DOM_OBJ_VALUE DOM_OBJ_VALUE;

struct DOM_OBJ {
    DOM_OBJ_TYPE   type;
    DOM_OBJ_VALUE* value;
    DOM_OBJ*       children;
    DOM_OBJ*       parent;
    int childcount;
    int parentcount;
};

struct DOM_OBJ_VALUE {
    union { DOM_OBJ node; char* plaintext; char* parsetext; } value;
    enum  { NODE, PLAINTEXT, PARSETEXT } active;
};

typedef struct {
    DOM_OBJ*  DOM;     /* dom representation of webpage */
    FILE*  buffer;     /* buffer of chasm templator */
    char   active;     /* active character in lexer */
    Token* token;      /* token sequence outputted from lexer */
    size_t size;       /* current capacity of token sequence */
    size_t token_pos;  /* current position in token sequence */
} Lexer;

void          tokenize      (Lexer* lexer);
void          iterate_tokens(Lexer* lexer);
void          free_resources(Lexer* lexer);

#endif
