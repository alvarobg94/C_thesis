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

   int i,ml,j,k,SIZE_DATA,n_data,n_samples;
   i=ml=k=0;j=1;n_data=2;SIZE_DATA=n_data*1000;
   double* d_array;
   d_array =calloc(SIZE_DATA,sizeof(double));
   if(d_array == NULL) {
    printf("malloc of d_array failed!\n");   
    exit(1);} 

   
   long long t_delay=95e4;
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




   while(flag==1)/////////////////////////SIGNAL
     {
    gettimeofday(&t1, NULL);
    gpioPWM(test_magnet,duty_cycle );
    v_out=duty_cycle/100.00;
    d_array[i]=t_s;
    i++;
    for (int b = 0; b < n_data-1; b++)
     {i++;d_array[i]=v_out;}

     k++;
  /////////ENLARGING THE DATA VECTOR FOR SAVING MORE DATA
     printf("%d\n",k );
    if (k==SIZE_DATA/n_data){
     j++;
     d_array= realloc(d_array, (SIZE_DATA*j) * sizeof(double));
     k=0;}
    nanosleep((const struct timespec[]){{0, t_delay}}, NULL);
    //// Calculate next step
    duty_cycle=duty_cycle+20;
    if (duty_cycle>dc_range)
    {
      duty_cycle=0;
    }
    printf("%f %f %d \n",d_array[i-1],d_array[i],i );
    ////Calculate time step
    gettimeofday(&t2, NULL);
    t_s=(t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0f;
    ////////append
    

     }

   n_samples=i/n_data;
   gpioPWM(test_magnet, 0);
   gettimeofday(&tf, NULL);
   cpu_time_used=(tf.tv_sec - t0.tv_sec) + (tf.tv_usec - t0.tv_usec) / 1000000.0f;
   mean_f=n_samples/cpu_time_used;
   mean_ts=1/mean_f;
   printf("took %f\nmean t_s is %f\n", cpu_time_used,mean_ts);
  

   printf("Data cllection ended  with %d samples\n",i );
  /////////////////////////////////     TXT FILE GENERATION
      ml=0;
      for(int m = 0; m < n_samples; m++) {
        for (int l = 0; l < n_data; l++)
        { ml++;
          fprintf(f,"%f",d_array[ml]);   
          if (l!=n_data-1){fprintf(f,", ");}
         }
          fprintf(f, "\n");}
//////////////////////////////   CLOSING
   pthread_join(thread_id, NULL);
   free(d_array);
   fclose(f);
return(0);

}


