#include <stdio.h>
#include <unistd.h>

void main (int argc, char **argv){

  if(argc!= 3){
     printf("Please, introduce the correct parameters\n");
  }
  else{
     int w = rename(argv[1], argv[2]);
     if(w==0){
        printf("The %s is moved to %s\n", argv[1], argv[2]);
     }
     else{
        perror("The following error ocurred: ");
     }
  }
}
