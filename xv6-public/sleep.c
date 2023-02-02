#include "types.h"
#include "user.h"


int main(int argc, char *argv[]) {

	if (argc < 2){
		printf(1, "Enter time in milliseconds...\n");
		exit();
	}
	
    int time = atoi(argv[1]);
	if (time > 0){

        sleep(time);
        exit();
	} 
    printf(1, "Enter a positive value...\n");
	exit();
}



