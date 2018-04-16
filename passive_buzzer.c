#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <time.h>

//------------------------


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

//int song_1[] = {CM3,CM5,CM6,CM3,CM2,CM3,CM5,CM6,CH1,CM6,CM5,CM1,CM3,CM2,
//				CM2,CM3,CM5,CM2,CM3,CM3,CL6,CL6,CL6,CM1,CM2,CM3,CM2,CL7,
//				CL6,CM1,CL5};

//int beat_1[] = {1,1,3,1,1,3,1,1,1,1,1,1,1,1,3,1,1,3,1,1,1,1,1,1,1,2,1,1,
//				1,1,1,1,1,1,3};
//

//int song_2[] = {CM1,CM1,CM1,CL5,CM3,CM3,CM3,CM1,CM1,CM3,CM5,CM5,CM4,CM3,CM2,
//				CM2,CM3,CM4,CM4,CM3,CM2,CM3,CM1,CM1,CM3,CM2,CL5,CL7,CM2,CM1
//				};

//int beat_2[] = {1,1,1,3,1,1,1,3,1,1,1,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,3,3,2,3};


int tmp = 0;

int song_1[] = {CM3,CM3,CM4,CM5,CM5,CM4,CM3,CM2};
int beat_1[] = {1,1,1,1,1,1,1,1};

void Print(int);
int main(void)
{
	int i, j;
   
 	if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
		printf("setup wiringPi failed !");
		return 1; 
	}

	if(softToneCreate(BuzPin) == -1){
		printf("setup softTone failed !");
		return 1; 
	}
       
        pinMode(TouchPin, INPUT);
	
        time_t seconds = time(NULL);
        time_t result;
      
 
  //  while (1){Print(digitalRead(TouchPin));}
        
 
 
//	   Print(digitalRead(TouchPin));
	   printf("music is being played...\n");
 
     int record,flag=1;
     while (flag){
                	printf("%d\n",digitalRead(TouchPin));
		
if (digitalRead(TouchPin)==0){record=0;
                              softToneWrite(BuzPin, song_1[i%(sizeof(song_1)/4)]); 
			      delay(beat_1[i%(sizeof(song_1)/4)] * 300);}		
			else {if (record++>6) flag=0; 
                              softToneWrite(BuzPin, 0);  
                              delay(50);	}

           
        i++;
            } 
                

	return 0;
}
 
void Print(int x){
	if (x != tmp){
		if (x == 0)
			printf("...ON\n");
		if (x == 1)
			printf("OFF..\n");
		tmp = x;
	}
}

 

