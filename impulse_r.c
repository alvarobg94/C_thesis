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
#include <pigpio.h>
#define PORT 8080

/////////////////GLOBAL VARIABLES
int flag=1;

///////////////////////////////////////THREAD FUNCTIONS

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
   int n_samples=1000;
   int n_data=2;
   int data[1000];
   int t_series[1000];

   for (int i = 0; i < n_samples; ++i)
    {data[i]=0;t_series[i]=0;}

   int i,ml;
   i=ml=0;
   long long t_delay=1e7;
   double t_s,tnext,v_out,mean_f,mean_ts,cpu_time_used;
   t_s=v_out=0.0;
   struct timeval t1,t2,t0,tf;
   unsigned test_magnet,duty_cycle,dc_range;
   test_magnet=27;duty_cycle=0;dc_range=2000;
   pthread_t thread_id;
   pthread_create(&thread_id, NULL, UserGUI, NULL);////////////////THREAD
   FILE *f = fopen("impulse_magnet.txt", "w");
   if (f == NULL){printf("Error opening file!\n");exit(1);}/////FILE
   if (gpioInitialise() < 0){printf("pigpio initialisation failed.\n");}/////GPIO
   ///////PWM SETTING
   gpioSetPWMrange(test_magnet,2000);
   gpioSetPWMfrequency(test_magnet,8000);
   gpioPWM(test_magnet, 0);

   
   gettimeofday(&t0, NULL);
   ////////////////////////////////////////////////////////////////LOOOP




   while((flag==1)&&(i<n_samples))/////////////////////////SIGNAL
     {
    gettimeofday(&t1, NULL);
    gpioPWM(test_magnet,duty_cycle );
    v_out=duty_cycle/100;
    data[i]=v_out;
    t_series[i]=t_s;
    nanosleep((const struct timespec[]){{0, t_delay}}, NULL);
    //// Calculate next step
    duty_cycle++;
    if (duty_cycle>dc_range)
    {
      duty_cycle=0;
    }
    printf("dc is %d\n",duty_cycle );
    ////Calculate time step
    gettimeofday(&t2, NULL);
    t_s=(tf.tv_sec - t0.tv_sec) + (tf.tv_usec - t0.tv_usec) / 1000000.0f;
    i++;
    ////////append
    

     }


   gpioPWM(test_magnet, 0);
   gettimeofday(&tf, NULL);
   cpu_time_used=(tf.tv_sec - t0.tv_sec) + (tf.tv_usec - t0.tv_usec) / 1000000.0f;
   mean_f=n_samples/cpu_time_used;
   mean_ts=1/mean_f;
   printf("took %f\nmean t_s is %f\n", cpu_time_used,mean_ts);
  

   printf("Data cllection ended  with %d samples\n",i );
  /////////////////////////////////     TXT FILE GENERATION
   for(int m = 0; m < n_samples; m++) {
   fprintf(f,"%f, %f \n",t_series[i],data[i]);}
//////////////////////////////   CLOSING
   pthread_join(thread_id, NULL); 
   fclose(f);
return(0);

}


