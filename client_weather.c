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
#include <wiringPi.h>
#include <stdint.h>

#define BUFFER_SIZE 1024

#define MAXTIMINGS 85

#define DHTPIN 0

int dht11_dat[5] = {0,0,0,0,0};

void read_dht11_dat()
{
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	float f; // fahrenheit

	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

	// pull pin down for 18 milliseconds
	pinMode(DHTPIN, OUTPUT);
	digitalWrite(DHTPIN, LOW);
	delay(18);
	// then pull it up for 40 microseconds
	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(40); 
	// prepare to read the pin
	pinMode(DHTPIN, INPUT);

	// detect change and read data
	for ( i=0; i< MAXTIMINGS; i++) {
		counter = 0;
		while (digitalRead(DHTPIN) == laststate) {
			counter++;
			delayMicroseconds(1);
			if (counter == 255) {
				break;
			}
		}
		laststate = digitalRead(DHTPIN);

		if (counter == 255) break;

		// ignore first 3 transitions
		if ((i >= 4) && (i%2 == 0)) {
			// shove each bit into the storage bytes
			dht11_dat[j/8] <<= 1;
			if (counter > 16)
				dht11_dat[j/8] |= 1;
			j++;
		}
	}

	// check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	// print it out if data is good
	if ((j >= 40) && 
			(dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)) ) {
		f = dht11_dat[2] * 9. / 5. + 32;
		printf("Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n", 
				dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f);
	}
	else
	{
		printf("Data not good, skip\n");
	}
}


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
    if (wiringPiSetup () == -1)
	exit (1) ; 
	
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
		
		//printf("please enter time in the following format:\nyyyy-MM-dd HH:mm:ss\n");
		/*
		while((buf[inputLen++]=getchar())!='\n')
		{
		}
		buf[inputLen++] = 0;
	*/
		read_dht11_dat();
		delay(1000);
		//buf = dht11_dat;
	 
	    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		{
		   printf("\n Error : Connect Failed \n");
		   return 1;
		}

        //printf("Yes");
        	int k;
	       int buff[6];
	       buff[0] = 1;
	       for (k = 1; k < 6; k++){
	             buff[k] = dht11_dat[k-1];
	       }
	       
		printf("%d.%d %d.%d\n", dht11_dat[0],dht11_dat[1],dht11_dat[2],dht11_dat[3]);
		send(sockfd, buff, sizeof(buff), 0);
		
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
