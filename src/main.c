#include "term.h"


int
main(int argc, char *argv[])
{
        const char *dbfile = NULL;
        if (argc > 1)
                dbfile = argv[1];
        run_interface(dbfile);
}