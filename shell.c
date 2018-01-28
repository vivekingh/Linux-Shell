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



#define len 500
#define arlen 50
#define STATUS_TER 0
#define STATUS_RUN 1
#define STATUS_DONE 2



typedef struct bgprocess{
	int status;
	int pd;
	char cmd[len];
}bgprocess;

int size = 0;

#include "builtin.c"




void init(bgprocess bg[],int length){
	int i = 0;
	for(i=0;i<length;i++){
		bg[i].status = -1;
		bg[i].pd = -1;
		bg[i].cmd[0] = '\0';
	}
}

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

	char a[len],b[len],e[len];
	int pid,stat,bckjob = 0;
	bgprocess bg[arlen];
	init(bg,arlen);

	signal(SIGINT, SIG_IGN);//ignoring ctrl+c

	while(1){

		here:

		bckjob = size==0?size:bckjob;

		char *p = printpromt();

		char *l = readline(p);//reading command

		strcpy(b,l);

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
				exit(-1);
			}

			if(pid==0){
				args[nofc-1] = NULL;
				execvp(*args,args);
				exit(-1);
			}
	
			waitpid(-1,&stat, WNOHANG);

			if(bckjob+1<arlen){

				printf("[%d] %d\n",++bckjob,pid);
				size++;
				bg[bckjob].status = STATUS_RUN;
				bg[bckjob].pd = pid;
				strcpy(bg[bckjob].cmd,b);

			}
			
			continue;
		}



		int check = isbuiltin(args[0]);


		if(check>=0){
			runbuiltin(check,args,nofc,bg);
		}
		else{

			pid = fork();
			if(pid<0){
				perror("Fork Error: Process couldn't be created\n");
			}
			if(pid==0){

				if(strcmp(args[0]+strlen(args[0])-2,".c")==0){
					strcpy(e,"gcc ");
					strcat(e,args[0]);
					system(e);
					strcpy(e,"./a.out ");
					strcat(e,args[1]);
					strcat(e," ");
					strcat(e,args[2]);
					system(e);
					exit(-1);
				}

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