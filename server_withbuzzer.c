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
#include <wiringPi.h>
#include <softTone.h>
 
#define BuzPin    0
#define TouchPin  1 

#define  CL1  131
#define  CL2  147
#define  CL3  165
#define  CL4  175
#define  CL5  196
#define  CL6  221
#define  CL7  248

#define  CM1  262
#define  CM2  294
#define  CM3  330
#define  CM4  350
#define  CM5  393
#define  CM6  441
#define  CM7  495

#define  CH1  525
#define  CH2  589
#define  CH3  661
#define  CH4  700
#define  CH5  786
#define  CH6  882
#define  CH7  990

#define BUFFER_SIZE 6

int buzzer(void);
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
        if (abs(difftime(sys, input))<60) buzzer();
        close(connfd);
        sleep(1);
     }

}

int buzzer(void){
int i,record,flag=1;
int song_1[] = {CM3,CM3,CM4,CM5,CM5,CM4,CM3,CM2};
int beat_1[] = {1,1,1,1,1,1,1,1};
   
 	if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
		printf("setup wiringPi failed !");
		return 1; 
	}

	if(softToneCreate(BuzPin) == -1){
		printf("setup softTone failed !");
		return 1; 
	}
       
        pinMode(TouchPin, INPUT);
	
       printf("START ALARMING...\n");
 

     while (flag){
       printf("%d\n",digitalRead(TouchPin));
		
       if (digitalRead(TouchPin)==0){
          record=0;
          softToneWrite(BuzPin, song_1[i%(sizeof(song_1)/4)]); 
	  delay(beat_1[i%(sizeof(song_1)/4)] * 300);
       }		
       else {
         if (record++>6) flag=0; 
         softToneWrite(BuzPin, 0);  
         delay(50);	
       }
  
       i++;
    } 
                
  return 0;

}

