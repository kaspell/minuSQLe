#include "cursor.h"


extern Table *curr_table;


void
cur_advance(Cursor *cur)
{
        ++cur->row_idx;
        if (cur->row_idx >= cur->table->row_cnt)
                cur->eot = true;
}

void *
cur_eval(Cursor *cur)
{
        uint32_t page_idx = cur->row_idx / ROWS_PER_PAGE;
        void *page = get_page(cur->table->pager, page_idx);
        uint32_t row_offset = cur->row_idx % ROWS_PER_PAGE;
        return page + row_offset * ROW_SZ;
}

Cursor *
cur_seek(uint32_t ridx)
{
        Cursor *cur = (Cursor *) malloc(sizeof(Cursor));
        cur->table = curr_table;
        switch (ridx) {
                case (CSEEK_BEGIN):
                        cur->row_idx = 0;
                        cur->eot = (curr_table->row_cnt == 0);
                        break;
                case (CSEEK_END):
                        cur->row_idx = curr_table->row_cnt;
                        cur->eot = true;
                        break;
                default:
                        cur->row_idx = ridx;
                        cur->eot = (curr_table->row_cnt == ridx);
                        break;
        }
        return cur;
}