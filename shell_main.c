#include "command_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#define MAX_LINE_LENGTH 512
 
 
void sigintHandler(int sig)
{   
printf("Exited with control-c\n");
}
void handle_sigchld(int sig)
{   
int saved_errno = errno;
int status;
while(waitpid(-1,&status,WNOHANG)>0){
//display reason why child terminated
   if(WIFEXITED(status)
   {   
      printf("child exited with status %d\n",WEXITSTATUS(status));
   }
   else if(WIFSIGNALED(status))
   {   
      printf("child exited due to signal %d\n",WTERMSIG(status));
   }
}
errno = saved_errno;

}
void externalCommandBackground(char * arguments[])
{   
   int pid = fork();
   if (pid < 0)
      perror("error in fork");
   
   if (pid == 0)
      {   
         // child
         execvp(arguments[0],arguments);
      }
}

void externalCommandForeground(char * arguments[])
{
   int pid = fork();
   if (pid < 0)
      perror("error in fork");
   if (pid == 0)
   {
      // child 
      execvp(arguments[0],arguments);   
   }
   else
   {
      //parent
      int status;
      int result = waitpid(pid,&status,0);
      if(result<0)
      perror("result less than 0");
   }
   }
  
int main(int argc, const char **argv)
{
   char cmdline[MAX_LINE_LENGTH];
   struct CommandLine command; 
   signal(SIGINT, sigintHandler);
   struct sigaction sa;
   sa.sa_handler = &handle_sigchld;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_RESTART;
   if(sigaction(SIGCHLD,&sa,0)==-1)
   {
       perror(0);
       exit(1);
   }
   for (;;)
   {
       printf("> ");
       fgets(cmdline, MAX_LINE_LENGTH, stdin);
       if (feof(stdin))
           exit(0);

       bool gotLine = parseLine(&command, cmdline);
       if (gotLine)
       {
           if(strcmp(command.arguments[0],"q")==0)
               exit(1);
           if(strcmp(command.arguments[0],"cd")==0)
           {
               if(command.argCount>1)
              {
                   if(chdir(command.arguments[1])!=0)
                       perror("Failed changing directory");
                   chdir(command.arguments[1]);
               }
               else
               {
                   char* directory = getenv("HOME");
                   if(chdir(directory)!=0)
                       perror("Failed changing to home directory");
               }
           }
           else
           {
               if(command.background)
                   externalCommandBackground(command.arguments);
               else
                   externalCommandForeground(command.arguments);
           }
           freeCommand(&command);
       }
   }
   }
