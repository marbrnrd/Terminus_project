#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFSIZE 2000
#define path "/users/alumnos/acaf/acaf0201/Terminus_project/mans/"

int main(int argc, char *argv[]){

int fd;
int n;
char buffer[BUFSIZE];
char a[100]=path;

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
}
