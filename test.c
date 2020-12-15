#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		getchar();
	}
	else
	{
		waitpid(-1, 0, WUNTRACED);
		geatchar();
	}
	
}