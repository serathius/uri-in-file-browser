#include "../include/file_cursor.h"
#include "../include/exceptions.h"

File::File(const char * file_name)
{
    this->file = fopen(file_name, "r");
    if (this->file == nullptr)
    {
        throw FileNotExistsError();
    }
    this->offset = 0;
}

File::~File()
{
    fclose(this->file);
}

void File::checkout(unsigned long offset)
{
    if(offset != this->offset)
    {
        fseek(this->file, offset, SEEK_SET);
        this->offset = offset;
    }
}

bool File::eof(unsigned long offset)
{
    checkout(offset);
    return feof(this->file);
}

char File::get(unsigned long offset)
{
    checkout(offset);
    this->offset++;
    return getc(this->file);
}

void File::gets(unsigned long offset, int length, char * str)
{
    checkout(offset);
    this->offset += length;
    fgets(str, length+1, this->file);
}

FileCursor::FileCursor(File * file)
{
    this->file = std::shared_ptr<File>(file);
    this->offset = 0;
}

FileCursor::FileCursor(const FileCursor& orig) 
{
    this->file = std::move(orig.file);
    this->offset = orig.offset;
}

FileCursor::~FileCursor() 
{
   
}

bool FileCursor::eof()
{
    return this->file.get()->eof(this->offset);
}

unsigned long FileCursor::get_offset() 
{
    return this->offset;
}

void FileCursor::gets(int length, char * str) 
{
    this->file.get()->gets(this->offset, length, str);
    this->offset += length;
}

char FileCursor::get() 
{
    return this->file.get()->get(this->offset++);
}
