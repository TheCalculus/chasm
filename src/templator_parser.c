#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "templator_lexer.h"
#include "string_reps.h"

typedef enum {
    ANCHOR,             /* <a>...</a> */
    BOLD, STRONG,       /* <b/strong>...</b/strong> */
    ITALIC,             /* <i>...</i> */
    SUB, SUP,           /* <sub/sup>...</sub/sup> */
    PARAGRAPH,          /* <p>...</p> */
    UNORD_LIST,         /* <ul>...</ul> */
    LIST_ITEM,          /* <li>...</li> */
    HEADING,            /* <h1>...</h1>, <h2>...</h2>, ... */
    IMAGE,              /* <img src="..." alt="..."> */
    DIV,                /* <div>...</div> */
    SPAN,               /* <span>...</span> */
    TABLE,              /* <table>...</table> */
    TABLE_ROW,          /* <tr>...</tr> */
    TABLE_HEADER,       /* <th>...</th> */
    TABLE_DATA,         /* <td>...</td> */
    FORM,               /* <form>...</form> */
    INPUT,              /* <input type="..." /> */
    BUTTON,             /* <button>...</button> */
    LABEL,              /* <label>...</label> */
    SELECT,             /* <select>...</select> */
    OPTION,             /* <option>...</option> */
    TEXTAREA,           /* <textarea>...</textarea> */
    LINE_BREAK,         /* <br> */
    HORIZONTAL_RULE,    /* <hr> */
    BODY,               /* <body>...</body> */
    HTML,               /* <html>...</html> */
    STYLE,              /* <style>...</style> */
    SCRIPT,             /* <script>...</script> */
    META,               /* <meta> */
    COMMENT,            /* <!-- ... --> */
    DOCTYPE             /* <!DOCTYPE> */
} NODE_TYPE;

struct NODE_VALUE {
    union { char* plaintext; char* parsetext; } value;
    enum  { PLAINTEXT, PARSETEXT } active;
};

typedef struct NODE       NODE;
typedef struct NODE_VALUE NODE_VALUE;

struct NODE {
    NODE_TYPE   type;
    NODE_VALUE  value;
    NODE*       children;
    NODE*       parent;
    char**      attributes;
    int childcount;
    int parentcount;
};

/* 
    <body>
        <div class="container">
            <p class="title">TITLE!</span>
        </div>
    </body>
    -------
    NODE(
        BODY,
        NODE(
            DIV,
            NODE(
                P,
                "TITLE!",
                "class='title'"
            ),
            "class='container'",
            1, // p
            2, // html, body
        ),
        2, // div, p
        1, // html
    )
*/