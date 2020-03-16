
#include <fcntl.h>
#include<sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<dirent.h>
#include<error.h>
#include <stdlib.h>

  
int cat(char* file)
{    
  ssize_t byteread;
  
  fflush(stdout);

  int pagesize = getpagesize();
  int fd = open(file, O_RDONLY);
  if(fd < 0){
        fprintf(stderr,"%s: %s\n", "COULD NOT OPEN", file);
        return -1;
  }

  char* buffer = (char*)malloc(pagesize);
 
  while((byteread = read(fd, buffer, pagesize)) > 0){
 	 write(1,buffer,byteread);
  }
  if(byteread < 0){
	fprintf(stderr,"Error while reading\n");
	return -1;
  }
  close(fd);
  return 0;
}   


