#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    int myPid = -10;

    printf(1, "myPid 0: %d\n", myPid);

    myPid = getpid();

    printf(1, "myPid 1: %d\n", myPid);

    myPid = time_scheduled(myPid);
    
    printf(1, "myPid 2: %d\n", myPid);
    
    exit();
}