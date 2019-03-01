#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <time.h>

#define PORT 8080
#define IP_ADDR "127.0.0.1"

void send_msg(int des)
{
	struct in_addr network_address;
	if(inet_aton(IP_ADDR,&network_address) == -1)
		perror("inet aton");
	struct sockaddr_in client = {
		.sin_family = AF_INET,
		.sin_port = PORT,
		.sin_addr = network_address
	};
	int num;
	for(int i=0;i<100;i++)
	{
		num = rand()%10+1;
		if(sendto(des,(void*)&num,sizeof(num),0,(struct sockaddr*)&client,sizeof(client)) == -1)
			perror("sendto");
	}
}


int main(void)
{
	srand(time(NULL));
	int des = socket(AF_INET, SOCK_DGRAM, 0);
	if(des == -1)
		perror("des");
  send_msg(des);
	close(des);
}
