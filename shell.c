#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

#include "builtin.c"

#define len 500


char *printpromt(){

	char a[len],b[len],c[len];
	char *d = (char *)malloc(len*sizeof(char));
	getcwd(a,len);//directory path
	gethostname(b,len);
    	getlogin_r(c,len);
	snprintf(d,len,"%s@%s:~%s$ ", c,b,a);
	return d;

}

void recordhistory(char *a,char **l){

	history_expand(a, l);// for !n or !-n
	add_history(*l);//for getting history using up/down arrow

}

int parsecommand(char *l,char **args){
	
	int count = 0;
	char *a = strtok(l," ");

	while(a!=NULL){

		args[count++] = (char *)malloc(strlen(a)*sizeof(char));
		strcpy(args[count-1],a);
		a = strtok(NULL," ");

	}	
	return count;

}

int main(int argc, char const *argv[])
{

	char a[len];
	int pid,stat;

	while(1){

		char *p = printpromt();

		char *l = readline(p);//reading command
		free(p);

		char* args[50] = {NULL};

		if(l==NULL)
			continue;

		strcpy(a,l);
		recordhistory(a,&l);

		//printf("%s\n",l );
		int nofc = parsecommand(l,args);//command parsing

		if(nofc==0)
			continue;

		//for background process
		if(strcmp(args[nofc-1],"&")==0){
			pid = fork();
			if(pid<0){
				perror("Fork Error: Process couldn't be created\n");
			}
			if(pid==0){
				args[nofc-1] = NULL;
				execvp(*args,args);
				exit(-1);
			}
			waitpid(-1,&stat, WNOHANG);
			
			//printf("hello\n");
			continue;
		}

		int check = isbuiltin(args[0]);


		if(check>=0){
			runbuiltin(check,args);
		}
		else{

			pid = fork();
			if(pid<0){
				perror("Fork Error: Process couldn't be created\n");
			}
			if(pid==0){
				execvp(*args,args);
				system(l);
				exit(-1);
			}
			else{
				wait(NULL);
			}
		}
	}
	return 0;
}
