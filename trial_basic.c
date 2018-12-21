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
/////////////////GLOBAL VARIABLES

///////////////////////////////////////FUNCTIONS
//////////////////////////////////////////////// MAIN
int main() {
int data,fd,reg;
fd=  wiringPiI2CSetup (DevAddr);
data= wiringPiI2CReadreg8(fd,0x00) ;
printf("%d\n",data );

return(0);
}