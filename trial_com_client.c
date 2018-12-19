#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#define PORT 8080


int flag=1;
int main() {

// VARIABLES
struct sockaddr_in address, serv_addr; 
struct timeval t0,t1,t2,tf;
int i,k,j,n_samples,n_data,SIZE_DATA,ml,server_fd, new_socket, valread,sock,addrlen,nano_to_micro;
i=k=0; j=1; SIZE_DATA=3000;n_data=3;sock=0;nano_to_micro=1e3;addrlen=sizeof(address);int data_send[2]={10,20};
double mean_f,cpu_time_used,t_s,id;
mean_f=t_s=cpu_time_used=id=0.0;double data_rec[2]={0.0,0.0};double close_vector[2]={-1.0,-1.0};
long long t_delay=1e1*nano_to_micro;
sock = socket(AF_INET, SOCK_STREAM, 0);
memset(&serv_addr, '0', sizeof(serv_addr));
serv_addr.sin_family = AF_INET; 
serv_addr.sin_port = htons(PORT); 
inet_pton(AF_INET, "192.168.0.20", &serv_addr.sin_addr); 
// PROGRAM
connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));



gettimeofday(&t0, NULL);
while(flag==1) {
  gettimeofday(&t1, NULL);
  send(sock , data_send , sizeof(data_send) , 0 );
  i++;
  //nanosleep((const struct timespec[]){{0, t_delay}}, NULL);
  valread = read(sock , data_rec, 1024);
  if (data_rec==close_vector)
  {
    flag=0;
  }
  gettimeofday(&t2, NULL);
  t_s=(t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0f;
}


gettimeofday(&tf, NULL);
cpu_time_used =(tf.tv_sec - t0.tv_sec) + (tf.tv_usec - t0.tv_usec) / 1000000.0f;
printf("Total time is %f",cpu_time_used);
return(0);
}