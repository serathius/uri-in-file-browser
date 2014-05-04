#include "file_cursor.h"

FileCursor::FileCursor(char * filename)
{
    this->file = fopen(filename, "rb");
    this->offset = 0;
}

FileCursor::FileCursor(const FileCursor& orig) 
{
    this->file = orig.file;
    this->offset = orig.offset;
}

FileCursor::~FileCursor() 
{
    fclose(this->file);
}

bool FileCursor::eof()
{
    return feof(this->file);
}

unsigned long FileCursor::get_offset() 
{
    return this->offset;
}

char * FileCursor::gets(int length) 
{
    char* str = new char(length+1);
    fgets(str, length+1, this->file);
    return str;
}

char FileCursor::get() 
{
    this->offset++;
    return getc(this->file);
}
