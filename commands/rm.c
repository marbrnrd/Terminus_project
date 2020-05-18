#include <stdio.h> 
#include <unistd.h>

int main(int argc,char **argv) {

  if(argc!=2){
    printf("Please, introduce the correct parameters\n");
  }
  else{ 
    int exist;
    exist=unlink(argv[1]); 
    if(exist==0){ 
      printf("File deleted\n");
    }
    else{ 
      perror("unlink");
    }
  }

  return 0;
}
