#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{

  if(argc < 2){
        printf(2,"Invalid input. \n");
  }
  if(strcmp(argv[1], "get") == 0){
        get_sched_priority(1);
  }
  if(strcmp(argv[1], "set") == 0){
        set_sched_priority(10);
  }
  if(strcmp(argv[1], "time") == 0){
	time_scheduled(11);
  }
  cps();
  exit();
}
