#ifndef tokenizer_h
#define tokenizer_h

#include <stdbool.h>

#include "term.h"


typedef struct {
        const char *start;
        const char *curr;
} Reader;

typedef enum {
        TOKEN_INSERT,
        TOKEN_SELECT,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_COMMA,
        TOKEN_COLVAL,
        TOKEN_SEP,
        TOKEN_END,
        TOKEN_ERR
} TokenType;

typedef struct {
        TokenType type;
        const char *start;
        int length;
} Token;

Reader reader;
extern Reader *rdr;


Token next_token();
void situate_reader(const char *);

#endif