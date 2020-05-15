#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h> 
#define MAX 4096 
#define PORT 2048
#define SA struct sockaddr 

void func(int sockfd) 
{ 
    char buff[MAX]; 
    char *str;
    int n;
    ssize_t byteread;
	
   
	
    	while((byteread = recv(sockfd,buff,sizeof(buff),0)) > 0){
		write(1,buff,byteread);
	}
		
	byteread=read(0,buff,sizeof(buff));
	send(sockfd,buff,byteread,0);
   
} 

int main() 
{ 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 6); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
 

    func(sockfd);

  
    // close the socket 
    close(sockfd); 
} 
