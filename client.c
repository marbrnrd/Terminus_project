#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>

#define MAX 9096
#define PORT 2048
#define SA struct sockaddr 

void check(int err)
{
    if(err < 0)
    {
       perror("client: ");
       exit(1);
    }
}


void func(int sockfd) 
{ 
    char buff[MAX]; 
    ssize_t byteread;

    sleep(2);

    check(byteread = recv(sockfd,buff,sizeof(buff),0));
    check(write(1,buff,byteread));


    while(1)
    {

	bzero(buff, MAX);
        check(byteread=read(0,buff,sizeof(buff)));
        check(send(sockfd, buff, byteread, 0));

        sleep(1);

        bzero(buff, MAX);
        check(byteread = recv(sockfd,buff,sizeof(buff),0));
        check(write(1,buff,byteread));
        
    }
  
} 

int main(int argc, char* argv[]) 
{ 
    int sockfd;
    struct sockaddr_in servaddr; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM,0); 
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
    return 0;
} 
