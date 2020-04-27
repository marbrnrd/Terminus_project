#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <err.h>
#include "echo.h"
#include <signal.h>

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage:\n"
            "Arg 1 = Port number (e.g. 2048)");

    //addrinfo struct
    struct addrinfo hints;

    struct addrinfo *result;
    int addrinfo_error;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    //Get info
    addrinfo_error = getaddrinfo(NULL, argv[1], &hints, &result);

    //Error
    if (addrinfo_error != 0){
	errx(EXIT_FAILURE, "Fail getting addrinfo");
    }

    //result to a linked list
    //Try to connect
    struct addrinfo *rp;
    int cnx;

    int opt = 1;

    for(rp = result; rp != NULL; rp= rp->ai_next)
    {
	cnx = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

	if (cnx == -1)
		continue;

	setsockopt(cnx, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(cnx,rp->ai_addr, rp->ai_addrlen) == 0){
		break;
	}
	close(cnx);
    }

    freeaddrinfo(result);

    if (rp == NULL){
	errx(EXIT_FAILURE, "No socket has been opened");
    }

     if (listen(cnx ,5) == -1)
           errx(EXIT_FAILURE,"Listen failed");

     printf("Waiting for connections...\n");

     while(1){

     	int new_socket = accept(cnx, rp->ai_addr, &rp->ai_addrlen);

     	if( new_socket == -1)
	     	errx(EXIT_FAILURE, "Accept failed\n");

	pid_t pid = fork();

	if(pid == -1)
		errx(EXIT_FAILURE, "Fork failed");

	else if(pid > 0){
		close(new_socket);
		continue;
	}

	else{

		close(cnx);

		printf("New connection (pid = %i)\n", getpid());
		
		char buffer[2096];
		memset(&buffer,0,2096);

		int w = write(new_socket, buffer, 2096);

		if (w == -1)
			errx(EXIT_FAILURE, "Write failed\n");
		if (w != 0)
			echo(new_socket, new_socket);

		printf("Close connection (pid = %i)\n", getpid());
		signal(SIGCHLD, SIG_IGN);
		close(new_socket);
		exit(EXIT_SUCCESS);
	}
    }

     close(cnx);

     return EXIT_SUCCESS;
}
