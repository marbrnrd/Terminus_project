#ifndef ECHO_H
#define ECHO_H

int read_line(char line[], int* eofp);
int parse_command(char* cmdp,int* argcp, char* args[], int max);
int parse_pipe(char line[], char** pipedcmd);
int parse_redirection(int cmdnum, char* cmd[],char* files[] ,char* mem[]);
void removeDirectoryTree();
void sigint_handler(int sig);
void sigint_ignore(int sig);
void location_desc(char* location);
int builtin_handler(char** cmds);
int execute(char* line);
int execute_piped(int cmdnum,char** cmds,char* rfiles[]);
int main_shell();

#endif
