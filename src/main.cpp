#include "../include/parser.h"
#include "../include/exceptions.h"

static const std::string MIN_LENGTH0 = "--minlength";
static const std::string MIN_LENGTH1 = "-m";
static const std::string SCHEMAS0 = "--schemas";
static const std::string SCHEMAS1 = "-s";

static const std::set<std::string> flags = {
    MIN_LENGTH0,
    MIN_LENGTH1,
    SCHEMAS0,
    SCHEMAS1
};

int main(int argc, const char** argv)
{
    if(argc < 2)
    {
        printf("Arguments: file name [flags].\n");
        printf("Flags --minlength n\n");
        printf("      --schemas schema1 schema2...\n");
        return 1;
    }
    else
    {
        try
        {
           int min_length = 0;
           std::set<std::string> schemas;
           for(int a=2; a<argc; a++)
           {
               if(argv[a] == MIN_LENGTH0 || argv[a] == MIN_LENGTH1)
               {
                   a++;
                   min_length = atoi(argv[a]);
               }
               else if(argv[a] == SCHEMAS0 || argv[a] == SCHEMAS1)
               {
                   a++;
                   for(; a < argc; a++)
                   {
                       if(flags.count(argv[a])) 
                       {
                           a--;
                           break;
                       }
                       schemas.insert(argv[a]);
                   }
               }
           }
           FileAccess * file = new FileAccess(argv[1]); 
           TextCursor cursor(file);
           parse_file(cursor, min_length, schemas);
        }
        catch(FileNotExistsError)
        {
            printf("File not exists.\n");
            return 2;
        }
    }
    return 0;
}
