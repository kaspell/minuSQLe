#include "table.h"


#define member_sz(strct, attr) sizeof(((strct *)0)->attr)

Table *curr_table = &main_table;

const uint32_t ICOL_SZ = member_sz(Row, icol);
const uint32_t SCOL_SZ = member_sz(Row, scol);

const uint32_t ICOL_OFFSET = 0;
const uint32_t SCOL_OFFSET = ICOL_OFFSET + ICOL_SZ;

const uint32_t ROW_SZ = ICOL_SZ + SCOL_SZ;
const uint32_t PAGE_SZ = 4096;

const uint32_t ROWS_PER_PAGE = PAGE_SZ / ROW_SZ;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;


void
serialize_row(Row *src, void *dest)
{
        memcpy(dest+ICOL_OFFSET, &(src->icol), ICOL_SZ);
        memcpy(dest+SCOL_OFFSET, &(src->scol), SCOL_SZ);
}

void
deserialize_row(void *src, Row *dest)
{
        memcpy(&(dest->icol), src+ICOL_OFFSET, ICOL_SZ);
        memcpy(&(dest->scol), src+SCOL_OFFSET, SCOL_SZ);
}

void
init_table(Table *table, const char *dbfile)
{
        table->col_cnt = TABLE_COLCNT;
        table->pager = curr_pager;
        table->row_cnt = curr_pager->file_sz / ROW_SZ;
}