/* templator_lexer.c reset
file has to be modified */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/templator_lexer.h"
#include "../include/templator_parser.h"
#include "../include/string_reps.h"
#include "../include/rainbow_output.h"

void scanLiterals(Lexer* lexer, Token* token) {
    if (isalpha(lexer->active)) {
        char* literal = (char*)malloc(sizeof(char) * 10);
        int      size = 0;

        do {
            literal[size++] = lexer->active;
            lexer->active = getc(lexer->buffer);
        } while (isalnum(lexer->active) && size < 10);

        literal[++size] = '\0';

        unsigned long hashedLiteral = hash(literal); // templator_lexer.h

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
        token->value = (char*)realloc(token->value, sizeof(char) * size);
        strcpy(token->value, literal);

        if (!isspace(lexer->active))
            ungetc(lexer->active, lexer->buffer);

        free(literal);
        token->size  = size;
    }
}

void tokenize(Lexer* lexer, Parser* parser) {
    while ((lexer->active = getc(lexer->buffer)) != EOF) {
        if (isspace(lexer->active) || !isprint(lexer->active))
            continue;
        
        Token token;
        token.size  = 1;
        token.value = (char*)malloc(sizeof(char) * token.size);
        strcpy(token.value, &lexer->active);

        switch (lexer->active) {
            case '<':
            {
                token.type = HTML_OPEN;
                Node* node = defaultNode();

                if (parser->active == NULL) {
                    parser->nodes[parser->position++] = *node;
                    parser->active = node;
                    break;
                }

                printf("%s ", token.value);
                scanLiterals(lexer, &token);
                printf("%s\n", token.value);

                if (!strcmp(token.value, parser->active->attributes[0])) {
                    printf("we're at the ending tag for %s\n", token.value);

                    node->parent = parser->active;
                    node->isNested = true;

                    parser->active->children[parser->active->childPosition++] = node;
                    parser->active = node;
                }

                // freeNode(node);

                break;
            }
            case '>': token.type = HTML_CLOSE;      break;
            case '{': token.type = LEFT_BRACE;      break;
            case '}': token.type = RIGHT_BRACE;     break;
            case '@': token.type = CHASM_KWD_CAST;  break;
            case '/': 
            {
                token.type = HTML_CLOSE_CAST; 

                Node* activeNode = parser->active;

                activeNode->hasFinishedDeclaration = true;
                activeNode = activeNode->parent;

                break;
            }
            case '=': token.type = EQUAL_SIGN;      break;
            default: 
            {
                printf("%s ", token.value);
                scanLiterals(lexer, &token);
                printf("%s\n", token.value);

                Node* activeNode = parser->active;

                if (activeNode != NULL) {
                    if (!activeNode->hasFinishedDeclaration) {
                        attributeResize(activeNode);
                        strncpy(activeNode->attributes[activeNode->attrPosition++], token.value, 5);
                    }
                    break;
                }

                printf("NULL activeNode for '%s'\n", token.value);
            }
        }

        tokenResize(lexer);
      
        ungetc(lexer->active, lexer->buffer);
        memcpy(&lexer->token[lexer->position], &token, sizeof(token));

        lexer->active = getc(lexer->buffer);
        lexer->position++;
    }

    lexer->token[lexer->position++] = (Token) { END_OF_FILE, "", 1 };
}

void tokenResize(Lexer* lexer) {
    if (lexer->position >= lexer->size) {
        lexer->size  += 50;
        lexer->token = (Token*)realloc(lexer->token, sizeof(Token) * lexer->size);
        printf("Token* resized to %zu\n", lexer->size);
    }
}

void iterateTokens(Lexer* lexer) {
    printf(GRN "beginning token iteration\n" RESET);
    int index = 0;
    while (lexer->token[index].type != END_OF_FILE && index <= 1000) {
        printf("%-10s %-10s\n", lexer->token[index].value,
                token_reps[lexer->token[index].type]);
        index++;
    }
}

void freeResources(Lexer* lexer) {
    fclose(lexer->buffer);
    free(lexer->token); 
    free(lexer);
}

void freeParser(Parser* parser) {

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

void freeNode(Node* node) {
    for (int i = 0; i < node->attrSize; i++)
        free(node->attributes[i]);

    for (int i = 0; i < node->childSize; i++)
        free(node->value[i]);

    free(node->attributes);
    free(node->value);
}

void attributeResize(Node* node) {
    if (node->attrPosition >= node->attrSize) {
        node->attrSize   += 5;
        node->attributes = (char**)realloc(node->attributes, sizeof(char*) * node->attrSize);
        
        for (int i = 0; i < 5; i++)
            node->attributes[node->attrSize - i - 1] = (char*)malloc(sizeof(char) * 5);
       
        printf("node->attributes (char**) resized to %zu\n", node->attrSize);
    }
}

unsigned long hash(const char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}
