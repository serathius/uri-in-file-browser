#include "../include/file_cursor.h"

FileCursor::FileCursor(char * filename)
{
    FILE * file = fopen(filename, "rb");
    this->file = std::shared_ptr<FILE>(file, fclose);
    this->offset = 0;
}

FileCursor::FileCursor(const FileCursor& orig) 
{
    this->file = orig.file;
    this->offset = orig.offset;
}

FileCursor::~FileCursor() 
{
   
}

bool FileCursor::eof()
{
    return feof(this->file.get());
}

unsigned long FileCursor::get_offset() 
{
    return this->offset;
}

char * FileCursor::gets(int length) 
{
    char* str = new char(length+1);
    fgets(str, length+1, this->file.get());
    return str;
}

char FileCursor::get() 
{
    this->offset++;
    return getc(this->file.get());
}
