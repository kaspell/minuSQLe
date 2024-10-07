#include "table.h"


extern Table *curr_table;
const char DB_FPATH[10] = "minSQLe.db";
Pager *curr_pager = &main_pager;


void
close_pager(Pager *pager)
{
        pager_flush();
}

void *
allocate_page(Pager *pager, uint32_t page_idx)
{
        void *page = (void *) malloc(PAGE_SZ);
        uint32_t page_cnt = pager->file_sz / PAGE_SZ;
        if (pager->file_sz % PAGE_SZ)
                ++page_cnt;
        if (page_idx <= page_cnt) {
                if (lseek(pager->fd, page_idx*PAGE_SZ, SEEK_SET) < 0) {
                        perror("lseek");
                        exit(EXIT_FAILURE);
                }
                if (read(pager->fd, page, PAGE_SZ) < 0) {
                        perror("read");
                        exit(EXIT_FAILURE);
                }
        }
        return pager->pages[page_idx] = page;
}

void *
get_page(Pager *pager, uint32_t page_idx)
{
        if (page_idx > TABLE_MAX_PAGES) {
                exit(EXIT_FAILURE);
                printf("Page index larger than maximum number of pages: %d", TABLE_MAX_PAGES);
        }
        if (pager->pages[page_idx] == NULL)
                return allocate_page(pager, page_idx);
        return pager->pages[page_idx];
}

void
init_pager(Pager *pager, const char *dbfile)
{
        int fd;
        const char *dbpath = DB_FPATH;
        if (dbfile)
                dbpath = dbfile;
        if ((fd = open(dbpath, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
        }
        pager->file_sz = lseek(fd, 0, SEEK_END);
        pager->fd = fd;
        for (uint32_t i=0; i<TABLE_MAX_PAGES; i++)
                pager->pages[i] = NULL;
}

void
pager_flush()
{
        uint32_t full_pages_cnt = curr_table->row_cnt / ROWS_PER_PAGE;
        uint32_t page_idx = full_pages_cnt;
        uint32_t surplus_row_cnt = curr_table->row_cnt % ROWS_PER_PAGE;
        for (uint32_t i=0; i<full_pages_cnt; i++) {
                if (curr_pager->pages[i] == NULL)
                        continue;
                page_flush(curr_pager, i, PAGE_SZ);
                free(curr_pager->pages[i]);
                curr_pager->pages[i] = NULL;
        }
        if (surplus_row_cnt > 0)
                if (curr_pager->pages[page_idx] != NULL) {
                        page_flush(curr_pager, page_idx, surplus_row_cnt*ROW_SZ);
                        free(curr_pager->pages[page_idx]);
                        curr_pager->pages[page_idx] = NULL;
                }
        if (close(curr_pager->fd) < 0) {
                perror("close");
                exit(EXIT_FAILURE);
        }
        for (uint32_t i=0; i<TABLE_MAX_PAGES; i++) {
                void *page = curr_pager->pages[i];
                if (page) {
                        free(page);
                        curr_pager->pages[i] = NULL;
                }
        }
}


void
page_flush(Pager *pager, uint32_t page_idx, uint32_t write_sz)
{
        if (pager->pages[page_idx] == NULL) {
                printf("Attempted to flush null page\n");
                exit(EXIT_FAILURE);
        }
        if (lseek(pager->fd, page_idx*PAGE_SZ, SEEK_SET) < 0) {
                perror("lseek");
                exit(EXIT_FAILURE);
        }
        if (write(pager->fd, pager->pages[page_idx], write_sz) < 0) {
                perror("write");
                exit(EXIT_FAILURE);
        }
}