#ifndef FILE_CURSOR_H
#define	FILE_CURSOR_H

#include <cstdio>
#include <memory>

class File
{
    FILE * file;
    unsigned long offset;
    inline void checkout(unsigned long offset);
    
public:
    File(const char*);
    virtual ~File();
    virtual bool eof(unsigned long);
    virtual char get(unsigned long);
    virtual void gets(unsigned long, int, char *);
};

class TextCursor 
{
    std::shared_ptr<File> file;
    unsigned long offset;
    
public:
    TextCursor(File * file);
    TextCursor(const TextCursor& orig);
    virtual ~TextCursor();
    
    virtual bool eof();
    virtual unsigned long get_offset();
    virtual void gets(int length, char * str);
    virtual char get();
};

#endif
