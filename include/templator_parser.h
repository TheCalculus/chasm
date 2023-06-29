#ifndef TEMPLATOR_PARSER_H
#define TEMPLATOR_PARSER_H

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
    int      attrCount,  attrSize;   /* count is position, size is capacity */
    int      childCount, childSize;  /* either char* within value (char**) or Node within children (Node*). 
                                        need to add support for both instead of either-or */
};

typedef struct {
    Node*  nodes;    /* node sequence outputted from parsing while tokenizing */
    Node*  active;   /* currently active node */
    size_t size;     /* current depth of node sequence */
    size_t position; /* current position in node sequence */
} Parser;

Node* defaultNode();
void  freeNode       (Node* node);
void  attributeResize(Node* node);

#endif