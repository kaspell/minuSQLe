#ifndef term_h
#define term_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_CMD_SZ 2048
#define MAX_ERR_SZ 2048

char *buffer_in[MAX_CMD_SZ];
char error[MAX_ERR_SZ];
extern size_t max_cmd_sz;
char prompt[2];


void run_interface(const char *dbfile);

#endif