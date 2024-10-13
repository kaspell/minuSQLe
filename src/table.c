#include "table.h"


Table *tbl = &glob_table;


void
init_table(const char *dbfile)
{
        tbl->col_cnt = COLCNT;
        tbl->rowsz = sizeof(ColType) * COLCNT; 
        tbl->pager = pgr;
        tbl->row_cnt = pgr->file_sz / tbl->rowsz;
        pgr->rows_per_page = pgr->pagesz / tbl->rowsz;
        tbl->max_rowcnt = pgr->rows_per_page*pgr->max_pagecnt;
}