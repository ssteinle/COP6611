#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "proc.c"

int main(int argc, char *argv[]) {
  // int pid;
  // int h, g;
  int a, b;
  printf(2, "\n\n****************************************\n\n");

  acquire(&tickslock);
  uint xticks1 = ticks;
  release(&tickslock);

  // g = 2;
  // a = 0;
  fork();
  fork();
  fork();
  // for ( h = 0; h < g; h++ ) {
  //   pid = fork ();
  //   if ( pid < 0 ) {
  //     printf(1, "not working \n");
  //   } else if (pid > 0) {
  //     printf(1, "p. %d creates c. %d\n",getpid(), pid);
  //     wait();
  //     }
  //     else{
	// printf(1,"c. %d made\n",getpid());
	for(b = 0; b < 10000; b+=1)
	    a = a + 259;

  printf(2, "\nMy pid: %d\n", getpid());
	// break;
  //     }
  // }

  acquire(&tickslock);
  uint xticks = ticks - xticks1;
  release(&tickslock);

  pritnf(2, "\nMy pid: %d\n", )

  exit();
}

