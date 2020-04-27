#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include "echo.h"

int main(int argc, char **argv)
{
    // Print mode.
    // If one parameter is passed,
    // print the contents of the file.
    if (argc == 2)
    {
        int fd = open(argv[1], O_RDONLY);
        if (fd == -1)
            err(EXIT_FAILURE, "open()");

        echo(fd, STDOUT_FILENO);

        close(fd);
    }

    // Copy mode.
    // If two parameters are passed,
    // copy the first file to the second file.
    else if (argc == 3)
    {
        int fdr = open(argv[1], O_RDONLY);
        if (fdr == -1)
            err(EXIT_FAILURE, "open()");

        int fdw = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fdw == -1)
        {
            close(fdr);
            err(EXIT_FAILURE, "open()");
        }

        echo(fdr, fdw);

        close(fdw);
        close(fdr);
    }

    // Echo mode.
    // Otherwise, copy the standard input to the standard output.
    else
        echo(STDIN_FILENO, STDOUT_FILENO);

    return EXIT_SUCCESS;
}
