#include "../include/file_cursor.h"

FileCursor::FileCursor(const char * filename)
{
    FILE * file = fopen(filename, "r");
    this->file = std::shared_ptr<FILE>(file, fclose);
    this->offset = 0;
    this->_eof = false;
}

FileCursor::FileCursor(const FileCursor& orig) 
{
    this->file = std::move(orig.file);
    this->offset = orig.offset;
    this->_eof = orig._eof;
}

FileCursor::~FileCursor() 
{
   
}

bool FileCursor::eof()
{
    return this->_eof;
}

unsigned long FileCursor::get_offset() 
{
    return this->offset;
}

char * FileCursor::gets(int length) 
{
    char* str = new char(length+1);
    fseek(this->file.get(), this->offset, SEEK_SET);
    this->offset += length;
    fgets(str, length+1, this->file.get());
    this->_eof = feof(this->file.get());
    return str;
}

char FileCursor::get() 
{
    char c;
    fseek(this->file.get(), this->offset++, SEEK_SET);
    c = getc(this->file.get());
    this->_eof = feof(this->file.get());
    return c;
}
