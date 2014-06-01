#include "../include/parser.h"
#include "../include/exceptions.h"

int main(int argc, const char** argv)
{
    if(argc != 2)
    {
        printf("Arguments: file name.\n");
        exit(1);
    }
    else
    {
        try
        {
           File * file = new File(argv[1]); 
           FileCursor cursor(file);
           parse_file(cursor);
        }
        catch(FileNotExistsError)
        {
            printf("File not exists.\n");
            exit(2);
        }
    }
}
