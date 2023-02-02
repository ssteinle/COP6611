#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void argCheck(int argNum, char *args[])
{
    // int sizeSearch = 0;

    if (argNum < 4)
    {
        printf(1, "Not enough arguments. Minimum \"find <folder> -name <name>\"\n");
        exit();
    }
    
    if (argNum > 6)
    {
        printf(1, "Too many arguments. Maximum \"find <folder> -name <name> <flag> <flag type>\"\n");
        exit();
    }
    
    if (strcmp(args[2], "-name") != 0)
    {
        printf(1, "Improper argument. Must begin with \"find <folder> -name <name>\"\n");
        exit();
    }

    if (argNum == 6)
    {
        if (((strcmp(args[4], "-type") == 0) && !((strcmp(args[5], "f") == 0) || (strcmp(args[5], "d") == 0))))
        {
            printf(1, "Improper flag. \"-type\' must be followed by \"f\" or \"d\"\n");
            exit();        
        }

    //     if (strcmp(args[4], "-size") == 0)
    //     {
    //         int myLen = sizeof(args[5]) / sizeof(char *);

    //         printf("%d", &myLen);
            
    //         if (((args[5])[0] == '-') || ((args[5])[0] == '+'))
    //         {
    //             sizeSearch = 1 + myLen;
    //             printf(1, "catnip!\n");
    //         }

    //         printf("%d", sizeSearch);
    //     }
    }

}

/*
psudo find(current directory, file, flags)
{
    if current depth > max depth
        return

    for all files and dirctoreies in current directory
        if filefound == file searched
            print current directory path + file name
        if a directory is found
            find (new directory, file name, flags)
}

*/

void find(char *path)
{
    char buffer[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

   fd = open(path, 0);

    // fstat(fd, &st);


    strcpy(buffer, path);

    p = buffer + strlen(buffer);

    printf(1, p);

    *p++ = '/';



    while(read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if(de.inum == 0)
        {
            continue;
        }


        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        printf(1, de.name);

        // printf(1, p);


        if(stat(buffer, &st) < 0)
        {
            // printf(1, "bop");
            continue;
        }
    }
}


int main(int argc, char *argv[])
{

    argCheck(argc, argv);

    char *rootDir = argv[1];

    // char *searchTerm = argv[3];

    // printf(1, rootDir);
    // printf(1, searchTerm);

    find(rootDir);

    exit();
}