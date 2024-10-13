#ifndef stmt_h
#define stmt_h

#include "table.h"


typedef enum {
        STMT_INSERT,
        STMT_SELECT
} StmtType;

typedef struct {
        StmtType type;
        Row new_row;
} Stmt;

typedef enum {
        EXEC_SUCCESS,
        EXEC_ERR_TABLE_FULL
} ExecExitStatus;


ExecExitStatus exec_stmt(Stmt *);
ExecExitStatus exec_insert(Stmt *);
ExecExitStatus exec_select(Stmt *);

#endif