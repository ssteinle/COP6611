#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

// Copied from our ls command
// Performs name formatting and some overflow checking
char* fmtname(char *path, int type)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;

    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;

    memmove(buf, p, strlen(p));

    if (type==1){
        memset(buf+strlen(p), '/', 1*sizeof(char));
        memset(buf+strlen(p)+1, ' ', DIRSIZ-strlen(p));
    }
    else {
        memset(buf+strlen(p), ' ', 1*sizeof(char));
        memset(buf+strlen(p)+1, ' ', DIRSIZ-strlen(p));
    }
    return buf;
}

// Heart of the find command
// Finds files and directories modified by several possible flags
void find(char *path, char *searchTerm, char* flag, char* flagArg)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        printf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        printf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    strcpy(buf, path);

    p = buf+strlen(buf);
    *p++ = '/';

    // Reads current directory and iterates through objects inside
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if(stat(buf, &st) < 0){
            printf(1, "find: cannot stat %s\n", buf);
            continue;
        }

        // Checks the passed flag and argument, then prints objects that match the requirements
        if ((fmtname(buf,st.type)[0] != 46 || st.ino == 1) && ((strcmp(p, searchTerm) == 0) || (strcmp("*", searchTerm) == 0))){
            if((strcmp(flag, "*") == 0) && (strcmp(flagArg, "*") == 0))
            {
                printf(1, "%s/%s\n", path, fmtname(buf,st.type));
            }
            else if(strcmp(flag, "-type") == 0)
            {
                if(strcmp(flagArg, "f") == 0)
                {
                    if(st.type == 2)
                        printf(1, "%s/%s\n", path, fmtname(buf,st.type));                    
                }
                else if(strcmp(flagArg, "d") == 0)
                {
                    if(st.type == 1)
                        printf(1, "%s/%s\n", path, fmtname(buf,st.type));
                }   
                else
                {
                    printf(1, "Error: Unknown flag argument");
                    exit();
                }
            }
            else if((strcmp(flag, "-size") == 0))
            {
                int argHook = 0;
                char * sizeNum;

                // Interprets the size argument
                if((flagArg[0] == '-'))
                {
                    argHook = 1;
                    sizeNum = flagArg+1;
                }
                else if((flagArg[0] == '+'))
                {
                    argHook = 2;
                    sizeNum = flagArg+1;
                }
                else
                {
                    sizeNum = flagArg;
                }

                // Checks to make sure the size argument is actually an integer
                for(int i=0; i<(strlen(sizeNum)); i++)
                {
                    if((sizeNum[i] < '0') || (sizeNum[i] > '9')) 
                    {
                        printf(1, "%c in %s is not an int!\n", sizeNum[i], sizeNum);
                        exit();
                    }
                }

                // Prints out objects that meet the size requirement
                if((argHook == 0) && (atoi(sizeNum) == st.size))
                {
                    printf(1, "%s/%s\n", path, fmtname(buf,st.type));                                                        
                }
                else if((argHook == 1) && (atoi(sizeNum) > st.size))
                {
                    printf(1, "%s/%s\n", path, fmtname(buf,st.type));                                                        
                }
                else if((argHook == 2) && (atoi(sizeNum) < st.size))
                {
                    printf(1, "%s/%s\n", path, fmtname(buf,st.type));                                                        
                }
            }
            else
            {
                printf(1, "Error: Invalid Flag or Flag Argument\n");
                exit();
            }
        }

        // Checks if the object is a new directory and creates a new path for a recursive find operation to investigate
        if((st.type == 1) && (strcmp(p, ".") != 0) && (strcmp(p, "..") != 0))
        {
            int lenDir = strlen(p);
            int lenPath = strlen(path);

            char dest[lenDir + lenPath + 1];

            for(int i=0; i<lenPath; i++)
            {
                dest[i] = path[i];
            }

            dest[lenPath] = '/';

            // Appends the directory with the path
            for(int i=0; i<lenDir; i++)
            {
                dest[lenPath + 1 + i] = p[i];
            }

            dest[lenPath + 1 + lenDir] = '\0';      

            // Searches the found directory
            find(dest, searchTerm, flag, flagArg);
        }
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    char * rootDir;
    char * searchTerm;
    char * flag;
    char * flagArg;

    // Accounts for various combinations of arguments
    if(argc == 6)
    {
        rootDir = argv[1];

        searchTerm = argv[3];

        flag = argv[4];

        flagArg = argv[5];
    }
    if(argc == 5)
    {
        rootDir = ".";

        searchTerm = argv[2];

        flag = argv[3];

        flagArg = argv[4];
    }
    else if(argc == 4)
    {
        rootDir = argv[1];

        if(strcmp(argv[2], "-name") == 0)
        {
            searchTerm = argv[3];

            flag = "*";

            flagArg = "*";
        }
        else if((strcmp(argv[2], "-type") == 0) || (strcmp(argv[2], "-size") == 0))
        {
            searchTerm = "*";

            flag = argv[2];

            flagArg = argv[3];
        }
        else
        {
            exit();
        }
    }
    else if(argc == 3)
    {
        rootDir = ".";

        if(strcmp(argv[1], "-name") == 0)
        {
            searchTerm = argv[2];

            flag = "*";

            flagArg = "*";
        }
        else if((strcmp(argv[1], "-type") == 0) || (strcmp(argv[1], "-size") == 0))
        {
            searchTerm = "*";

            flag = argv[1];

            flagArg = argv[2];
        }
        else
        {
            printf(1, "Error: Bad arguments\n");
            exit();
        }
    }
    else if(argc == 2)
    {
        rootDir = argv[1];

        searchTerm = "*";

        flag = "*";

        flagArg = "*";   
    }
    else if(argc == 1)
    {
        rootDir = ".";

        searchTerm = "*";

        flag = "*";

        flagArg = "*";   
    }
    else
    {
        printf(1, "Error: Bad arguments\n");
        exit();
    }
 
    // Removes attached / from the path for uniform processing
    if(strcmp(&rootDir[strlen(rootDir) - 1], "/") == 0)
    {
        rootDir[strlen(rootDir) - 1] = '\0';
    }

    find(rootDir, searchTerm, flag, flagArg);

    exit();
}