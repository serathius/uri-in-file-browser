#include "../include/parser.h"
#include "../include/exceptions.h"

#define IGNORE_PARSE_ERROR(X) \
try                     \
{                       \
    return X(cursor);   \
}                       \
catch (ParseError e)    \
{                       \
                        \
}                       \
inline char get_char(FileCursor& cursor)
{
    char c = cursor.get();
    if (cursor.eof())
        throw ParseError();
    return c;
}

//   sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
//                 / "*" / "+" / "," / ";" / "="
FileCursor parse_sub_delims(FileCursor cursor)
{
    char c = get_char(cursor);
    if(c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' ||
        c == '*' || c == '+' || c == ',' || c == ';' || c == '=')
        return cursor;
    else
        throw ParseError(); 
}

//   gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
FileCursor parse_gen_delims(FileCursor cursor)
{
    char c = get_char(cursor);
    if (c == ':' || c == '/' || c == '?' || c == '#' || c == '[' || c == ']' ||
            c =='@')
        return cursor;
    else
        throw ParseError();
}

//   reserved      = gen-delims / sub-delims
FileCursor parse_reserved(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_gen_delims)
    IGNORE_PARSE_ERROR(parse_sub_delims)
    throw ParseError();
}

FileCursor parse_alpha(FileCursor cursor)
{
    char c = get_char(cursor);
    if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
        return cursor;
    else
        throw ParseError();
}

FileCursor parse_digit(FileCursor cursor)
{
    char c = get_char(cursor);
    if('0' <= c && c <= '9')
        return cursor;
    else
        throw ParseError();
}

//   unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
FileCursor parse_unreserved(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_alpha)
    IGNORE_PARSE_ERROR(parse_digit)
    char c = get_char(cursor);
    if (c == '-' || c == '.' || c == '_' || c == '~')
        return cursor;
    else
        throw ParseError();
}

FileCursor parse_hexdigit(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_digit)
    char c = get_char(cursor);
    if('A' <= c && c <= 'F')
        return cursor;
    throw ParseError();
}

//   pct-encoded   = "%" HEXDIG HEXDIG
FileCursor parse_pct_encoded(FileCursor cursor)
{
    char c = get_char(cursor);
    if (c != '%')
        throw ParseError();
    parse_hexdigit(cursor);
    return parse_hexdigit(cursor);
}

//   pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
FileCursor parse_pchar(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_unreserved)
    IGNORE_PARSE_ERROR(parse_pct_encoded)
    IGNORE_PARSE_ERROR(parse_sub_delims)
    char c = get_char(cursor);
    if(c == ':' || c == '@')
        return cursor;
    else
        throw ParseError();
}

