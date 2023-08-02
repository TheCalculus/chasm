#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/templator_lexer.h"
#include "../include/templator_parser.h"
#include "../include/string_reps.h"
#include "../include/rainbow_output.h"

void scanLiterals(Token* token) {
    if (isprint(lexer->active)) {
        char* literal = (char*)malloc(sizeof(char) * 11);
        int size = 0;

        do {
            literal[size++] = lexer->active;
            lexer->active = getc(lexer->buffer);
        } while (isalnum(lexer->active) && size < 10);

        literal[size] = '\0';

        unsigned long hashedLiteral = hash(literal);

        switch (hashedLiteral) {
            case KWDHASH_LOOP:
                token->type = CHASM_KWD_LOOP;
                goto exit;
            case KWDHASH_IF:
                token->type = CHASM_KWD_IF;
                goto exit;
        }

        token->type = LITERAL;

    exit:
        token->value = (char*)realloc(token->value, sizeof(char) * (size + 1));
        strcpy(token->value, literal);

        if (!isspace(lexer->active) && lexer->active != EOF)
            ungetc(lexer->active, lexer->buffer);

        free(literal);
        token->size = size;
    }
}

void generateNodeTree() {
    while ((lexer->active = getc(lexer->buffer)) != EOF) {
        if (isspace(lexer->active) || !isprint(lexer->active))
            continue;

        switch (lexer->active) {
            case '<':
            {
                Node* node = defaultNode();

                if (parser->active == NULL) {
                    parser->nodes[parser->position++] = *node;
                    parser->active = node;
                    break;
                }

                Token tmp;
                tmp.value = (char*)malloc(sizeof(char));

                lexer->active = getc(lexer->buffer);
                scanLiterals(&tmp);

                // next character isn't alnum (/)
                if (*tmp.value == '<') {
                    lexer->active = getc(lexer->buffer);
                    scanLiterals(&tmp);
                }

                // scanLiterals causes the lexer->position to increase by tmp.size
                for (int i = tmp.size; i >= 0; i--)
                    ungetc(tmp.value[i], lexer->buffer);

                if (strncmp(tmp.value, parser->active->attributes[0], tmp.size) == 0) {
                    // printf("'%s' == '%s' (end tag found)\n", tmp.value, parser->active->attributes[0]);

                    parser->active->hasFinishedDeclaration = true;
                    parser->active = parser->active->parent;
                } else {
                    node->parent = parser->active;
                    node->isNested = true;

                    parser->active->children[parser->active->childPosition++] = node;
                    parser->active = node;
                }

                free(tmp.value);

                break;
            }
            case '>': break;
            case '{': break;
            case '}': break;
            case '@': break;
            case '=': break;
            case '/': break;
            default:
            {
                Token token = {
                    .size  = 1,
                    .value = (char*)malloc(sizeof(char) * 1),
                };
                
                scanLiterals(&token);
                Node* activeNode = parser->active;

                if (activeNode != NULL && !activeNode->hasFinishedDeclaration) {
                    attributeResize(activeNode);
                    // strncpy(activeNode->attributes[activeNode->attrPosition++], token.value, 2);
                    memcpy(activeNode->attributes[activeNode->attrPosition++], token.value, 5);
                }
            }
        }

        tokenResize();

        ungetc(lexer->active, lexer->buffer);

        lexer->active = getc(lexer->buffer);
        lexer->position++;
    }

    lexer->token[lexer->position++] = (Token) { END_OF_FILE, "", 1 };
}

void iterateTokens() {
    printf(GRN "beginning token iteration\n" RESET);
    int index = 0;
    while (lexer->token[index].type != END_OF_FILE && index <= 1000) {
        printf("%-10s %-10s\n", lexer->token[index].value,
                token_reps[lexer->token[index].type]);
        index++;
    }
}

void freeLexer() {
    fclose(lexer->buffer);
    free(lexer->token);
    free(lexer);

    lexer = NULL;
}

void freeNode(Node* node) {
    if (node == NULL)
        return;

    for (size_t i = 0; i < node->attrPosition; i++)
        free(node->attributes[i]);

    if (node->isNested) {
        for (size_t i = 0; i < node->childPosition; i++)
            freeNode(node->children[i]);

        free(node->children);
    }

    free(node->value);
    free(node);
}

void freeParser() {
    if (parser == NULL)
        return;

    for (size_t i = 0; i < parser->position; i++)
        freeNode(&parser->nodes[i]);

    free(parser);
}

Node* defaultNode() {
    int size = 5;

    Node* node = (Node*)malloc(sizeof(Node));

    node->attributes = (char**)malloc(sizeof(char*) * size);
    node->value      = (char**)malloc(sizeof(char*) * size);
    node->children   = (Node**)malloc(sizeof(Node*) * size); /* constant, max 5 children for debug */
    node->attrSize   = size;
    node->childSize  = size;
    node->isNested   = false;
    node->hasFinishedDeclaration = false;

    for (int i = 0; i < size; i++) {
        node->attributes[i] = (char*)malloc(sizeof(char) * size);
        node->value[i]      = (char*)malloc(sizeof(char) * size);
        node->children[i]   = (Node*)malloc(sizeof(Node));
    }
    
    return node;
}


void tokenResize() {
    if (lexer->position >= lexer->size) {
        lexer->size += 50;
        lexer->token = (Token*)realloc(lexer->token, sizeof(Token) * lexer->size);
        // printf("Token* resized to %zu\n", lexer->size);
    }
}

void attributeResize(Node* node) {
    if (node->attrPosition >= node->attrSize) {
        node->attrSize++;
        node->attributes = (char**)realloc(node->attributes, sizeof(char*) * node->attrSize);

        if (node->attributes == NULL)
            exit(1);

        node->attributes[node->attrSize - 1] = (char*)malloc(sizeof(char) * 5);
    }
}

char* strcat_steroids(char* dest, char* src) {
    while (*dest) dest++;
    while ((*dest++ = *src++))
        ;

    return --dest;
}

void iterateAllChildren(Node* node) {
    if (node == NULL) {
        printf("node is NULL\n");
        return;
    }

    if (node->attributes == NULL) {
        printf("this aint right...");
        fflush(stdout);
        exit(1);
    } else {
        printf("attrSize: %zu\n", node->attrSize);
        printf("attributes: %p\n", node->attributes); // no crash
        printf("attributes[0]: %s\n", &node->attributes); // segfault
    }

    for (int i = 0; i < node->childPosition; i++)
        iterateAllChildren(node->children[i]);
}

unsigned long hash(const char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}
