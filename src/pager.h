#ifndef pager_h
#define pager_h

#include <fcntl.h>
#include <stdlib.h>

#include "common.h"


#define MAX_PAGECNT 128
#define PAGESZ 4096

typedef struct {
        int fd;
        u32 file_sz;
        u32 page_cnt;
        void *pages[MAX_PAGECNT];
        u32 rows_per_page;
        u32 pagesz;
        u32 max_pagecnt;
} Pager;


void close_pager();
char *dbfpath(char *);
void *fetch_page(u32 page_num);
Pager *pgr();

#endif