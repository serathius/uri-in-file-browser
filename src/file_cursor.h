#ifndef FILE_CURSOR_H
#define	FILE_CURSOR_H

#include <stdio.h>

class FileCursor 
{
private:
    FILE * file;
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
