#ifndef PARSER_H
#define	PARSER_H

#include "file_cursor.h"

FileCursor parse_file(FileCursor cursor);

FileCursor parse_uri(FileCursor cursor);

#endif	/* PARSER_H */

