#include "prepr.h"
#include "term.h"
#include "util.h"


char prompt[2] = "#";
size_t max_cmd_sz = MAX_CMD_SZ;


void
run_interface(const char *dbfile)
{
        ssize_t rbytes;
        dbfpath((char *)dbfile);
        while (1) {
                printf("%s ", prompt);
                if ((rbytes = getline(&buffer_in[0], &max_cmd_sz, stdin)) < 0) {
                        perror("getline");
                        exit(EXIT_FAILURE);
                }
                if (!strncmp(buffer_in[0], "\\quit", strlen("\\quit"))) {
                        close_db_connection();
                        exit(EXIT_SUCCESS);
                }

                Stmt stmt;
                if (prepr_stmt(&stmt) < 0) {
                        printf("%s", error);
                        continue;
                }
                exec_stmt(&stmt);
        }
        close_db_connection();
}