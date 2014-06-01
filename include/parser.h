#ifndef PARSER_H
#define	PARSER_H

#include "text_cursor.h"

void parse_file(TextCursor);

TextCursor parse_ipv6address(TextCursor);

#endif	/* PARSER_H */