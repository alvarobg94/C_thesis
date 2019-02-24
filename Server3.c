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
#define PORT 8080

/////////////////GLOBAL VARIABLES
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

//////////////////////////////////////////////// MAIN
int main() {

//////////////////////////////////////////////////////// VARIABLES
struct sockaddr_in address; 
long long i,t_delay=9e7;
int k,j,n_samples,n_data_t,n_data_r,n_data_s,SIZE_DATA,ml,server_fd, socket_r,socket_s, valread,opt,addrlen;
i=k=0; j=1;n_data_t=8;n_data_r=4;n_data_s=3; SIZE_DATA=n_data_t*1000;opt=1;addrlen=sizeof(address);
int data_rec_r[4]={0,0,0,0};int data_send_r[8]={0,0,0,0,0,0,0,0};int close_vector_r[8]={-1,-1,-1,-1,-1,-1,-1,-1};
int data_rec_s[3]={0,0,0};int data_send_s[4]={0,0,0,0}; int close_vector_s[4]={-1,-1,-1,-1};
double mean_f,cpu_time_used,t_s,tnext,id;
mean_f=t_s=cpu_time_used=id=tnext=0.0;
unsigned long long t_s_micro=0;
unsigned m1r,m1l,m2r,m2l,m3r,m3l,m4r,m4l; //pwm
struct timeval t0,t1,t2,tf;


// thread creation
pthread_t thread_id; 
pthread_create(&thread_id, NULL, UserGUI, NULL);

// dynamic array creatiom
double* d_array;
d_array =calloc(SIZE_DATA*j,sizeof(double));
if(d_array == NULL) {
	printf("malloc of d_array failed!\n");   
	exit(1);} 
// data file creation
FILE *f = fopen("data.txt", "w");
if (f == NULL){
	 printf("Error opening file!\n");
     exit(1);}

//server creation
server_fd = socket(AF_INET, SOCK_STREAM,0);
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
setsockopt(server_fd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag,sizeof(int)); 
address.sin_family = AF_INET; 
address.sin_addr.s_addr =INADDR_ANY; //ADDRESS of the host here
address.sin_port = htons( PORT );      // Port defined up in the beggining 8080



/////////////////////////////////////////////////////////////// PROGRAM

//////////////////BIND THE SERVER
bind(server_fd, (struct sockaddr *)&address, sizeof(address));
printf("%s\n","binded" );
listen(server_fd, 3);
//////////////// CONNECTION WITH CLIENT
socket_s = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
socket_r = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
if (socket_r>=0){printf("%s\n","connected to the stator" );}
if (socket_s>=0){printf("%s\n","connected to the rotor" );}

data_send_r[0]=0;data_send_r[1]=1000;data_send_r[2]=500;data_send_r[3]=1500;data_send_r[4]=250;data_send_r[5]=1250;data_send_r[6]=750;data_send_r[7]=1750;
data_send_s[0]=0;data_send_s[1]=1000;data_send_s[2]=500;data_send_s[3]=1500;


   /////////////////////////////////LOOP 
gettimeofday(&t0, NULL);
while(flag==1) {

  i++;
  ////TIME APPENDING
  gettimeofday(&t1, NULL);
  d_array[i] = t_s;
  /////// RECEIVING DATA FROM CLIENTS
  valread = read(socket_s , data_rec_s, 40);
  valread = read(socket_r , data_rec_r, 40);
  ////////DATA APPENDING
  for (int b = 0; b < n_data_t-1; b++)
  {
    i++;
    if (b<n_data_r){d_array[i] = data_rec_r[b];}
    if (b>=n_data_r){d_array[i]=data_rec_s[b-4];}
  }
  k++;
  /////////ENLARGING THE DATA VECTOR FOR SAVING MORE DATA

  if (k+1==SIZE_DATA/n_data_t){
	  j++;
	  d_array= realloc(d_array, (SIZE_DATA*j) * sizeof(double));
    k=0;}

  ///////////////// CALCULATE ACTUATION 
  ////////////////// IMPLEMENT HERE ANY CONTROL ALGORITHM
  for (int mag = 0; mag < 8; mag++)
{ 
  
  data_send_r[mag]=data_send_r[mag]+1;

  if (data_send_r[mag]>2000)
  {
    data_send_r[mag]=0;
  }}
 for (int  mag = 0; mag < 4; mag++)
{ 
  
  data_send_s[mag]=data_send_s[mag]+1;

  if (data_send_s[mag]>2000)
  {
    data_send_s[mag]=0;
  }}


  /////////////////SENDING ACTUATION
  send(socket_s , data_send_s , sizeof(data_send_s) , 0 );
  send(socket_r , data_send_r , sizeof(data_send_r) , 0 );
  /////////////TIME CALCULATION
  gettimeofday(&t2, NULL);
  t_s=(t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0f;
  tnext=tnext+t_s;
  }

  //// END OF LOOP
 gettimeofday(&tf, NULL);

////////// END CLIENTS
valread = read( socket_s , data_rec_s, 1024);
valread = read( socket_r , data_rec_s, 1024);
send(socket_s , close_vector_s , sizeof(close_vector_s) , 0 );
send(socket_r , close_vector_r , sizeof(close_vector_r) , 0 );
////// TIME CALCULATION
cpu_time_used = (tf.tv_sec - t0.tv_sec) + (tf.tv_usec - t0.tv_usec) / 1000000.0f;
n_samples=i/n_data_t;
mean_f=n_samples/cpu_time_used;
printf("done in %f \n",cpu_time_used);
printf("mean frequency = %f\n",mean_f);
printf("mean time_step = %f\n",1/mean_f);

/////// DATA PROCESSING


printf("Data cllection ended  with %d samples\n",n_samples );
ml=0;
for(int m = 0; m < n_samples; m++) {
  for (int l = 0; l < n_data_t; l++)
  { ml++;
    fprintf(f,"%f",d_array[ml]);   
    if (l!=n_data_t-1){fprintf(f,", ");}
   }
    fprintf(f, "\n");}
////////////////////////////////////////// CLOSING EVERYTHING
pthread_join(thread_id, NULL); 
free(d_array);
fclose(f);
printf("Program ended");
return(0);
}