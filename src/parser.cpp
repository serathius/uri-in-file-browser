#include "../include/parser.h"
#include "../include/exceptions.h"

#define IGNORE_PARSE_ERROR(FUN, ARG)\
try                                 \
{                                   \
    return FUN(ARG);                \
}                                   \
catch (ParseError)                  \
{                                   \
                                    \
}                                   \

#define REPEAT_IGNORING(FUN, ARG)   \
while(true)                         \
{                                   \
    try                             \
    {                               \
        ARG = FUN(ARG);             \
    }                               \
    catch(ParseError)               \
    {                               \
        return ARG;                 \
    }                               \
}                                   \

inline char get_char(TextCursor& cursor)
{
    char c = cursor.get();
    if (cursor.eof())
        throw ParseError();
    return c;
}

//   sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
//                 / "*" / "+" / "," / ";" / "="
TextCursor parse_sub_delims(TextCursor cursor)
{
    char c = get_char(cursor);
    if(c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' ||
        c == '*' || c == '+' || c == ',' || c == ';' || c == '=')
        return cursor;
    else
        throw ParseError();
}

//   gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
TextCursor parse_gen_delims(TextCursor cursor)
{
    char c = get_char(cursor);
    if (c == ':' || c == '/' || c == '?' || c == '#' || c == '[' || c == ']' ||
            c =='@')
        return cursor;
    else
        throw ParseError();
}

//   reserved      = gen-delims / sub-delims
TextCursor parse_reserved(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_gen_delims, cursor);
    IGNORE_PARSE_ERROR(parse_sub_delims, cursor);
    throw ParseError();
}

TextCursor parse_alpha(TextCursor cursor)
{
    char c = get_char(cursor);
    if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
        return cursor;
    else
        throw ParseError();
}

TextCursor parse_digit(TextCursor cursor)
{
    char c = get_char(cursor);
    if('0' <= c && c <= '9')
        return cursor;
    else
        throw ParseError();
}

//   unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
TextCursor parse_unreserved(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_alpha, cursor);
    IGNORE_PARSE_ERROR(parse_digit, cursor);
    char c = get_char(cursor);
    if (c == '-' || c == '.' || c == '_' || c == '~')
        return cursor;
    else
        throw ParseError();
}

// hexdigit = DIGIT / 'A' - 'F' / 'a' - 'f'
TextCursor parse_hexdigit(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_digit, cursor);
    char c = get_char(cursor);
    if(('A' <= c && c <= 'F') || ('a' <= c && c <= 'f'))
        return cursor;
    throw ParseError();
}

//   pct-encoded   = "%" HEXDIG HEXDIG
TextCursor parse_pct_encoded(TextCursor cursor)
{
    char c = get_char(cursor);
    if (c != '%')
        throw ParseError();
    parse_hexdigit(cursor);
    return parse_hexdigit(cursor);
}

//   pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
TextCursor parse_pchar(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_unreserved, cursor);
    IGNORE_PARSE_ERROR(parse_pct_encoded, cursor);
    IGNORE_PARSE_ERROR(parse_sub_delims, cursor);
    char c = get_char(cursor);
    if(c == ':' || c == '@')
        return cursor;
    else
        throw ParseError();
}

//query_fragment_element = pchar / "/" / "?"
TextCursor parse_query_fragment_element(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_pchar, cursor);
    char c = get_char(cursor);
    if(c == '/' || c == '?')
        return cursor;
    else
        throw ParseError();
}

//query         = *query_fragment_element
//fragment      = *query_fragment_element
TextCursor parse_query_fragment(TextCursor cursor)
{
    REPEAT_IGNORING(parse_query_fragment_element, cursor);
}

//   segment_nz_nc_element = unreserved / pct-encoded / sub-delims / "@" 
TextCursor parse_segment_nz_nc_element(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_unreserved, cursor);
    IGNORE_PARSE_ERROR(parse_pct_encoded, cursor);
    IGNORE_PARSE_ERROR(parse_sub_delims, cursor);
    char c = get_char(cursor);
    if (c == '@')
        return cursor;
    else
        throw ParseError();
}

// segment-nz-nc = 1*segment-nz-nc
TextCursor parse_segment_nz_nc(TextCursor cursor)
{
    cursor = parse_segment_nz_nc_element(cursor);
    REPEAT_IGNORING(parse_segment_nz_nc_element, cursor);
}

//  segment       = *pchar
TextCursor parse_segment(TextCursor cursor)
{
    REPEAT_IGNORING(parse_pchar, cursor);
}

//   segment-nz    = 1*pchar
TextCursor parse_segment_nz(TextCursor cursor)
{
    cursor = parse_pchar(cursor);
    REPEAT_IGNORING(parse_pchar, cursor);
}

//   slash_segment = '/' segment
TextCursor parse_slash_segment(TextCursor cursor)
{
    char c = get_char(cursor);
    if (c != '/')
        throw ParseError();
    else
        IGNORE_PARSE_ERROR(parse_segment, cursor);
}

//   path-abempty  = *( "/" segment )
TextCursor parse_path_abempty(TextCursor cursor)
{
    REPEAT_IGNORING(parse_slash_segment, cursor);
}

//   path-absolute = "/" [ segment-nz *( "/" segment ) ]
TextCursor parse_path_absolute(TextCursor cursor)
{
    char c = get_char(cursor);
    if (c != '/')
        throw ParseError();
    else
    {
        try
        {
            TextCursor cursor2(parse_segment_nz(cursor));
            REPEAT_IGNORING(parse_slash_segment, cursor2);
        }
        catch(ParseError)
        {
            return cursor;
        }
    }
}

//   path-noscheme = segment-nz-nc *( "/" segment )
TextCursor parse_path_noscheme(TextCursor cursor)
{
    TextCursor cursor2(parse_segment_nz_nc(cursor));
    REPEAT_IGNORING(parse_slash_segment, cursor2);
}

//   path-rootless = segment-nz *( "/" segment )
TextCursor parse_path_rootless(TextCursor cursor)
{
    TextCursor cursor2(parse_segment_nz(cursor));
    REPEAT_IGNORING(parse_slash_segment, cursor2);
}

//   path-empty    = 0<pchar>
TextCursor parse_path_empty(TextCursor cursor)
{
    return cursor;
}

//   path          = path-abempty    ; begins with "/" or is empty
//                 / path-absolute   ; begins with "/" but not "//"
//                 / path-noscheme   ; begins with a non-colon segment
//                 / path-rootless   ; begins with a segment
//                 / path-empty      ; zero characters
TextCursor parse_path(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_path_abempty, cursor);
    IGNORE_PARSE_ERROR(parse_path_absolute, cursor);
    IGNORE_PARSE_ERROR(parse_path_noscheme, cursor);
    IGNORE_PARSE_ERROR(parse_path_rootless, cursor);
    IGNORE_PARSE_ERROR(parse_path_empty, cursor);
    throw ParseError();
}
// 250_255 = "25" 0-5
TextCursor parse_250_255(TextCursor cursor)
{
    char c = get_char(cursor);
    if (c != '2')
        throw ParseError();
    c = get_char(cursor);
    if (c != '5')
        throw ParseError();
    c = get_char(cursor);
    if ('0' <= c && c <= '5')
        return cursor;
    else
        throw ParseError();
}

// 200_249 = "2" 0-4 DIGIT
TextCursor parse_200_249(TextCursor cursor)
{
    char c = get_char(cursor);
    if (c != '2')
        throw ParseError();
    c = get_char(cursor);
    if ('0' <= c && c <= '4')
    {
        return parse_digit(cursor);
    }
    else
        throw ParseError();
}

// 100_199 = "1" DIGIT DIGIT
TextCursor parse_100_199(TextCursor cursor)
{
    char c = get_char(cursor);
    if(c != '1')
        throw ParseError();
    return parse_digit(parse_digit(cursor));
}

// 10_99 = 1-9 DIGIT
TextCursor parse_10_99(TextCursor cursor)
{
    char c = get_char(cursor);
    if ('1' <= c && c <= '9')
    {
        return parse_digit(cursor);
    }
    else 
        throw ParseError();
}

//dec-octet     = DIGIT          ; 0-9
//                 / 10_99       ; 10-99
//                 / 100_199     ; 100-199
//                 / 200_249     ; 200-249
//                 / 250_255     ; 250-255
TextCursor parse_dec_octet(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_250_255, cursor);
    IGNORE_PARSE_ERROR(parse_200_249, cursor);
    IGNORE_PARSE_ERROR(parse_100_199, cursor);
    IGNORE_PARSE_ERROR(parse_10_99, cursor);
    IGNORE_PARSE_ERROR(parse_digit, cursor);
    throw ParseError();
}

//   reg_name_segment = *( unreserved / pct-encoded / sub-delims )
TextCursor parse_reg_name_segment(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_unreserved, cursor);
    IGNORE_PARSE_ERROR(parse_pct_encoded, cursor);
    IGNORE_PARSE_ERROR(parse_sub_delims, cursor);
    throw ParseError();
}

// reg_name = *(reg_name_segment)
TextCursor parse_reg_name(TextCursor cursor)
{
    REPEAT_IGNORING(parse_reg_name_segment, cursor);
}

//   IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet
TextCursor parse_ipv4address(TextCursor cursor)
{
    for (int i=0; i<3; i++)
    {
        cursor = parse_dec_octet(cursor);
        if (get_char(cursor) != '.')
            throw ParseError();
    }
    return parse_dec_octet(cursor);
}

//   h16           = 1*4HEXDIG
TextCursor parse_h16(TextCursor cursor)
{
    cursor = parse_hexdigit(cursor);
    for (int i=0; i<3; i++)
    {
        try
        {
            cursor = parse_hexdigit(cursor);
        }
        catch(ParseError)
        {
            return cursor;
        }
    }
    return cursor;
}

//   ls32          = ( h16 ":" h16 ) / IPv4address
TextCursor parse_ls32(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_ipv4address, cursor);
    cursor = parse_h16(cursor);
    if (get_char(cursor) != ':')
        throw ParseError();
    return parse_h16(cursor);
}

// h16_colon = h16 ':'
TextCursor parse_h16_colon(TextCursor cursor)
{
    cursor = parse_h16(cursor);
    if(get_char(cursor) == ':')
        return cursor;
    else
        throw ParseError();
}

TextCursor parse_colon_h16(TextCursor cursor)
{
    if(get_char(cursor) != ':')
        throw ParseError();  
    return parse_h16(cursor);
}

TextCursor parse_n_h16_colon(TextCursor cursor, int n)
{
    for (int i=0; i<n; i++)
    {
        cursor = parse_h16_colon(cursor);
    }
    return cursor;
}

TextCursor parse_lt_n_h16_colon(TextCursor cursor, int n)
{
    for (int i=0; i<n; i++)
    {
        try
        {
            cursor = parse_h16_colon(cursor);
        }
        catch(ParseError)
        {
            return cursor;
        }
    }
    return cursor;
}

// double_colon = ":" ":"
TextCursor parse_double_colon(TextCursor cursor)
{
    if (get_char(cursor) != ':')
        throw ParseError();
    else
        if (get_char(cursor) != ':')
            throw ParseError();
        else
            return cursor;
}

//   IPv6address1   =                           6( h16 ":" ) ls32
//                 /                       "::" 5( h16 ":" ) ls32
//                 / [               h16 ] "::" 4( h16 ":" ) ls32
//                 / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
//                 / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
//                 / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
//                 / [ *4( h16 ":" ) h16 ] "::"              ls32
//                 / [ *5( h16 ":" ) h16 ] "::"              h16
//                 / [ *6( h16 ":" ) h16 ] "::"
TextCursor parse_ipv6address(TextCursor cursor)
{
    try
    {
        return parse_ls32(parse_n_h16_colon(cursor, 6));
    }
    catch(ParseError)
    {
    }
    bool h16;
    try
    {
        cursor = parse_h16(cursor);
        h16 = true;
    }
    catch(ParseError)
    {
        h16 = false;
    }
    
    int n = 0;
    for (; n<7; n++)
    {
        try
        {
            cursor = parse_colon_h16(cursor);
        }
        catch(ParseError)
        { 
            break;
        }
    }
    cursor = parse_double_colon(cursor);
    try
    {
        return parse_ls32(parse_n_h16_colon(cursor, 5 - h16));
    }
    catch(ParseError)
    {

    }  
    
    if (n <= 1)
    {
        try
        {
            return parse_ls32(parse_n_h16_colon(cursor, 3));
        }
        catch(ParseError)
        {
            
        }
        
    }
    if (n <= 2)
    {
        try
        {
            return parse_ls32(parse_n_h16_colon(cursor, 2));
        }
        catch(ParseError)
        {
            
        }        
    }
    if (n <= 3)
    {
        try
        {
            return parse_ls32(parse_n_h16_colon(cursor, 1));
        }
        catch(ParseError)
        {
            
        }
    }
    if (n <= 4)
    {
        IGNORE_PARSE_ERROR(parse_ls32, cursor);
    }
    if (n <= 5)
    {
        IGNORE_PARSE_ERROR(parse_h16, cursor);
    }
    if (n <= 6)
        return cursor;
    throw ParseError();
}

TextCursor parse_unreserved_subdelims_colon(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_unreserved, cursor);
    IGNORE_PARSE_ERROR(parse_sub_delims, cursor);
    if(get_char(cursor) == ':')
        return cursor;
    else
        throw ParseError();
}

TextCursor parse_at_least_one_unreserved_subdelims_colon(TextCursor cursor)
{
    cursor = parse_unreserved_subdelims_colon(cursor);
    REPEAT_IGNORING(parse_unreserved, cursor);
}

TextCursor parse_at_least_one_hexdigit(TextCursor cursor)
{
    cursor = parse_hexdigit(cursor);
    REPEAT_IGNORING(parse_hexdigit, cursor);
}

//IPvFuture = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
TextCursor parse_ipvfuture(TextCursor cursor)
{
    if(get_char(cursor) != 'v')
        throw ParseError();
    cursor = parse_at_least_one_hexdigit(cursor);
    if(get_char(cursor) != '.')
        throw ParseError();
    return parse_at_least_one_unreserved_subdelims_colon(cursor);    
}

//ipv6address_ipfuture =  IPv6address / IPvFuture  
TextCursor parse_ipv6address_ipfuture(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_ipv6address, cursor);
    IGNORE_PARSE_ERROR(parse_ipvfuture, cursor);
    throw ParseError();
}

//   IP-literal    = "[" ipv6address_ipfuture "]"
TextCursor parse_ip_literal(TextCursor cursor)
{
    if(get_char(cursor) != '[')
        throw ParseError();
    cursor = parse_ipv6address_ipfuture(cursor);
    if(get_char(cursor) == ']')
        return cursor;
    else
        throw ParseError();
}

//port          = *DIGIT
TextCursor parse_port(TextCursor cursor)
{
    REPEAT_IGNORING(parse_digit, cursor);
}

//host = IP-literal / IPv4address / reg-name
TextCursor parse_host(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_ip_literal, cursor);
    IGNORE_PARSE_ERROR(parse_ipv4address, cursor);
    IGNORE_PARSE_ERROR(parse_reg_name, cursor);
    throw ParseError();
}

//userinfo_segment = unreserved / pct-encoded / sub-delims / ":" 
TextCursor parse_userinfo_segment(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_unreserved, cursor);
    IGNORE_PARSE_ERROR(parse_pct_encoded, cursor);
    IGNORE_PARSE_ERROR(parse_sub_delims, cursor);
    if(get_char(cursor) == ':')
        return cursor;
    else
        throw ParseError();
}

//userinfo      = *user_info_segment
TextCursor parse_userinfo(TextCursor cursor)
{
    REPEAT_IGNORING(parse_userinfo_segment, cursor);
}

//authority     = [ userinfo "@" ] host [ ":" port ]
TextCursor parse_authority(TextCursor cursor)
{
    try
    {
        TextCursor cursor_copy(parse_userinfo(cursor));
        if(get_char(cursor_copy) != '@')
            throw ParseError();
        cursor = cursor_copy;
    }
    catch(ParseError)
    {
    }
    cursor = parse_host(cursor);
    try
    {
        TextCursor cursor_copy2(cursor);
        if(get_char(cursor_copy2) != ':')
            throw ParseError();
        cursor = parse_port(cursor_copy2);
    }
    catch(ParseError)
    {
    }
    return cursor;
}

//parse_hier_part_case = "//" authority path-abempty
TextCursor parse_hier_part_case(TextCursor cursor)
{
    if (get_char(cursor) != '/')
        throw ParseError();
    if (get_char(cursor) != '/')
        throw ParseError();
    cursor = parse_authority(cursor);
    return parse_path_abempty(cursor);
}

//hier-part     = hier_part_case
//              / path-absolute
//              / path-rootless
//              / path-empty
TextCursor parse_hier_part(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_hier_part_case, cursor);
    IGNORE_PARSE_ERROR(parse_path_absolute, cursor);
    IGNORE_PARSE_ERROR(parse_path_rootless, cursor);
    IGNORE_PARSE_ERROR(parse_path_empty, cursor);
    throw ParseError();
}

// scheme_element =  ALPHA / DIGIT / "+" / "-" / "." 
TextCursor parse_scheme_element(TextCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_alpha, cursor);
    IGNORE_PARSE_ERROR(parse_digit, cursor);
    char c = get_char(cursor);
    if (c == '+' || c == '-' || c == '.')
        return cursor;
    else
        throw ParseError();
}

//scheme        = ALPHA *scheme_element
TextCursor parse_scheme(TextCursor cursor)
{
    cursor = parse_alpha(cursor);
    REPEAT_IGNORING(parse_scheme_element, cursor);
}

//uri = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
TextCursor parse_uri(TextCursor cursor)
{
    cursor = parse_scheme(cursor);
    if(get_char(cursor) != ':')
        throw ParseError();
    cursor = parse_hier_part(cursor);
    try
    {
        TextCursor cursor_copy(cursor);
        if (get_char(cursor_copy) != '?')
            throw ParseError();
        cursor = parse_query_fragment(cursor_copy);
    }
    catch(ParseError)
    {
    }
    try
    {
        TextCursor cursor_copy2(cursor);
        if (get_char(cursor_copy2) != '#')
            throw ParseError();
        cursor = parse_query_fragment(cursor);
    }
    catch(ParseError)
    {
    }
    return cursor;
}

void save_uri(int n, const char * uri, int offset)
{
    char file_name[4];
    sprintf(file_name, "%04d", n);
    FILE * file = fopen(file_name, "w");
    fprintf(file, "%s\n%d", uri, offset);
    fclose(file);
}

void parse_file(TextCursor cursor)
{
    int uri_length, i = 0, pos;
    while(!cursor.eof())
    {
        try
        {
           TextCursor stop_cursor(parse_uri(cursor));
           uri_length = stop_cursor.get_offset() - cursor.get_offset();
           char uri[uri_length + 1];
           pos = cursor.get_offset();
           cursor.gets(uri_length, uri);
           save_uri(i++, uri, pos);
           printf("%4d %s\n", pos, uri);
           cursor = stop_cursor;
        }
        catch(ParseError)
        {
            cursor.get();
        }
    }
}
