#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int arc, char **argv) {
	printf("--beginning of program\n");

	int counter = 0;
	sleep(2);
	printf("creating child\n");
	pid_t pid = fork(); 

	if (pid == 0) {
		// child process
		for (int i = 0; i < 10; ++i) {
			sleep(3);
			// printf("child process: counter = %d\n", ++counter);
		}
	}
	else if (pid > 0) {
		// parent process
		for (int j = 0; j < 10; ++j) {
			sleep(3);
			// printf("parent process: counter = %d\n", ++counter);

			if (j == 5) {
				printf("Exit parent process... child alone.\n");
				printf("Child will display a message when he is done (21sec).\n");
				printf("user prompt should be available:\n");
				exit(0); 
			}
		}
	}
	else {
		// fork failed
		printf("fork() failed!\n");
		return 1;
	}

	printf("--end of child process\n");

	return 0;
}