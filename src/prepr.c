#include <string.h>

#include "prepr.h"
#include "table.h"
#include "term.h"
#include "tokenizer.h"


int
prepr_insert(Stmt *stmt)
{
        char fields[tbl->col_cnt][255];
        Token toks[tbl->col_cnt];
        stmt->type = STMT_INSERT;
        for (int i=0; i<tbl->col_cnt; i++) {
                toks[i] = next_token();
                strncpy(fields[i], toks[i].start, toks[i].length);
                fields[i][toks[i].length] = '\0';
        }
        for (int i=0; i<tbl->col_cnt; i++) {
                if (toks[i].type != TOKEN_COLVAL) {
                        sprintf(error, "Error in defining column values\n");
                        return -1;
                }
                stmt->new_row.cols[i] = atoi(fields[i]);
        }
        return 0;
}

int
prepr_stmt(Stmt *stmt)
{
        buffer_in[0][strlen(buffer_in[0])-1] = '\0';
        situate_reader(buffer_in[0]);
        Token token = next_token();
        TokenType ttype = token.type;
        switch (ttype) {
                case TOKEN_SELECT: {
                        stmt->type = STMT_SELECT;
                        return 0;
                }
                case TOKEN_INSERT:
                        return prepr_insert(stmt);
                case TOKEN_ERR:
                default: {
                        sprintf(error, "Unrecognized keyword: %s\n", buffer_in[0]);
                        return -1;
                        break;
                }
        }
}