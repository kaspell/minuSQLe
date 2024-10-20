#ifndef table_h
#define table_h

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "pager.h"


#define COLCNT 3

typedef struct {
        Pager *pager;
        u32 col_cnt;
        u32 row_cnt;
        u32 rowsz;
        u32 max_rowcnt;
} Table;

typedef i32 ColType;

typedef struct {
        ColType cols[COLCNT];
} Row;


Table *tbl();

#endif