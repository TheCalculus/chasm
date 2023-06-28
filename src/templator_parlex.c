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
                token.type = HTML_OPEN;
                Node node = {
                    .attributes = (char**)malloc(sizeof(char*) * 5),
                    .value      = (char**)malloc(sizeof(char*) * 5),
                    .attrSize   = 5,
                    .childSize  = 5,
                };

                if (parser->active == NULL) {
                    parser->nodes[parser->position++] = node;
                    parser->active = &node;
                    parser->size++;
                    break;
                }

                node = (Node) {
                    .parent     = parser->active,
                    .isNested   = true,
                };

                parser->active->children[parser->active->childCount++] = node;
                parser->active = &node;

                // free during testing because i know im going to cause troubles otherwise
                free(node.attributes);
                free(node.value);

                break;
            case '>': token.type = HTML_CLOSE;      break;
            case '{': token.type = LEFT_BRACE;      break;
            case '}': token.type = RIGHT_BRACE;     break;
            case '@': token.type = CHASM_KWD_CAST;  break;
            case '/': 
                token.type = HTML_CLOSE_CAST; 

                parser->active->hasFinishedDeclaration = true;
                parser->active = parser->active->parent;
                
                break;
            case '=': token.type = EQUAL_SIGN;      break;
            default: 
                scanLiterals(lexer, &token);

                if (!parser->active->hasFinishedDeclaration) {
                    // strncpy wont work because each char* in node.attributes is not malloc'd
                    strncpy(node.attributes[node.attrCount++], token.value, token.size);
                }
        }

        if(lexer->position >= lexer->size) {
            lexer->size += 50;
            lexer->token = (Token*)realloc(lexer->token, lexer->size * sizeof(Token));
            printf("Token* resized to %zu\n", lexer->size);
        }

        ungetc(lexer->active, lexer->buffer);
        memcpy(&lexer->token[lexer->position], &token, sizeof(token));
        fflush(stdout);

        lexer->active = getc(lexer->buffer);
        lexer->position++;
    }

    lexer->token[lexer->position++] = (Token) { END_OF_FILE, "", 1 };
}

void iterate_tokens(Lexer* lexer) {
    int index;
    while (lexer->token[index].type != END_OF_FILE) {
        printf("%-10s %-10s\n", lexer->token[index].value,
                token_reps[lexer->token[index].type]);
        index++;
    }
}

void free_resources(Lexer* lexer) {
    fclose(lexer->buffer);
    free(lexer->token); 
    free(lexer);
}

unsigned long hash(const char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}