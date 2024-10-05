#ifndef table_h
#define table_h

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pager.h"


#define TABLE_COLCNT 2
#define MAX_SLEN 255
#define member_sz(strct, attr) sizeof(((strct *)0)->attr)

const uint32_t ICOL_SZ;
const uint32_t SCOL_SZ;

const uint32_t ICOL_OFFSET;
const uint32_t SCOL_OFFSET;

const uint32_t ROW_SZ;
const uint32_t PAGE_SZ;

const uint32_t ROWS_PER_PAGE;
const uint32_t TABLE_MAX_ROWS;

typedef struct {
        Pager *pager;
        uint32_t row_cnt;
        uint32_t col_cnt;
} Table;

typedef struct {
        int32_t icol;
        char scol[MAX_SLEN + 1];
} Row;

Table main_table;
extern Table *curr_table;


void deserialize_row(void *src, Row *dest);
void init_table(Table *, const char *dbfile);
void serialize_row(Row *src, void *dest);

#endif