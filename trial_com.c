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
int k,j,n_samples,n_data,SIZE_DATA,ml,server_fd, new_socket, valread,opt,addrlen;
i=k=0; j=1;n_data=3; SIZE_DATA=n_data*1000;opt=1;addrlen=sizeof(address);int data_rec[2]={0,0};
double mean_f,cpu_time_used,t_s,id;
mean_f=t_s=cpu_time_used=id=0.0;double data_send[2]={0.0,0.0};double close_vector[2]={-1.0,-1.0};
unsigned long long t_s_micro=0;
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
address.sin_family = AF_INET; 
address.sin_addr.s_addr =INADDR_ANY; //ADDRESS of the host here
address.sin_port = htons( PORT );      // Port defined up in the beggining 8080



/////////////////////////////////////////////////////////////// PROGRAM

//////////////////BIND THE SERVER
bind(server_fd, (struct sockaddr *)&address, sizeof(address));
printf("%s\n","binded" );
listen(server_fd, 3);
//////////////// CONNECTION WITH CLIENT
new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
if (new_socket>=0){printf("%s\n","connected to the client" );}

/////////////////////////////////LOOP 
gettimeofday(&t0, NULL);
while(flag==1) {
  i++;
  ////TIME APPENDING
  gettimeofday(&t1, NULL);
  d_array[i] = t_s;
  /////// RECEIVING DATA FROM CLIENTS
  valread = read( new_socket , data_rec, 1024);
  ////////DATA APPENDING
  for (int b = 0; b < n_data-1; b++)
  {
    i++;
    d_array[i] = data_rec[b];
  }
  k++;
  /////////ENLARGING THE DATA VECTOR FOR SAVING MORE DATA
  if (k==SIZE_DATA/n_data){
	  j++;
	  d_array= realloc(d_array, (SIZE_DATA*j) * sizeof(double));
    k=0;}
  /////////////////SENDING ACTUATION
  send(new_socket , data_send , sizeof(data_send) , 0 );
  /////////////TIME CALCULATION
  gettimeofday(&t2, NULL);
  t_s=(t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0f;
  }
  //// END OF LOOP
 gettimeofday(&tf, NULL);

////////// END CLIENTS
valread = read( new_socket , data_rec, 1024);
send(new_socket , close_vector , sizeof(close_vector) , 0 );
////// TIME CALCULATION
cpu_time_used = (tf.tv_sec - t0.tv_sec) + (tf.tv_usec - t0.tv_usec) / 1000000.0f;
n_samples=i/n_data;
mean_f=n_samples/cpu_time_used;
printf("done in %f \n",cpu_time_used);
printf("mean frequency = %f\n",mean_f);
printf("mean time_step = %f\n",1/mean_f);

/////// DATA PROCESSING
n_samples=i/n_data;

printf("Data cllection ended  with %d samples\n",n_samples );
ml=0;
for(int m = 0; m < n_samples; m++) {
  for (int l = 0; l < n_data; l++)
  { ml++;
    fprintf(f,"%f",d_array[ml]);   
    if (l!=n_data-1){fprintf(f,", ");}
   }
    fprintf(f, "\n");}
////////////////////////////////////////// CLOSING EVERYTHING
pthread_join(thread_id, NULL); 
free(d_array);
fclose(f);
printf("Program ended");
return(0);
}