#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];
int times[512];

char remove_duplicate(char* str, int ignore_case){
    char* compare1 = str;
    char* compare2 = str;
    int index = 0;
    int same = 1;
    times[0] = 1;
    while(*compare2 != 0 && *compare2 != '\n'){
        compare2++;
    }
    compare2++;
    str = compare2;

    while(*compare2 != 0){

        while(*compare1 == '\b'){
            compare1++;
        }
        if(*compare1 == '\n' && *compare2 == '\n'){
            times[index]++;
        }
        same = 1;
        if(ignore_case == 1){
                if(((*compare1 >= 65 && *compare1 <= 90) && (*compare2 >= 97 && *compare2 <= 122))
                    || ((*compare2 >= 65 && *compare2 <= 90) && (*compare1 >= 97 && *compare1 <= 122))) {

                    int result = *compare2 - *compare1;
                    if (!(result == 32 || result == -32)){
                        same = 0;
                    }
                }
                else{
                    int result = *compare2 - *compare1;
                    if (result != 0){
                        same = 0;
                    }
                }
        }
        else{
            if(*compare2 != *compare1){
                same = 0;
            }
        }

        if(same == 0){
            index++;
            while(*compare1 != 0 && *compare1 != '\n'){
                compare1++;
            }
            compare1++;
            if(*compare2 == '\n'){
                compare2--;
                if(*compare2 == '\n'){
                    compare2++;
                }
                else{
                    while(*compare2 != '\n'){
                        compare2--;
                    }
                    compare2++;
                }
            }
            else{
                while(*compare2 != '\n'){
                    compare2--;
                }
                compare2++;
            }
            
            int times1 = 0;
            int times2 = 0;
            int keep = 1;
            while(*compare2 != 0 && *compare2 != '\n'){

                if(*str != '\n' && keep == 1){
                    times1++;
                }
                else{
                    keep = 0;
                }

                *str = *compare2;
                str++;
                compare2++;
                times2++;
            }
            if(times1 < times2){
                while(times1 >= 0){
                    compare1--;
                    times1--;
                }
            }
            compare2++;
            while(*str != 0 && *str != '\n'){
                *str = '\b';
                str++;
            }
            str++;
            times[index]++;
        }
        else{
            compare1++;
            compare2++;
        }
    }

    *str++ = 0;
    return 1;
}

char uniq_read(int fd, int count, int only_same, int ignore_case){
    int j;
    int n;
    int max_size = sizeof(buf);
    char* buf_ptr=buf;
    n = read(fd, buf, max_size);
    if (n > max_size){
        printf(1, "error, the context bytes shouldn't exceed than %s", max_size);
        return 0;
    }
    else{
        buf[n] = 0;
        remove_duplicate(buf, ignore_case);

        if (count == 1 || only_same == 1){
            for(j=0;j<sizeof(times) && times[j] != 0;j++){
                if (count == 1){
                    printf(1, "%d ",times[j]);
                }
                while(*buf_ptr != 0){
                    if (only_same == 0 || times[j] != 1){
                        // break;
                        printf(1,"%c",*buf_ptr);
                    }
                    if(*buf_ptr++ == '\n'){
                        break;
                    }
                }
            }
            //printf(1,"\n");
            exit();
        }

        while(*buf_ptr != 0){
            printf(1,"%c",*buf_ptr++);
        }
        //printf(1,"\n");
        close(fd);
        exit();
    }
    return 0;
}


int main(int argc, char* argv[]){
    int fd;
    int count = 0;
    int only_same = 0;
    int ignore_case = 0;

    if(argc < 2){
        uniq_read(0,count, only_same, ignore_case);
    }
    if(argc == 2){
        if((fd = open(argv[1],0)) < 0){
            printf(2, "uniq: cannot open %s \n", argv[1]);
            exit();
        }
        uniq_read(fd, count, only_same, ignore_case);
        close(fd);
        exit();
    }
    if(argc > 2){
          if(strcmp(argv[1], "-c") == 0){
            count = 1;
            }
          if(strcmp(argv[1], "-d") == 0){
            only_same = 1;
            }
          if(strcmp(argv[1], "-i") == 0){
            ignore_case = 1;
          }
          if((fd = open(argv[2],0)) < 0){
            printf(2, "uniq: cannot open %s \n", argv[2]);
            exit();
          }
          uniq_read(fd, count, only_same, ignore_case);
          close(fd);
    }
    exit();
}

