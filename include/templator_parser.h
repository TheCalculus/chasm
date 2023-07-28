#ifndef TEMPLATOR_PARSER_H
#define TEMPLATOR_PARSER_H

#define PARSER_INITIAL_SIZE 25

#include <stdbool.h>

typedef enum {
    PAIRED,
    UNPAIRED, // current code will assume paired
              // unpaired to be implemented
} TAG_PAIRING;

typedef struct Node Node;

struct Node {
    Node**   children;
    Node*    parent;
    char**   attributes;
    char**   value;
    bool     isNested;
    bool     hasFinishedDeclaration;
    size_t   attrPosition,  attrSize;
    size_t   childPosition, childSize;
};

typedef struct {
    Node*  nodes;    /* node sequence outputted from parsing while tokenizing */
    Node*  active;   /* currently active node */
    size_t size;     /* current size of node sequence */
    size_t position; /* current position in node sequence */
} Parser;

void  freeNode(Node* node);
void  freeParser(Parser* parser);
Node* defaultNode();
void  attributeResize(Node* node);
void  parseTreeToHTML(Node* node, int level);

#endif