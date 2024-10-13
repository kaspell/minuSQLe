#include "table.h"


extern Table *tbl;
const char DB_FPATH[10] = "minSQLe.db";
Pager *pgr = &glob_pager;


void *
alloc_page(u32 page_idx)
{
        void *page = (void *) malloc(pgr->pagesz);
        u32 page_cnt = pgr->file_sz / pgr->pagesz;
        if (pgr->file_sz % pgr->pagesz)
                ++page_cnt;
        if (page_idx <= page_cnt) {
                if (lseek(pgr->fd, page_idx*pgr->pagesz, SEEK_SET) < 0) {
                        perror("lseek");
                        exit(EXIT_FAILURE);
                }
                if (read(pgr->fd, page, pgr->pagesz) < 0) {
                        perror("read");
                        exit(EXIT_FAILURE);
                }
        }
        return pgr->pages[page_idx] = page;
}

void *
fetch_page(u32 page_idx)
{
        if (page_idx > pgr->max_pagecnt) {
                exit(EXIT_FAILURE);
                printf("Page index larger than maximum number of pages: %d", pgr->max_pagecnt);
        }
        if (pgr->pages[page_idx] == NULL)
                return alloc_page(page_idx);
        return pgr->pages[page_idx];
}

void
init_pager(const char *dbfile)
{
        int fd;
        const char *dbpath = DB_FPATH;
        if (dbfile)
                dbpath = dbfile;
        if ((fd = open(dbpath, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
        }
        pgr->file_sz = lseek(fd, 0, SEEK_END);
        pgr->fd = fd;
        for (u32 i=0; i<pgr->max_pagecnt; i++)
                pgr->pages[i] = NULL;
        pgr->pagesz = PAGESZ;
        pgr->max_pagecnt = pgr->max_pagecnt;
}

void
_page_flush(u32 page_idx, u32 write_sz)
{
        if (pgr->pages[page_idx] == NULL) {
                printf("Attempted to flush null page\n");
                exit(EXIT_FAILURE);
        }
        if (lseek(pgr->fd, page_idx*pgr->pagesz, SEEK_SET) < 0) {
                perror("lseek");
                exit(EXIT_FAILURE);
        }
        if (write(pgr->fd, pgr->pages[page_idx], write_sz) < 0) {
                perror("write");
                exit(EXIT_FAILURE);
        }
}

void
page_flush(u32 page_idx, u32 write_sz)
{
        _page_flush(page_idx, write_sz);
        free(pgr->pages[page_idx]);
        pgr->pages[page_idx] = NULL;
}

void
pager_flush()
{
        u32 full_pages_cnt = tbl->row_cnt / pgr->rows_per_page;
        u32 page_idx = full_pages_cnt;
        u32 residue_rows = tbl->row_cnt % pgr->rows_per_page;
        for (u32 i=0; i<full_pages_cnt; i++) {
                if (pgr->pages[i] == NULL)
                        continue;
                page_flush(i, pgr->pagesz);
        }
        if (residue_rows > 0 && (pgr->pages[page_idx] != NULL))
                page_flush(page_idx, residue_rows*tbl->rowsz);
        if (close(pgr->fd) < 0) {
                perror("close");
                exit(EXIT_FAILURE);
        }
        for (u32 i=0; i<pgr->max_pagecnt; i++) {
                void *page = pgr->pages[i];
                if (page) {
                        free(page);
                        pgr->pages[i] = NULL;
                }
        }
}

void
close_pager()
{
        pager_flush();
}