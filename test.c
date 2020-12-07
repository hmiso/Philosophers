
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;

void print_arr(int *arr)
{
	int i = 0;
	while(i < 10)
	{
		printf("%d ", arr[i]);
		i++;
	}
	printf("\n");
}

void* count(void *arr)
{
	int i = 0;
	pthread_mutex_lock(&mutex);
	while(i < 10)
	{
		((int *)arr)[i]++;
		i++;
	}
	print_arr((int*)arr);
	usleep(1000);
	i = 0;
	while(i < 10)
	{
		((int *)arr)[i]--;
		i++;
	}
	print_arr((int*)arr);
	printf("end\n");
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main()
{
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	// pthread_t p1, p2;
	pthread_t *mas_fil;
	mas_fil = malloc(sizeof(pthread_t) * 2);
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&mas_fil[0], NULL, count, (void *)arr);
	pthread_create(&mas_fil[1], NULL, count, (void *)arr);
	// pthread_join(mas_fil[0], NULL);
	// pthread_join(mas_fil[1], NULL);
	int i = 0;
	while(i < 2)
	{
		pthread_join(mas_fil[i], NULL);
		i++;
	}	
	return 0;
}