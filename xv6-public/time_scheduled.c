#include "types.h"
#include "user.h"


// #include "defs.h"



int main(int argc, char *argv[]) {


    if (argc < 2){
        int calling_pid = getpid();

        printf(1, "%d\n", calling_pid);
        printf(1, "This is time_schedule() of calling function...\n");
        exit();
        // return calling_pid;
    }
    else{

        int pid = atoi(argv[1]);
        // int aa = uptime();


        // uint xticks;

        // acquire(&tickslock);
        // xticks = ticks;
        // release(&tickslock);


        printf(1, "%d\n", pid);
        // printf(1, "%d\n", aa);
        exit();

    }
}