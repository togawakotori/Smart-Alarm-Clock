#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#define BUFFER_SIZE 6

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    int total_request = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    time_t sys; 
    time_t input;
    struct tm input_;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
    
    int buf[BUFFER_SIZE];

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
        memset(buf,0,sizeof(buf));
        recv(connfd, buf, sizeof(buf),0);
	sys = time(NULL);
	strptime(buf, "%Y-%m-%d %H:%M:%S", &input_);
	input_.tm_isdst = -1;
	input = mktime(&input_);
	printf("time received is %s\n", ctime(&input));
	printf("difftime is %f\n", difftime(sys, input));
        close(connfd);
        sleep(1);
     }
}

