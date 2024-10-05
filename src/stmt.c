#include "cursor.h"
#include "stmt.h"
#include "table.h"


ExecExitStatus
exec_insert(Stmt *stmt)
{
        if (curr_table->row_cnt >= TABLE_MAX_ROWS)
                return EXEC_TABLE_FULL;
        Cursor *cur = cur_seek(CSEEK_END);
        serialize_row(&(stmt->to_insert), cur_eval(cur));
        ++curr_table->row_cnt;
        free(cur);
        return EXEC_SUCCESS;
}

ExecExitStatus
exec_select(Stmt *stmt)
{
        Cursor *cur = cur_seek(CSEEK_BEGIN);
        Row row;
        while (!(cur->eot)) {
                deserialize_row(cur_eval(cur), &row);
                printf("%d, %s\n", row.icol, row.scol);
                cur_advance(cur);
        }
        free(cur);
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