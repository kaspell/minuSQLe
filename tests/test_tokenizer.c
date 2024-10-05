#include <assert.h>
#include <string.h>

#include "../src/tokenizer.h"

void test_tokenizer_insert_capture()
{
        Token token;
        void situate_reader(const char *);
        char buf[13] = "INSERT 0 abc;";
        situate_reader(buf);
        assert(next_token().type == TOKEN_INSERT);
        token = next_token();
        assert(token.type == TOKEN_COLVAL);
        assert(strncmp(token.start, "0", token.length) == 0);
        token = next_token();
        assert(token.type == TOKEN_COLVAL);
        assert(strncmp(token.start, "abc", token.length) == 0);
        assert(next_token().type == TOKEN_SEP);
        assert(next_token().type == TOKEN_END);
}

void test_tokenizer_select_capture()
{
        void situate_reader(const char *);
        char buf[8] = "SELECT;";
        situate_reader(buf);
        assert(next_token().type == TOKEN_SELECT);
        assert(next_token().type == TOKEN_SEP);
        assert(next_token().type == TOKEN_END);
}

int main()
{
        test_tokenizer_insert_capture();
        test_tokenizer_select_capture();
}