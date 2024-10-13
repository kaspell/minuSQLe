#include "cursor.h"
#include "stmt.h"
#include "table.h"


ExecExitStatus
exec_insert(Stmt *stmt)
{
        if (tbl->row_cnt >= tbl->max_rowcnt)
                return EXEC_ERR_TABLE_FULL;
        Cursor *cur = cur_seek(CSEEK_END);
        memcpy(cur_eval(cur), &(stmt->new_row.cols), tbl->rowsz);
        ++tbl->row_cnt;
        cur_destroy(cur);
        return EXEC_SUCCESS;
}

ExecExitStatus
exec_select(Stmt *stmt)
{
        Cursor *cur = cur_seek(CSEEK_BEGIN);
        Row row;
        while (!cur->eot) {
                memcpy(&(row.cols), cur_eval(cur), tbl->rowsz);
                for (int i=0; i<COLCNT; i++)
                        printf("%d ", row.cols[i]);
                printf("\n");
                cur_advance(cur);
        }
        cur_destroy(cur);
        return EXEC_SUCCESS;
}

ExecExitStatus
exec_stmt(Stmt *stmt)
{
        switch (stmt->type) {
                case (STMT_INSERT):
                        return exec_insert(stmt);
                case (STMT_SELECT):
                        return exec_select(stmt);
                default:
                        break;
        }
}