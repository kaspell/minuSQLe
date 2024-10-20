#include "cursor.h"


void
cur_advance(Cursor *cur)
{
        ++cur->row_idx;
        cur->eot = (cur->row_idx >= cur->table->row_cnt);
}

Cursor *
cur_create()
{
        Cursor *cur = (Cursor *) malloc(sizeof(Cursor));
        cur->table = tbl();
        return cur;
}

void
cur_destroy(Cursor *cur)
{
        free(cur);
        cur = NULL;
}

void *
cur_eval(Cursor *cur)
{
        u32 page_idx = cur->row_idx / pgr()->rows_per_page;
        u32 row_offset = cur->row_idx % pgr()->rows_per_page;
        return fetch_page(page_idx) + row_offset*tbl()->rowsz;
}

Cursor *
cur_seek(u32 ridx)
{
        Cursor *cur = cur_create();
        switch (ridx) {
                case (CSEEK_BEGIN):
                        cur->row_idx = 0;
                        cur->eot = (tbl()->row_cnt == 0);
                        break;
                case (CSEEK_END):
                        cur->row_idx = tbl()->row_cnt;
                        cur->eot = 1;
                        break;
                default:
                        cur->row_idx = ridx;
                        cur->eot = (tbl()->row_cnt == ridx);
                        break;
        }
        return cur;
}