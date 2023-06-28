#ifndef TEMPLATOR_PARSER_H
#define TEMPLATOR_PARSER_H

typedef enum {
    PAIRED,
    UNPAIRED, // current code will assume paired
              // unpaired to be implemented
} TAG_PAIRING;

typedef enum {
    BLOCK,
    CONTENT,
    ATTRIBUTE,
} CONTEXT;

typedef struct Node Node;

struct Node {
    TAG_PAIRING pairing;
    CONTEXT     context;
    Node*       children;
    Node*       parent;
    char**      attributes;
    char**      value;
    int childcount;
    int parentcount;
};

typedef struct {
    Node*  nodes;    /* node sequence outputted from parsing while tokenizing */
    size_t size;     /* current size of node sequence */
    size_t position; /* current position in node sequence */
} Parser;

#endif

/* oh boy are you in for a treat */
// 1) check what the previous token was (if any)
// 2) figure out whether the previous token was a tag
// 3) if the previous token was a tag then we're probably at an
// ...attribute. isnt that cool?
// 4) if we're an = after an attribute or a string after an =, 
// ...we need to appropriately set the value of the node
// 5) profit. syntax errors are not my problem. i mean how hard is it
// ...to mess up nearly vanilla HTML? jk ill figure something out for
// ...syntax errors later, but i need to get the base system working first
// 6: system is just a test, i didnt want to iterate throughout entire token
// ...array again for efficiency reasons. maybe ill remove the token array
// ...entirely and only store the previous and current token if this system
// ...works pretty well?