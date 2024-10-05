#ifndef cursor_h
#define cursor_h

#include <stdbool.h>

#include "table.h"


#define CSEEK_BEGIN 0
#define CSEEK_END -1


typedef struct {
        Table *table;
        uint32_t row_idx;
        bool eot;
} Cursor;


void cur_advance(Cursor *);
void *cur_eval(Cursor *);
Cursor *cur_seek(uint32_t ridx);

#endif