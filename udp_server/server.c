#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define PORT 8080
#define IP_ADDR "127.0.0.1"

int read_msg(int des)
{
	struct sockaddr_in client;
	socklen_t client_size = sizeof(client);
	int number,readbytes;
	for(int i=0;i<100;i++)
	{
		readbytes = recvfrom(des,&number,sizeof(number),0,(struct sockaddr*)&client,&client_size);
		if(readbytes == -1)
			perror("readbytes");
		if(number%3==0)
			printf("podzielna przez 3 : %d\n",number);
	}
}

int name_socket(int des)
{
	struct in_addr network_address;
	if(inet_aton(IP_ADDR,&network_address) == -1)
		perror("inet_aton");
	struct sockaddr_in server = {
		.sin_family = AF_INET,
		.sin_port = PORT,
		.sin_addr = network_address
	};
	if(bind(des,(struct sockaddr*)&server,sizeof(server))==-1)
		perror("bind");
}

int main(void)
{
	int des = socket(AF_INET, SOCK_DGRAM, 0);
	if(des == -1)
		perror("des");
	name_socket(des);
	read_msg(des);
	close(des);
}
