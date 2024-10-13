#include <assert.h>
#include <string.h>

#include "../src/tokenizer.h"


void assert_next_token(TokenType type, char *s)
{
        Token token = next_token();
        assert(token.type == type);
        assert(strncmp(token.start, s, token.length) == 0);
}

void
assert_next_token_type(TokenType type)
{
        Token token = next_token();
        assert(token.type == type);
}

void
test_tokenizer_insert_capture_000()
{
        void situate_reader(const char *);
        char buf[] = "INSERT 0 -1 500 abc xyz ABC;";
        situate_reader(buf);
        assert_next_token_type(TOKEN_INSERT);
        assert_next_token(TOKEN_COLVAL, "0");
        assert_next_token(TOKEN_COLVAL, "-1");
        assert_next_token(TOKEN_COLVAL, "500");
        assert_next_token(TOKEN_COLVAL, "abc");
        assert_next_token(TOKEN_COLVAL, "xyz");
        assert_next_token(TOKEN_COLVAL, "ABC");
        assert_next_token_type(TOKEN_SEP);
        assert_next_token_type(TOKEN_END);
}

void
test_tokenizer_insert_capture_001()
{
        void situate_reader(const char *);
        char buf[] = "INSERT -5000,60,XYZ,000,ptolemy;";
        situate_reader(buf);
        assert_next_token_type(TOKEN_INSERT);
        assert_next_token(TOKEN_COLVAL, "-5000");
        assert_next_token(TOKEN_COLVAL, "60");
        assert_next_token(TOKEN_COLVAL, "XYZ");
        assert_next_token(TOKEN_COLVAL, "000");
        assert_next_token(TOKEN_COLVAL, "ptolemy");
        assert_next_token_type(TOKEN_SEP);
        assert_next_token_type(TOKEN_END);
}

void test_tokenizer_select_capture()
{
        void situate_reader(const char *);
        char buf[8] = "SELECT;";
        situate_reader(buf);
        assert_next_token_type(TOKEN_SELECT);
        assert_next_token_type(TOKEN_SEP);
        assert_next_token_type(TOKEN_END);
}

int main()
{
        test_tokenizer_insert_capture_000();
        test_tokenizer_insert_capture_001();
        test_tokenizer_select_capture();
}