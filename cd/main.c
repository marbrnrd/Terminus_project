#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <err.h>

#include "cd.h"

int main (int argc, char **argv){
	if (argc > 2)
		errx(EXIT_FAILURE, "Request failed");
	return cd_main(argc, argv);
}
