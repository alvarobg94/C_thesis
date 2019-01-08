#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <pigpio.h>
#define PORT 8080
#define DevAddr 0x48
#define B_convert 0x8000
#define B_null 0x0000
#define Mux_com_1_0 0x0000
#define Mux_com_3_0 0x1000
#define Mux_com_3_1 0x2000
#define Mux_com_3_2 0x3000
#define Mux_com_0 0x4000
#define Mux_com_1 0x5000
#define Mux_com_2 0x6000
#define Mux_com_3 0x7000
#define Amp_3_2 0x0000
#define Amp_1 0x0200
#define Amp_1_2 0x0400
#define Amp_1_4 0x0600
#define Amp_1_8 0x0800
#define Amp_1_16 0x0a00
#define B_cont 0x0000
#define B_sing 0x0100
#define DR_128 0x0000
#define DR_250 0x0020
#define DR_490 0x0040
#define DR_920 0x0060
#define DR_1600 0x0080
#define DR_2400 0x00a0
#define DR_3300 0x00c0
#define COMP_MODE_T 0x0000
#define COMP_MODE_W 0x0001
#define COM_POL_L 0x0000
#define COM_POL_H 0x0008
#define COMP_LN 0x0000
#define COMP_LA 0x0004
#define COMP_QUE_1 0x0000
#define COMP_QUE_2 0x0001
#define COMP_QUE_3 0x0002
#define COMP_QUE_N 0x0003
/////////////////GLOBAL VARIABLES
int DATA_SEND[4]={0,0,0,0};
//int CONFIG_V[4]={0,0,0,0};
//fd=  wiringPiI2CSetup (DevAddr);

///////////////////////////////////////THREAD FUNCTIONS
int flag=1;
///////////////////////////////////////FUNCTIONS
void *UserGUI(void *vargp) 
{   
    char command[1];
    flag=1;
    while (flag==1){
      scanf("%s",command);
      if (strcmp(command,"s")==0){
        flag=0;}}
 
    printf("Thread interface closing everything \n"); 
    return NULL; 
} 


///////////////////////////////////////FUNCTIONS

//////////////////////////////////////////////// MAIN
int main() {
   int data,fd,reg,Config1,Config2,Config3,Config4,i,Init;
   int CONFIG_V[4]={0,0,0,0};
   long long t_delay=5e5;
   double t_s=0;
   struct timeval t1,t2,t0,tf;
   unsigned red,yellow,green,red2;
   red=14;yellow=15;green=17;red2=27;
   pthread_t thread_id;
   pthread_create(&thread_id, NULL, UserGUI, NULL);


   if (gpioInitialise() < 0)
    {
      printf("pigpio initialisation failed.\n");
    }
   gpioSetPWMrange(red,2000);
   gpioSetPWMrange(yellow,2000);
   gpioSetPWMrange(green,2000);
   gpioSetPWMrange(red2,2000);
   gpioSetPWMfrequency(red,8000);
   gpioSetPWMfrequency(yellow,8000);
   gpioSetPWMfrequency(green,8000);
   gpioSetPWMfrequency(red2,8000);

   gpioPWM(red, 0);
   gpioPWM(green, 0);
   gpioPWM(yellow, 0);
   gpioPWM(red2, 0);
   i=0;

   ///////////////////////WRITNG REGISTER CONFIGURATION

   i=1;
   unsigned mr,mg,my,mr2;
   mr=0;mg=500;my=1000;mr2=1999;
   gettimeofday(&t0, NULL);
   while(flag==1)
     {
    mr++;mg++;my++;mr2++;
    gpioPWM(red,mr );
    gpioPWM(green, mg);
    gpioPWM(yellow, my);
    gpioPWM(red2, mr2);
    {
      mr=0;
    }
    if (mg==2000)
    {
      mg=0;
    }
    if (my==2000)
    {
      my=0;
    }
    if (mr2==2000)
    {
      mr2=0;
    }
    nanosleep((const struct timespec[]){{0, t_delay}}, NULL);
     }


   gpioPWM(red, 0);
   gpioPWM(green, 0);
   gpioPWM(yellow, 0);
   gpioPWM(red2, 0);
   gettimeofday(&tf, NULL);
   t_s=(tf.tv_sec - t0.tv_sec) + (tf.tv_usec - t0.tv_usec) / 1000000.0f;
  printf("took %f\n", t_s);
  return(0);
}

