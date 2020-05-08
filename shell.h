#ifndef ECHO_H
#define ECHO_H

int read_args(int* argcp, char* args[], int max, int* eofp);
void removeDirectoryTree();
void sigint_handler(int sig);
void location_desc(char* location);
int execute(int argc, char *argv[]);
int main_shell();

#endif
