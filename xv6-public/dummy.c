#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
  int pid;
  int h, g;
  int a, b;

  g = 2;
  a = 0;
  pid = 0;

  for ( h = 0; h < g; h++ ) {
    pid = fork ();
    if ( pid < 0 ) {
      printf(1, "not working \n");
    } else if (pid > 0) {
      printf(1, "p. %d creates c. %d\n",getpid(), pid);
      wait();
      }
      else{
	printf(1,"c. %d made\n",getpid());
	for(b = 0; b < 999999999999; b+=1)
	    a = a + 259; 
	break;
      }
  }
  exit();
}


