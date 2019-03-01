#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<time.h>
#include<sys/file.h>
#include<sys/wait.h>
#include<errno.h>

void child(int des)
{
	struct flock f1;
	f1.l_whence = SEEK_SET;
	f1.l_start = 0;
	f1.l_len = 0;
	int buffer[100];
	for(int i=0; i<100; i++)
	{
		buffer[i] = rand()%10+1;
	}
	f1.l_type = F_WRLCK;
	if(fcntl(des, F_SETLKW, &f1) == -1)
	{
		if(errno == EACCES || errno == EAGAIN)
		{
			printf("locked by another process");
		}
		perror("fcntl write lock");
	}
	if(lseek(des,0,SEEK_SET) == -1)
		perror("lseek");
	if(write(des, buffer, sizeof(buffer)) == -1)
		perror("write");
	f1.l_type = F_UNLCK;
	sleep(1);
	close(des);
	exit(0);
}

void parent(int des)
{
	wait(0);
	struct flock f1;
	f1.l_whence = SEEK_SET;
	f1.l_start = 0;
	f1.l_len = 0;
	int buffer[100];
	f1.l_type = F_RDLCK;
	if(fcntl(des, F_SETLKW, &f1) == -1)
	{
		perror("fcntl");
	}
	if(lseek(des,0,SEEK_SET) == -1)
		perror("lseek");
	if(read(des,&buffer,sizeof(buffer))<0)
		perror("read");
	f1.l_type = F_UNLCK;
	if(fcntl(des, F_SETLKW, &f1) == -1)
		perror("unlock fcntl");
	for(int i=0; i<100; i++)
	{
		if(buffer[i]%3==0)
		printf(" %d ",buffer[i]);
	}
	close(des);
}

int main(void)
{
  srand(time(NULL));
  int filedes = open("file.txt", O_RDWR | O_CREAT, 0600);
  if(filedes == -1)
			perror("filedes");
	int duplicatedes = dup(filedes);
	if(duplicatedes == -1)
		perror("duplicatedes");
	pid_t pid = fork();
	if(pid == -1)
		perror("pid");
	if(pid == 0)
	{
			child(filedes);
	}else{
			parent(duplicatedes);
	}
}
