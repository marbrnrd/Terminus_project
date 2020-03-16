#include <stdio.h> 
#include <unistd.h>

int rm (int argc,char **argv) {
  
  printf("aaaaaaaaa\n");
  fflush(stdout);

  if(argc!=2){
    printf("Please, introduce the correct parameters\n");
  }
  else{ 
    int exist;
    exist=unlink(argv[1]); 
    if(exist==0){ 
      printf("File deleted");
    }
    else{ 
      perror("unlink");
    }
  }

  return 0;
}
