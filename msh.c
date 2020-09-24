//Author: Kevin Chawla

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 5     //shell only supports five arguments

int main()
{

  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
  int k,showpid[15],s,count_pid=0;
    char*history[15];
    for(k=0;k<15;k++)//Allocate memory for the history array to store user entered commands
    {
    history[k]=(char*)malloc(1024);
    memset(history[k],0,1024);
    }
    
  while( 1 )
  {
    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    static int count=0;
    strncpy(history[count++],cmd_str,strlen(cmd_str));
    if(count==15) {count=0;}
    
    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    int   token_count = 0;                                 
                                                           
    // Pointer to point to the token
    // parsed by strsep
    char *arg_ptr;                                         
                                                           
    char *working_str  = strdup( cmd_str );                

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (arg_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) && 
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

    //Following code is if else structure used to implement the built-in commands
      if(!strncmp(&cmd_str[0],"!",1))
    {
    int command_count=atoi(&cmd_str[1]);
    strcpy(cmd_str,history[command_count]);
    //token[0]=history[command_count]; 
    } 
 
   
    if(token[0]==NULL) //If the user inputs nothing then the code just goes to next line
    {
    }
    else if(strcmp(token[0],"history")==0) //if entered command is history then
    {
    for(k=0;k<count;k++)             //loop is called to print the history array
    {                                // which has the last 15 commands stored
    if(history[k]!=NULL)   
    {
    printf("%d:",k);
    printf(history[k]);
    }
    //else {printf("");} //code was throwing an error for else implementation
    }
     if(strncmp(&cmd_str[0],"!",1))
    {
    int command_count=atoi(&cmd_str[1]);
    strcpy(cmd_str,history[command_count]);
    //token[0]=history[command_count]; 
    }
    }
    else if(strcmp(token[0],"cd")==0) // Change directory function called witht the
    {                                 // specified directory  
    chdir(token[1]);
    }
    else if(strcmp(token[0],"exit")==0 || strcmp(token[0],"quit")==0)
    {
    exit(0);   //Function exit called to exit with 0 when user types exit or quit
    }
    else if(strcmp(token[0],"showpids")==0)
    {
    for(s=0;s<count_pid;s++) //printing the array showpid that stores all the forked pids
    {
 
    printf("%d:%d\n",s,showpid[s]);
    }
    }
    /*else if(!strncmp(&cmd_str[0],"!",1))
    {
    int command_count=atoi(&cmd_str[1]);
    strcpy(cmd_str,history[command_count]);
    token[0]=history[command_count]; 
    }*/
    else
    {
    pid_t pid = fork( );
    showpid[count_pid++]=pid; //store the pid values in the array
    if( pid == 0 )   //In the child
    {
    int ret = execvp(token[0], &token[0]); //call exec to implement the user entered command
    if( ret == -1 ) //if the entered command is not found in directories 
    { printf(token[0]);
      printf(" :Command not found\n");
      exit(0);
    }
    
    }
    else 
    {
    int status;
    wait( & status ); //Wait for the child to finish
    
    }
   free( working_root );
  }
  }
  return 0;
}
