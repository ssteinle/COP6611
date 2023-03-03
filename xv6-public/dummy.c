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

	for(b = 0; b < 10000; b+=1)
		a = a + 259;

  printf(2, "\nMy pid: %d\n", getpid());
	// break;
  //     }
  // }
  while(wait() != -1);
  exit();
}


