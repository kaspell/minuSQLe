#include "tokenizer.h"


Reader *reader = &main_reader;


void
advance_reader()
{
        ++reader->curr;
}

bool
is_at_end()
{
        return (*reader->curr == '\0');
}

bool
is_at_start()
{
        return (reader->curr == reader->start);
}

bool
isalph(char c)
{
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool
isnum(char c)
{
        return (c >= '0' && c <= '9');
}

bool
isalphnum(char c)
{
        return isalph(c) || isnum(c);
}

void
situate_reader(const char *s)
{
        reader->start = s;
        reader->curr = s;
}

void
start_at_curr()
{
        reader->start = reader->curr;
}

char
curr_chr()
{
        return *reader->curr;
}

char
next_chr()
{
        if (is_at_end())
                return '\0';
        return *(reader->curr + 1);
}

Token
form_token(TokenType type)
{
        Token token;
        token.start = reader->start;
        token.type = type;
        token.length = (int)(reader->curr - reader->start);
        return token;
}

Token
stmt_invocation()
{
        start_at_curr();
        while (isalph(curr_chr()))
                advance_reader();
        if (!strncmp(reader->start, "select", strlen("select")) || !strncmp(reader->start, "SELECT", strlen("SELECT")))
                return form_token(TOKEN_SELECT);
        if (!strncmp(reader->start, "insert", strlen("insert")) || !strncmp(reader->start, "INSERT", strlen("INSERT")))
                return form_token(TOKEN_INSERT);
        return form_token(TOKEN_ERR);
}

Token
colval()
{
        start_at_curr();
        while (isalphnum(curr_chr()))
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
        if (isalphnum(curr_chr()))
                return colval();
        if (curr_chr() == ';') {
                start_at_curr();
                advance_reader();
                return form_token(TOKEN_SEP);
        }
        return form_token(TOKEN_ERR);
}