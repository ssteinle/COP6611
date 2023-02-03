#include "types.h"
#include "user.h"


int main(int argc, char *argv[]) {

    // Print error if no time is given:
	if (argc < 2){
		printf(1, "Enter a time in milliseconds...\n");
		exit();
	}
	// Run sleep function for given time
    int time = atoi(argv[1]);
	if (time > 0){

        sleep(time);
        exit();
	} 
    printf(1, "Enter a positive value...\n");
	exit();
}



