#include "types.h"
#include "user.h"


int main(int argc, char *argv[]) {


    if (argc < 2){
        int calling_pid = getpid();

        printf(1, "%d", calling_pid);
        printf(1, "This is time_schedule() of calling function...\n");
        return calling_pid;
    }
    else{

        int pid = atoi(argv[1]);
        int aa = uptime();
        printf(1, "%d", pid);
        printf(1, "%d", aa);
        exit();

    }
}