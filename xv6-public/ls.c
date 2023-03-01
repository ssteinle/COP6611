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
    // Check type and add / if folder:
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

void ls(char *path, int show_hidden)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        printf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        printf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
            printf(1, "%s %d %d %d\n", fmtname(path, st.type), st.type, st.ino, st.size);
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf(1, "ls: path too long\n");
                break;
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
                    printf(1, "ls: cannot stat %s\n", buf);
                    continue;
                }
                // Show hidden files and folders:
                switch(show_hidden){
                    case 0:
                        if (fmtname(buf,st.type)[0] != 46 || st.ino == 1){
                            printf(1, "%s %d %d %d\n", fmtname(buf,st.type), st.type, st.ino, st.size);
                        }
                        break;
                    case 1:
                        printf(1, "%s %d %d %d\n", fmtname(buf,st.type), st.type, st.ino, st.size);
                        break;
                }
            }

            break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    time_scheduled(5);
    // int i;
    // int min = 2;
    // int path_start = 1;
    // int hidden = 0;
    // // Show hidden files.
    // if(strcmp(argv[1], "-a") == 0){
    //     hidden = 1;
    //     path_start = 2;
    //     min = 3;
    // }

    // if(argc < min){
    //     ls(".", hidden);
    // }
    // else{
    //     for(i=path_start; i<argc; i++){
    //         ls(argv[i], hidden);
    //     }
    // }

    exit();
}
