#include "util.h"
#include "table.h"


void
close_db_connection()
{
        close_pager(curr_pager);
}

void
open_db_connection(const char *dbfile)
{
        init_pager(curr_pager, dbfile);
        init_table(curr_table, dbfile);
}