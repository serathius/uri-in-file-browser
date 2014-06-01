#ifndef FILE_CURSOR_H
#define	FILE_CURSOR_H

#include <cstdio>
#include <memory>

class Access
{
public:
    virtual bool eof(unsigned long) = 0;
    virtual char get(unsigned long) = 0;
    virtual void gets(unsigned long, int, char *) = 0;
};

class FileAccess : public Access
{
    FILE * file;
    unsigned long offset;
    
    inline void checkout(unsigned long offset);

public:
    FileAccess(const char*);
    virtual ~FileAccess();
    virtual bool eof(unsigned long);
    virtual char get(unsigned long);
    virtual void gets(unsigned long, int, char *);
};

class TextCursor 
{
    std::shared_ptr<Access> file;
    unsigned long offset;
    
public:
    TextCursor(Access * file);
    TextCursor(const TextCursor& orig);
    virtual ~TextCursor();
    
    virtual bool eof();
    virtual unsigned long get_offset();
    virtual void gets(int length, char * str);
    virtual char get();
};

#endif
