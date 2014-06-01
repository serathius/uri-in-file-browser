#ifndef EXCEPTIONS_H
#define	EXCEPTIONS_H

#include <exception>

class ParseError : public std::exception
{
    
};

class FileNotExistsError : public std::exception
{
    
};

#endif	/* EXCEPTIONS_H */

