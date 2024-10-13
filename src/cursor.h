#ifndef cursor_h
#define cursor_h

#include "table.h"


#define CSEEK_BEGIN 0
#define CSEEK_END -1


typedef struct {
        Table *table;
        u32 row_idx;
        u8 eot;
} Cursor;


void cur_advance(Cursor *);
Cursor *cur_create();
void cur_destroy(Cursor *cur);
void *cur_eval(Cursor *);
Cursor *cur_seek(u32 ridx);

#endif