#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>


#define BUFSIZE 2000

int main(int argc, char *argv[]){
if(argc==2){
int fd;
int n;
char buffer[BUFSIZE];
char *a;

a=getenv("ROOT");
strcat(a, "/mans/");
strcat(a, argv[1]);

fd=open(a, O_RDONLY);
if (fd>0)
{
while((n=read(fd, buffer, BUFSIZE))>0)
	printf("%s\n", buffer);
}
else
{
printf("No existing command. Try with another one.\n");
}
}else{
printf("Error with the man command.Insert valid number of parameters.\n");
}
}
