#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

#define BUFFER_SIZE 512

void echo(int fd_in, int fd_out)
{
	char buffer [512];
	int r;
	int w;

	memset(&buffer,0,512);

	while(1){
		r = read(fd_in,buffer, 512);

		if (r == -1)
			errx(EXIT_FAILURE, "Read failed");
		if(r == 0)
			break;

		while (r > 0){
			int offset = 0;
			w = write(fd_out, buffer + offset,r);
			
			if (w == -1)
				errx(EXIT_FAILURE, "Write failed");

			offset += w;
			r -= w;
		}
	}
}
