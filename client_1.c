#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define BUFFER_SIZE 1024


int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

	int value = 0;
	int currentRecv;

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 
	while (1){
		memset(recvBuff, '0',sizeof(recvBuff));
		if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
		    printf("\n Error : Could not create socket \n");
		    return 1;
		} 

		memset(&serv_addr, '0', sizeof(serv_addr)); 

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(5000); 

		if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
		{
		    printf("\n inet_pton error occured\n");
		    return 1;
		} 

		char buf[BUFFER_SIZE];
		int inputLen=0;
		
		printf("please enter time in the following format:\nyyyy-MM-dd HH:mm:ss");
		
		while((buf[inputLen++]=getchar())!='\n')
		{
		}
		buf[inputLen++] = 0;
	
	    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		{
		   printf("\n Error : Connect Failed \n");
		   return 1;
		}

        //printf("Yes");
	    
		
		send(sockfd, buf, sizeof(buf), 0);
		
		/*while ((n = read(sockfd, recvBuff, sizeof(recvBuff)-1))>0){
			recvBuff[n] = 0;
			currentRecv = atoi(recvBuff);
			printf("previous value: %d \n", value);
			value += currentRecv;
			printf("current value: %d \n", value);
		}*/
		sleep(1);
	}

	return 0;
}
/*
    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    } 

    if(n < 0)
    {
        printf("\n Read error \n");
    } 

    return 0;
}
*/
