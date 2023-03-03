#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  //int id = 0;
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
      //if (strcmp(argv[2], "stressfs") == 0) {
      
      //}
	time_scheduled(11);
  }
  if(strcmp(argv[1], "position") == 0){
	int po = 0;
  	po = fifo_position(2);
	printf(2,"when pid is 2, the positon is: %d \n", po);
  }
  sps();
  exit();
}
