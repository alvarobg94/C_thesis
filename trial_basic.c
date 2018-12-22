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

///////////////////////////////////////FUNCTIONS
int Byte_swapper(int data16)
{
  int data,data1,data2;
  data1=(data16<<8)&0xff00;
  data2=data16>>8;
  data=data1 | data2;
  return data;
}
//////////////////////////////////////////////// MAIN
int main() {
   int data,fd,reg,num1,numswap,i;
   long long t_delay=5e4;
   num1= B_convert | Mux_com_0;
   num1= num1 | Amp_1;
   num1=num1 | B_sing;
   num1=num1 | DR_128;
   num1=num1 | COMP_MODE_T;
   num1=num1 | COM_POL_L;
   num1=num1 | COMP_LN;
   num1=num1 | COMP_QUE_N;
   numswap=Byte_swapper(num1);
   fd=  wiringPiI2CSetup (DevAddr);
   i=0;
   while(i<20){
   data= wiringPiI2CReadReg16(fd,0x00) ;
   data=Byte_swapper(data);
   data=data>>4;
   printf("%d\n",data );
   wiringPiI2CWriteReg16(fd,0x01,numswap);
   data= wiringPiI2CReadReg16(fd,0x01) ;
   printf("%x\n",(data&0x0080));
   nanosleep((const struct timespec[]){{0, t_delay}}, NULL);
   data= wiringPiI2CReadReg16(fd,0x01) ;
   printf("%x\n",(data&0x0080));
   i=i+1;
   }

return(0);
}