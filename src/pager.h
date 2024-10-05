#ifndef pager_h
#define pager_h

#include <fcntl.h>
#include <stdlib.h>


#define TABLE_MAX_PAGES 128

typedef struct {
        int fd;
        uint32_t file_sz;
        uint32_t page_cnt;
        void *pages[TABLE_MAX_PAGES];
} Pager;

extern const char DB_FPATH[10];
Pager main_pager;
extern Pager *curr_pager;


void close_pager(Pager *);
void *get_page(Pager *, uint32_t page_num);
void init_pager(Pager *, const char *dbfile);
void page_flush(Pager *, uint32_t page_idx, uint32_t write_sz);
void pager_flush();

#endif