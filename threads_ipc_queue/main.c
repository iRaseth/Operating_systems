#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<pthread.h>
#include<time.h>
#include<strings.h>

struct msgbuf{
	long type;
	int number;
};

int create_queue(void)
{
	int queue_id = msgget(IPC_PRIVATE, 0600|IPC_CREAT|IPC_EXCL);
	if(queue_id == -1)
		perror("msgget");
	return queue_id;
}

void* generate_numbers(void *data)
{
	int *queue_id = (int*)data;
	struct msgbuf buffer;
	for(int i=0; i<10; i++)
	{
		buffer.type = i+1;
		buffer.number = rand()%10+1;
		if(msgsnd(queue_id[0], &buffer, sizeof(buffer.number),0) == -1)
			perror("msgsnd");
		buffer.type = buffer.type + 10;
		if(msgsnd(queue_id[0], &buffer, sizeof(buffer.number),0) == -1)
				perror("msgsnd");
	}
	bzero(&buffer,sizeof(buffer));
	return NULL;
}

void* multiply(void *data)
{
	int *queue_id = (int*)data;
	int multi = 1;
	struct msgbuf buffer;
	for(int i=0; i<10; i++)
	{
	if(msgrcv(queue_id[0], &buffer, sizeof(buffer.number),i+1,0) == -1)
			perror("msgrcv mutliply");
	multi = multi * buffer.number;
	}
	printf("multiplication: %d\n", multi);
	bzero(&buffer,sizeof(buffer));
	return NULL;
}

void* sum(void *data)
{
	int *queue_id = (int*)data;
	struct msgbuf buffer;
	float sum = 0;
	for(int i=0; i<10; i++)
	{
		if(msgrcv(queue_id[0],&buffer,sizeof(buffer.number),10+i+1,0) == -1)
				perror("msgrcv sum");
		sum = sum + buffer.number;
		printf("sum : %.2f, incremented by: %d\n",sum,buffer.number);
	}
	bzero(&buffer,sizeof(buffer));
	float average = sum / 10;
	printf("average value : %.2f\n",average);
}

int main(void)
{
	srand(time(NULL));
	pthread_t thread_g, thread_m, thread_a;
	int queue_id = create_queue();
	if(pthread_create(&thread_g, NULL, &generate_numbers, &queue_id) == -1)
		perror("pthread create generate_numbers");
	if(pthread_create(&thread_m, NULL, &multiply, &queue_id) == -1)
		perror("pthread multily error");
	if(pthread_create(&thread_a, NULL, &sum, &queue_id) == -1)
		perror("pthread multily error");
	if(pthread_join(thread_g,NULL) != 0)
		perror("pthread join thread_g");
	if(pthread_join(thread_m,NULL) != 0)
		perror("pthread join thread_m");
	if(pthread_join(thread_a,NULL) != 0)
		perror("pthread join thread_a");
	msgctl(queue_id, IPC_RMID, 0);
	return 0;
}
