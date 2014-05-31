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
    IGNORE_PARSE_ERROR(parse_gen_delims, cursor);
    IGNORE_PARSE_ERROR(parse_sub_delims, cursor);
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
    IGNORE_PARSE_ERROR(parse_alpha, cursor);
    IGNORE_PARSE_ERROR(parse_digit, cursor);
    char c = get_char(cursor);
    if (c == '-' || c == '.' || c == '_' || c == '~')
        return cursor;
    else
        throw ParseError();
}

// hexdigit = DIGIT / 'A' - 'F' / 'a' - 'f'
FileCursor parse_hexdigit(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_digit, cursor);
    char c = get_char(cursor);
    if(('A' <= c && c <= 'F') || ('a' <= c && c <= 'f'))
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
FileCursor parse_query_fragment_element(FileCursor cursor)
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
FileCursor parse_query_fragment(FileCursor cursor)
{
    REPEAT_IGNORING(parse_query_fragment_element, cursor);
}

//   segment_nz_nc_element = unreserved / pct-encoded / sub-delims / "@" 
FileCursor parse_segment_nz_nc_element(FileCursor cursor)
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
FileCursor parse_segment_nz_nc(FileCursor cursor)
{
    cursor = parse_segment_nz_nc_element(cursor);
    REPEAT_IGNORING(parse_segment_nz_nc_element, cursor);
}

//  segment       = *pchar
FileCursor parse_segment(FileCursor cursor)
{
    REPEAT_IGNORING(parse_pchar, cursor);
}

//   segment-nz    = 1*pchar
FileCursor parse_segment_nz(FileCursor cursor)
{
    cursor = parse_pchar(cursor);
    REPEAT_IGNORING(parse_pchar, cursor);
}

//   slash_segment = '/' segment
FileCursor parse_slash_segment(FileCursor cursor)
{
    char c = get_char(cursor);
    if (c != '/')
        throw ParseError();
    else
        IGNORE_PARSE_ERROR(parse_segment, cursor);
}

//   path-abempty  = *( "/" segment )
FileCursor parse_path_abempty(FileCursor cursor)
{
    REPEAT_IGNORING(parse_slash_segment, cursor);
}

//   path-absolute = "/" [ segment-nz *( "/" segment ) ]
FileCursor parse_path_absolute(FileCursor cursor)
{
    char c = get_char(cursor);
    if (c != '/')
        throw ParseError();
    else
    {
        try
        {
            FileCursor cursor2(parse_segment_nz(cursor));
            REPEAT_IGNORING(parse_slash_segment, cursor2);
        }
        catch(ParseError)
        {
            return cursor;
        }
    }
}

//   path-noscheme = segment-nz-nc *( "/" segment )
FileCursor parse_path_noscheme(FileCursor cursor)
{
    FileCursor cursor2(parse_segment_nz_nc(cursor));
    REPEAT_IGNORING(parse_slash_segment, cursor2);
}

//   path-rootless = segment-nz *( "/" segment )
FileCursor parse_path_rootless(FileCursor cursor)
{
    FileCursor cursor2(parse_segment_nz(cursor));
    REPEAT_IGNORING(parse_slash_segment, cursor2);
}

//   path-empty    = 0<pchar>
FileCursor parse_path_empty(FileCursor cursor)
{
    return cursor;
}

//   path          = path-abempty    ; begins with "/" or is empty
//                 / path-absolute   ; begins with "/" but not "//"
//                 / path-noscheme   ; begins with a non-colon segment
//                 / path-rootless   ; begins with a segment
//                 / path-empty      ; zero characters
FileCursor parse_path(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_path_abempty, cursor);
    IGNORE_PARSE_ERROR(parse_path_absolute, cursor);
    IGNORE_PARSE_ERROR(parse_path_noscheme, cursor);
    IGNORE_PARSE_ERROR(parse_path_rootless, cursor);
    IGNORE_PARSE_ERROR(parse_path_empty, cursor);
    throw ParseError();
}
// 250_255 = "25" 0-5
FileCursor parse_250_255(FileCursor cursor)
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
FileCursor parse_200_249(FileCursor cursor)
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
FileCursor parse_100_199(FileCursor cursor)
{
    char c = get_char(cursor);
    if(c != '1')
        throw ParseError();
    return parse_digit(parse_digit(cursor));
}

// 10_99 = 1-9 DIGIT
FileCursor parse_10_99(FileCursor cursor)
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
FileCursor parse_dec_octet(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_250_255, cursor);
    IGNORE_PARSE_ERROR(parse_200_249, cursor);
    IGNORE_PARSE_ERROR(parse_100_199, cursor);
    IGNORE_PARSE_ERROR(parse_10_99, cursor);
    IGNORE_PARSE_ERROR(parse_digit, cursor);
    throw ParseError();
}

//   reg_name_segment = *( unreserved / pct-encoded / sub-delims )
FileCursor parse_reg_name_segment(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_unreserved, cursor);
    IGNORE_PARSE_ERROR(parse_pct_encoded, cursor);
    IGNORE_PARSE_ERROR(parse_sub_delims, cursor);
    throw ParseError();
}

// reg_name = *(reg_name_segment)
FileCursor parse_reg_name(FileCursor cursor)
{
    REPEAT_IGNORING(parse_reg_name_segment, cursor);
}

//   IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet
FileCursor parse_ipv4address(FileCursor cursor)
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
FileCursor parse_h16(FileCursor cursor)
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
FileCursor parse_ls32(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_ipv4address, cursor);
    cursor = parse_h16(cursor);
    if (get_char(cursor) != ':')
        throw ParseError();
    return parse_h16(cursor);
}

// h16_colon = h16 ':'
FileCursor parse_h16_colon(FileCursor cursor)
{
    cursor = parse_h16(cursor);
    if(get_char(cursor) == ':')
        return cursor;
    else
        throw ParseError();
}

FileCursor parse_colon_h16(FileCursor cursor)
{
    if(get_char(cursor) != ':')
        throw ParseError();  
    return parse_h16(cursor);
}

FileCursor parse_n_h16_colon(FileCursor cursor, int n)
{
    for (int i=0; i<n; i++)
    {
        cursor = parse_h16_colon(cursor);
    }
    return cursor;
}

FileCursor parse_lt_n_h16_colon(FileCursor cursor, int n)
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
FileCursor parse_double_colon(FileCursor cursor)
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
FileCursor parse_ipv6address(FileCursor cursor)
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

FileCursor parse_unreserved_subdelims_colon(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_unreserved, cursor);
    IGNORE_PARSE_ERROR(parse_sub_delims, cursor);
    if(get_char(cursor) == ':')
        return cursor;
    else
        throw ParseError();
}

FileCursor parse_at_least_one_unreserved_subdelims_colon(FileCursor cursor)
{
    cursor = parse_unreserved_subdelims_colon(cursor);
    REPEAT_IGNORING(parse_unreserved, cursor);
}

FileCursor parse_at_least_one_hexdigit(FileCursor cursor)
{
    cursor = parse_hexdigit(cursor);
    REPEAT_IGNORING(parse_hexdigit, cursor);
}

//IPvFuture = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
FileCursor parse_ipvfuture(FileCursor cursor)
{
    if(get_char(cursor) != 'v')
        throw ParseError();
    cursor = parse_at_least_one_hexdigit(cursor);
    if(get_char(cursor) != '.')
        throw ParseError();
    return parse_at_least_one_unreserved_subdelims_colon(cursor);    
}

//ipv6address_ipfuture =  IPv6address / IPvFuture  
FileCursor parse_ipv6address_ipfuture(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_ipv6address, cursor);
    IGNORE_PARSE_ERROR(parse_ipvfuture, cursor);
    throw ParseError();
}

//   IP-literal    = "[" ipv6address_ipfuture "]"
FileCursor parse_ip_literal(FileCursor cursor)
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
FileCursor parse_port(FileCursor cursor)
{
    REPEAT_IGNORING(parse_digit, cursor);
}

//host = IP-literal / IPv4address / reg-name
FileCursor parse_host(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_ip_literal, cursor);
    IGNORE_PARSE_ERROR(parse_ipv4address, cursor);
    IGNORE_PARSE_ERROR(parse_reg_name, cursor);
    throw ParseError();
}

//userinfo_segment = unreserved / pct-encoded / sub-delims / ":" 
FileCursor parse_userinfo_segment(FileCursor cursor)
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
FileCursor parse_userinfo(FileCursor cursor)
{
    REPEAT_IGNORING(parse_userinfo_segment, cursor);
}

//authority     = [ userinfo "@" ] host [ ":" port ]
FileCursor parse_authority(FileCursor cursor)
{
    try
    {
        FileCursor copy(parse_userinfo(cursor));
        if(get_char(copy) != '@')
            throw ParseError();
        cursor = copy;
    }
    catch(ParseError)
    {
    }
    cursor = parse_host(cursor);
    try
    {
        FileCursor copy2(cursor);
        if(get_char(copy2) != ':')
            throw ParseError();
        cursor = parse_port(copy2);
    }
    catch(ParseError)
    {
    }
    return cursor;
}

//parse_hier_part_case = "//" authority path-abempty
FileCursor parse_hier_part_case(FileCursor cursor)
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
FileCursor parse_hier_part(FileCursor cursor)
{
    IGNORE_PARSE_ERROR(parse_hier_part_case, cursor);
    IGNORE_PARSE_ERROR(parse_path_absolute, cursor);
    IGNORE_PARSE_ERROR(parse_path_rootless, cursor);
    IGNORE_PARSE_ERROR(parse_path_empty, cursor);
    throw ParseError();
}

// scheme_element =  ALPHA / DIGIT / "+" / "-" / "." 
FileCursor parse_scheme_element(FileCursor cursor)
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
FileCursor parse_scheme(FileCursor cursor)
{
    cursor = parse_alpha(cursor);
    REPEAT_IGNORING(parse_scheme_element, cursor);
}

//uri = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
FileCursor parse_uri(FileCursor cursor)
{
    cursor = parse_scheme(cursor);
    if(get_char(cursor) != ':')
        throw ParseError();
    cursor = parse_hier_part(cursor);
    try
    {
        FileCursor copy(cursor);
        if (get_char(copy) != '?')
            throw ParseError();
        cursor = parse_query_fragment(copy);
    }
    catch(ParseError)
    {
    }
    try
    {
        FileCursor copy2(cursor);
        if (get_char(copy2) != '#')
            throw ParseError();
        cursor = parse_query_fragment(cursor);
    }
    catch(ParseError)
    {
    }
    return cursor;
}

FileCursor parse_file(FileCursor cursor)
{
    int uri_length;
    while(!cursor.eof())
    {
        try
        {
           FileCursor stop_cursor(parse_uri(cursor));
           uri_length = stop_cursor.get_offset() - cursor.get_offset();
           char uri[uri_length + 1];
           cursor.gets(uri_length, uri);
           printf("%s\n", uri);
           cursor = stop_cursor;
        }
        catch(ParseError)
        {
            cursor.get();
        }
    }
}
