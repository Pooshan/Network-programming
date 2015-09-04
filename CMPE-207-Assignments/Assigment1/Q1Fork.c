#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

int main()

{
	pid_t pidp, pidc;
	int i,j;
	static int cnt;
	printf("\n I am the parent process with PID %d \n", getpid());
	pidp = getpid();
	fflush(stdout);
	pidc = fork();

	printf("%d", cnt);
	fflush(stdout);
	++cnt;
	j=cnt;
for (i = 1; i < 4; ++i) {

		if (pidc == 0) {

			printf("\n I am a child with PID %d and my parent ID is %d \n",
					getpid(), pidp);
			printf("%d", j);
			fflush(stdout);
			break;
		}

		if (pidc < 0) {
			printf("fork error\n");
			exit(1);
		} else {
			sleep(2);
		}
		pidc = fork();
		fflush(stdout);
		j=0;
		++cnt;
		j=cnt;

	}
}

