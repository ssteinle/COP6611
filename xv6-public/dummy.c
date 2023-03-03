#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
  // int pid;
  // int h, g;
  int a, b;
  printf(2, "\n\n****************************************\n\n");

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
  while(wait() != -1);

  exit();
}


