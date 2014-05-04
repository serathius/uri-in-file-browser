#ifndef FILE_CURSOR_H
#define	FILE_CURSOR_H

#include <cstdio>
#include <memory>

class FileCursor 
{
private:
    std::shared_ptr<FILE> file;
    unsigned long offset;
    
public:
    FileCursor(char * filename);
    FileCursor(const FileCursor& orig);
    virtual ~FileCursor();
    
    virtual bool eof();
    virtual unsigned long get_offset();
    virtual char * gets(int length);
    virtual char get();
};

#endif
