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
    Node*    children;
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

Node defaultNode() {
    int size = 5;

    Node node = {
        .attributes = (char**)malloc(sizeof(char*) * size),
        .value      = (char**)malloc(sizeof(char*) * size),
        .attrSize   = size,
        .childSize  = size,
    };

    for (int i = 0; i < size; i++) {
        node.attributes[i] = (char*)malloc(sizeof(char) * size);
        node.value[i]      = (char*)malloc(sizeof(char) * size);
    }
}

void freeNode(Node* node) {
    for (int i = 0; i < node->attrSize; i++)
        free(node->attributes[i]);

    for (int i = 0; i < node->childSize; i++)
        free(node->value[i]);

    free(node->attributes);
    free(node->value);
}

int attributeResize(Node* node) {
    if (node->attrCount >= node->attrSize) {
        node->attrSize   += 5;
        node->attributes = (char**)realloc(node->attributes, sizeof(char*) * node->attrSize);
        
        return node->attributes;
    }

    return 1;
}

#endif