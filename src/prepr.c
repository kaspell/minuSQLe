#include <string.h>

#include "prepr.h"
#include "table.h"
#include "term.h"
#include "tokenizer.h"


int
prepr_insert(Stmt *stmt)
{
        char fields[curr_table->col_cnt][255];
        Token toks[curr_table->col_cnt];
        for (int i=0; i<curr_table->col_cnt; i++) {
                toks[i] = next_token();
                strncpy(fields[i], toks[i].start, toks[i].length);
                fields[i][toks[i].length + 1] = '\0';
        }
        stmt->type = STMT_INSERT;

        if (toks[0].type != TOKEN_COLVAL || toks[1].type != TOKEN_COLVAL) {
                sprintf(error, "Syntax error in defining column values\n");
                return -1;
        }
        if (toks[1].length > MAX_SLEN) {
                sprintf(error, "Provided string too long: %s\n", toks[1].start);
                return -1;
        }
        stmt->to_insert.icol = atoi(fields[0]);
        strcpy(stmt->to_insert.scol, fields[1]);
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
                case TOKEN_SELECT:
                        stmt->type = STMT_SELECT;
                        return 0;
                case TOKEN_INSERT:
                        return prepr_insert(stmt);
                case TOKEN_ERR:
                default:
                        sprintf(error, "Unrecognized keyword: %s\n", buffer_in[0]);
                        return -1;
                        break;
        }
}