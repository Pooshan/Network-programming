#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	pid_t i;
	//char* argc1={NULL};
	char *newargv[] = { NULL };
    char *newenviron[] = { NULL };
	printf("from parent  %d \n ", getpid());
	//execve("/Users/Pooshan/workspace/execve/Debug/filecall.o", newargv, newenviron);

	if(i!=0)
	{
	  execve("/Users/Pooshan/workspace/execve/Debug/filecall.o", newargv, newenviron);

	//int execve(const char *test, char *const argv[],char *const envp[]);

	exit(1);
	}
	return 0;
}
