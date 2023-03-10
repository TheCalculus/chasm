#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct DOM_OBJ;
typedef struct DOM_OBJ DOM_OBJ;

typedef enum {
    ANCHOR,       /* <a>...</a> */
    BOLD, STRONG, /* <bold/strong></...> */
    ITALIC,       /* <i>...</i> */
    SUB, SUP,     /* <sub/sup></...> */
    PARAGRAPH,    /* <p>...</p> */
    HEADING,      /* <h[n]>...</h[n]> */
    /* todo: add block-level elements */
} DOM_OBJ_TYPE;

typedef struct {
    union { char* stringv; DOM_OBJ* domv; } value;
    enum  { STRINGV, DOMV } active;
} DOM_OBJ_VALUE;

struct DOM_OBJ {
    DOM_OBJ_TYPE   object;
    DOM_OBJ_VALUE* value; /* <...>{value}</...> */
    DOM_OBJ*       parentnode;
    DOM_OBJ*       childnodes;
    int            childcount;
};

typedef DOM_OBJ DOM[10];
int     DOM_INDEX = 0;

int main() {
    char* template = 
        "fe{arr}>\\fe\\";
}
