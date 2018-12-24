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



///////////////////////////////////////FUNCTIONS

int Byte_swapper(int data16)
 {
  int data,data1,data2;
  data1=(data16<<8)&0xff00;
  data2=data16>>8;
  data=data1 | data2;
  return data;
 }
int Config_Channel(int Config_16num,int Channel)
 { 
  switch(Channel)
   {
   case 1:
     Config_16num=Config_16num|B_convert|Mux_com_0;
     break;
   case 2:
     Config_16num=Config_16num|B_convert|Mux_com_1;
     break;
   case 3:
     Config_16num=Config_16num|B_convert|Mux_com_2;
     break;
   case 4:
     Config_16num=Config_16num|B_convert|Mux_com_3;
     break;
   default:
      printf("%s\n%s\n","Error: Not a valid Channel. ","Choose between channel 1 and 4");
   }
      
  return Config_16num;
 }
int Config_DR(int Config_16num,int DR)
 { 
  switch(DR)
   {
   case 128:
     Config_16num=Config_16num|DR_128;
     break;
   case 250:
     Config_16num=Config_16num|DR_250;
     break;
   case 490:
     Config_16num=Config_16num|DR_490;
     break;
   case 920:
     Config_16num=Config_16num|DR_920;
     break;
   case 1600:
     Config_16num=Config_16num|DR_1600;
     break;
   case 2400:
     Config_16num=Config_16num|DR_2400;
     break;
   case 3300:
     Config_16num=Config_16num|DR_3300;
     break;
   default:
      printf("%s\n%s\n","Error: Not a valid Data Rate. ","Choose between possible Data Rates: 128, 250, 490, 920, 1600, 2400, 3300");
   }
  return Config_16num;
 }
int Config_Gain(int Config_16num,int gain)
 { 
  switch(gain)
   {
   case 1:
     Config_16num=Config_16num|Amp_1;
     break;
   case 2:
     Config_16num=Config_16num|Amp_1_2;
     break;
   case 4:
     Config_16num=Config_16num|Amp_1_4;
     break;
   case 8:
     Config_16num=Config_16num|Amp_1_8;
     break;
   case 16:
     Config_16num=Config_16num|Amp_1_16;
     break;
   default:
      printf("%s\n%s\n","Error: Not a valid Gain. ","Choose between possible gains: 1, 2, 4, 8, 16");
   }
 return Config_16num;
 }
int Config_Mode(int Config_16num, int mode)
 {
  switch (mode)
    {
    case(0):
      Config_16num=Config_16num|B_sing;
      break;
    case(1):
      Config_16num=Config_16num|B_cont;
      break;
    default:
      printf("%s\n%s\n","Error: Not a valid Mode. ","Choose between Continuous (1) or Single-Shot (0)");
     }
  return Config_16num;
 }
int Config_Default(int Config_16num)
 {
   Config_16num=Config_16num | COMP_MODE_T;
   Config_16num=Config_16num | COM_POL_L;
   Config_16num=Config_16num | COMP_LN;
   Config_16num=Config_16num | COMP_QUE_N;
   return Config_16num;
 }
//////////////////////////////////////////////// MAIN
int main() {
   int data,fd,reg,Config1,Config2,Config3,Config4,i;
   int CONFIG_V[4]={0,0,0,0};
   long long t_delay=5e5;
   double t_s=0;
   struct timeval t1,t2,t0,tf;
   //pthread_t thread1,thread2,thread3,thread4; 
   //pthread_create(&thread_id, NULL, UserGUI, NULL);

   fd=  wiringPiI2CSetup (DevAddr);
   i=0;

   ///////////////////////WRITNG REGISTER CONFIGURATION
   Config1=0;
   Config1=Config_Default(Config1);
   Config1=Config_Channel(Config1,1);//Channel 1
   Config1=Config_DR(Config1,3300);
   Config1=Config_Gain(Config1,1);
   Config1=Config_Mode(Config1,0);
   Config1=Byte_swapper(Config1);
   CONFIG_V[0]=Config1;

   Config2=0;
   Config2=Config_Default(Config2);
   Config2=Config_Channel(Config2,2);//Channel 2
   Config2=Config_DR(Config2,3300);
   Config2=Config_Gain(Config2,1);
   Config2=Config_Mode(Config2,0);
   Config2=Byte_swapper(Config2);
   CONFIG_V[1]=Config2;

   Config3=0;
   Config3=Config_Default(Config3);
   Config3=Config_Channel(Config3,3);//Channel 3
   Config3=Config_DR(Config3,3300);
   Config3=Config_Gain(Config3,1);
   Config3=Config_Mode(Config3,0);
   Config3=Byte_swapper(Config3);
   CONFIG_V[2]=Config3;
   
   Config4=0;
   Config4=Config_Default(Config4);
   Config4=Config_Channel(Config4,4);//Channel 4
   Config4=Config_DR(Config4,3300);
   Config4=Config_Gain(Config4,1);
   Config4=Config_Mode(Config4,0);
   Config4=Byte_swapper(Config4);
   CONFIG_V[3]=Config4;

   gettimeofday(&t0, NULL);
   while(i<100)
     {
    for (int j = 0; j <4; j++)
    {
     
     wiringPiI2CWriteReg16(fd,0x01,CONFIG_V[j]);
     nanosleep((const struct timespec[]){{0, t_delay}}, NULL);
     data= wiringPiI2CReadReg16(fd,0x00) ;
     data=Byte_swapper(data);
     DATA_SEND[j]=data>>4;
     }
   for (int j = 0; j < 4; ++j)
   {
     printf("%d ",DATA_SEND[j] );
   }
   printf("\n");
   i++;
   }

   gettimeofday(&tf, NULL);
   t_s=(tf.tv_sec - t0.tv_sec) + (tf.tv_usec - t0.tv_usec) / 1000000.0f;
  printf("took %f\n", t_s);
  return(0);
}
