#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

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

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if(stat(buf, &st) < 0){
            printf(1, "find: cannot stat %s\n", buf);
            continue;
        }

        // printf(1, "%s - %s\n", p, searchTerm);
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

                for(int i=0; i<(strlen(sizeNum)); i++)
                {
                    if((sizeNum[i] < '0') || (sizeNum[i] > '9')) 
                    {
                        printf(1, "%c in %s is not an int!\n", sizeNum[i], sizeNum);
                        exit();
                    }
                }

                // printf(1, "arghook: %d -- atoi(sizeNum): %d\n", argHook, atoi(sizeNum));

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

        if((st.type == 1) && (strcmp(p, ".") != 0) && (strcmp(p, "..") != 0))
        {
            // printf(1, "%s/%s\n", path, p);
            // printf(1, "%s\n", fmtname(buf,st.type));
            int lenDir = strlen(p);
            int lenPath = strlen(path);

            // printf(1, "Path: %s (%d) -- Dir: %s (%d)\n", path, lenPath, p, lenDir);
            char dest[lenDir + lenPath + 1];

            for(int i=0; i<lenPath; i++)
            {
                // printf(1, "%c : %c\n", dest[i], path[i]);
                dest[i] = path[i];
            }

            dest[lenPath] = '/';

            for(int i=0; i<lenDir; i++)
            {
                // printf(1, "%c : %c\n", dest[lenPath + 1 + i], p[i]);
                dest[lenPath + 1 + i] = p[i];
            }

            dest[lenPath + 1 + lenDir] = '\0';      

            // printf(1, "New Path: %s\n", &dest);

            // // strcat(dest, p);
            // // printf(1, "%s\n", dest);
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
 
    // if((strcmp(flag, "*") != 0) && (strcmp(flagArg, "*") != 0))
    // {    
    //     if((strcmp(flag, "-type") != 0) && (strcmp(flag, "-size") != 0))
    //     {
    //         printf(1, "Error: Bad flag\n");
    //         exit();
    //     }
    
    //     if((strcmp(flag, "-type") == 0))
    //     {
    //         if((strcmp(flagArg, "f") != 0) && (strcmp(flagArg, "d") != 0))
    //         {
    //             printf(1, "Error: Bad flag argument\n");
    //             exit();
    //         }
    //     }
    // }
 
    if(strcmp(&rootDir[strlen(rootDir) - 1], "/") == 0)
    {
        rootDir[strlen(rootDir) - 1] = '\0';
    }

    find(rootDir, searchTerm, flag, flagArg);

    exit();
}