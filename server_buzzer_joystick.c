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
#include <pcf8591.h>
 
#define BuzPin    0
#define TouchPin  1
#define PCF       120 

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

int AIN0 = PCF + 0;
int AIN1 = PCF + 1;
int AIN2 = PCF + 2;
 
char *state[6] = {"home", "down","up" ,"left", "right", "pressed"};

int avg_x=220;
int avg_y=170;
int dev=20;

int song_1[] = {CM3,CM3,CM4,CM5,CM5,CM4,CM3,CM2};
int beat_1[] = {1,1,1,1,1,1,1,1};

int direction(void);
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
    
       if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
		printf("setup wiringPi failed !");
		return 1; 
	}

	if(softToneCreate(BuzPin) == -1){
		printf("setup softTone failed !");
		return 1; 
	}
       
    pcf8591Setup (PCF, 0x48);// Setup pcf8591 on base pin 120, and address 0x48
  //  buzzer();
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
        memset(buf,0,sizeof(buf));
        recv(connfd, buf, sizeof(buf),0);
	sys = time(NULL);
         buf[sizeof(buf)+1]=0;
       printf("%d %d\n",buf[0],buf[1]);
	//strptime(buf, "%Y-%m-%d %H:%M:%S", &input_);
	//input_.tm_isdst = -1;
	//input = mktime(&input_);
//	printf("time received is %s\n", ctime(&input));
//	printf("difftime is %f\n", difftime(sys, input));
   //     if (abs(difftime(sys, input))<60) buzzer();
        close(connfd);
        sleep(1);
     }

}

int buzzer(void){
      
     printf("START ALARMING...\n");
     
     int i,record,flag=1;
     int tmp, status = 0;
     
     pinMode(TouchPin, INPUT);

     while (flag){
         printf("%d\n",digitalRead(TouchPin));
         tmp = direction();
	 if (tmp != status)
		{    
                       if (tmp==3) {int i; for (i=0;i<8;i++) beat_1[i]++;}
			printf("%s\n", state[tmp]);
			status = tmp;
		}
    
       if (digitalRead(TouchPin)==0)
  	{
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

int direction(void){
	int x, y, b;
	int tmp=0;
	x = analogRead(AIN1);
	y = analogRead(AIN0);
	b = analogRead(AIN2);
      // printf("%d %d %d\n",x,y,b);
        if (y-avg_y<-dev)  tmp = 1;		// down
 	if (y-avg_y>+dev) tmp = 2;		// up
        if (x-avg_x>+dev) tmp = 3;		// left
	if (x-avg_x<-dev)  tmp = 4;		// right
	if (b == 0) tmp = 5;		// button preesd
	if (x-avg_x<10 && x-avg_x>-10 && y-avg_y<10 && y-avg_y>-10 && b !=0)
		tmp = 0;		// home position
	
	return tmp;
}


