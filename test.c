#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include "libft/libft.h"

int main()
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{

	}
	else
	{
		waitpid(-1, 0, WUNTRACED);
	}
	
}