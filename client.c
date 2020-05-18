#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>

#define MAX 9128
#define SA struct sockaddr 


/* 1 - Function to check if there are execution errors.if the int returned by the function to check
       is negative the error is reported and execution stops.

       Input:  int returned by the function to check correct completion of.
       Output: Error information defined by errno is printed and program exited if input value < 0.
**********************************************************************************************************/
void check(int err)
{
    if(err < 0)
    {
       perror("client: ");
       exit(1);
    }
}

/* 2 - Checks if a given string is a number. Used to check correct port inputs.
************************************************************************************************************/
int isNumeric(const char *str) 
{
    while(*str != '\0')
    {
        if(*str < '0' || *str > '9')
            return 1;
        str++;
    }
    return 0;
}


/* 3 - This function handles the interactions of the client with the server. First the client receives the
       initial data from the server(the game intro text) and then enters the following loop: issue command
       to the server, wait until server responds and receive message. Repeat.
************************************************************************************************************/
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

/* 4 - Main function. Establishes the connection between client/server.

       Input: The ip address of the server to connect to (argv[1]) and the port number (argv[2])
********************************************************************************************************/
int main(int argc, char* argv[]) 
{ 
    int sockfd;
    long arg;
    struct sockaddr_in servaddr; 

    if(argc != 3)
       errx(EXIT_FAILURE, "Usage:\n"
            "Arg 1 = destination IP address (e.g. 127.0.0.1)\n"
            "Arg 2 = Port number (e.g. 2048)");  


    if (isNumeric(argv[2]) == 1) {
       errx(EXIT_FAILURE, "Usage:\n"  "Invalid port number\n"); 
    }
  
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
    servaddr.sin_addr.s_addr = inet_addr(argv[1]); 
    servaddr.sin_port = htons(atoi(argv[2]));

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
