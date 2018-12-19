#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 8080
int main(){
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	int numbers[2]={10,20};
	char *hello = "Hello from server"; 
	server_fd = socket(AF_INET, SOCK_STREAM,0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	listen(server_fd, 3);
	new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
	if (new_socket>=0)
	{
		printf("%s\n","connected" );
	}
	valread = read( new_socket , buffer, 1024); 
	printf("%s\n",buffer );
	send(new_socket , numbers , sizeof(numbers) , 0 );
	printf("Hello message sent\n"); 
return 0;  
}