#include "tokenizer.h"


Reader *rdr = &reader;


void
advance_reader()
{
        ++rdr->curr;
}

bool
is_at_end()
{
        return (*rdr->curr == '\0');
}

bool
is_at_start()
{
        return (rdr->curr == rdr->start);
}

bool
isalph(char c)
{
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool
iscolvalsymbol(char c)
{
        return isalph(c) || (c >= '0' && c <= '9') || (c == '-');
}

void
situate_reader(const char *s)
{
        rdr->start = s;
        rdr->curr = s;
}

void
start_at_curr()
{
        rdr->start = rdr->curr;
}

char
curr_chr()
{
        return *rdr->curr;
}

Token
form_token(TokenType type)
{
        Token token;
        token.start = rdr->start;
        token.type = type;
        token.length = (int)(rdr->curr - rdr->start);
        return token;
}

Token
stmt_invocation()
{
        start_at_curr();
        while (isalph(curr_chr()))
                advance_reader();
        if (!strncmp(rdr->start, "select", strlen("select")) || !strncmp(rdr->start, "SELECT", strlen("SELECT")))
                return form_token(TOKEN_SELECT);
        if (!strncmp(rdr->start, "insert", strlen("insert")) || !strncmp(rdr->start, "INSERT", strlen("INSERT")))
                return form_token(TOKEN_INSERT);
        return form_token(TOKEN_ERR);
}

Token
colval()
{
        start_at_curr();
        while (iscolvalsymbol(curr_chr()))
                advance_reader();
        return form_token(TOKEN_COLVAL);
}

Token
next_token()
{
        if (is_at_start())
                return stmt_invocation();
        if (is_at_end())
                return form_token(TOKEN_END);
        if (curr_chr() == ' ' || curr_chr() == ',')
                advance_reader();
        if (iscolvalsymbol(curr_chr()))
                return colval();
        if (curr_chr() == ';') {
                start_at_curr();
                advance_reader();
                return form_token(TOKEN_SEP);
        }
        return form_token(TOKEN_ERR);
}