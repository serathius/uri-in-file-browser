#include "../include/text_cursor.h"
#include "../include/exceptions.h"

FileAccess::FileAccess(const char * file_name) : Access()
{
    this->file = fopen(file_name, "r");
    if (this->file == nullptr)
    {
        throw FileNotExistsError();
    }
    this->offset = 0;
}

FileAccess::~FileAccess()
{
    fclose(this->file);
}

void FileAccess::checkout(unsigned long offset)
{
    if(offset != this->offset)
    {
        fseek(this->file, offset, SEEK_SET);
        this->offset = offset;
    }
}

bool FileAccess::eof(unsigned long offset)
{
    checkout(offset);
    return feof(this->file);
}

char FileAccess::get(unsigned long offset)
{
    checkout(offset);
    this->offset++;
    return getc(this->file);
}

void FileAccess::gets(unsigned long offset, int length, char * str)
{
    checkout(offset);
    this->offset += length;
    fgets(str, length+1, this->file);
}

TextCursor::TextCursor(Access * file)
{
    this->file = std::shared_ptr<Access>(file);
    this->offset = 0;
}

TextCursor::TextCursor(const TextCursor& orig) 
{
    this->file = std::move(orig.file);
    this->offset = orig.offset;
}

TextCursor::~TextCursor() 
{
   
}

bool TextCursor::eof()
{
    return this->file.get()->eof(this->offset);
}

unsigned long TextCursor::get_offset() 
{
    return this->offset;
}

void TextCursor::gets(int length, char * str) 
{
    this->file.get()->gets(this->offset, length, str);
    this->offset += length;
}

char TextCursor::get() 
{
    return this->file.get()->get(this->offset++);
}
