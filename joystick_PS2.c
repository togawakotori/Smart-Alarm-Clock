#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>


#define PCF       120
#define uchar	unsigned char

int AIN0 = PCF + 0;
int AIN1 = PCF + 1;
int AIN2 = PCF + 2;

//char *state[6+1] = {"home", "down","up" ,"left", "right", "pressed","SegmentationFault"};
char *state[6] = {"home", "down","up" ,"left", "right", "pressed"};

int avg_x=220;
int avg_y=170;
int dev=20;
int direction(){
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

	if (b == 0)
		tmp = 5;		// button preesd
	if (x-avg_x<10 && x-avg_x>-10 && y-avg_y<10 && y-avg_y>-10 && b !=0)
		tmp = 0;		// home position
	
	return tmp;
}

int main (void)
{
	int tmp;
	int status = 0;
	wiringPiSetup ();
	// Setup pcf8591 on base pin 120, and address 0x48
	pcf8591Setup (PCF, 0x48);
	while(1) // loop forever
	{
		tmp = direction();
		if (tmp != status)
		{    //printf("%d\n",tmp);
			printf("%s\n", state[tmp]);
			status = tmp;
		}
               //usleep(100);
	}
	return 0 ;
}
