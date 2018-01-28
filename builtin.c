void endit(){
	exit(-1);
}

void resetbg(bgprocess bg[],int x){
	bg[x].status = -1;
	bg[x].pd = -1;
	strcpy(bg[x].cmd,"\0");
}

int isbuiltin(char *a){
	if(strcmp(a,"cd")==0)
		return 0;
	if(strcmp(a,"jobs")==0)
		return 1;
	if(strcmp(a,"exit")==0)
		return 2;
	if(strcmp(a,"kill")==0)
		return 3;
	if(strcmp(a,"history")==0)
		return 4;
	return -1;
}

void runbuiltin(int c,char **args,int nofc,bgprocess bg[]){

	int x = 0;
	switch(c){

		case 0:
			chdir(args[1]);
			break;

		case 1:
			for(x=1;x<arlen;x++){
				if(bg[x].status==STATUS_DONE){
					printf("[%d]+  Done\t\t%s\n",x, bg[x].cmd);
					resetbg(bg,x);
				}
				else if(bg[x].status==STATUS_TER){
					printf("[%d]+  Ter\t\t%s\n",x, bg[x].cmd);
					resetbg(bg,x);
				}
				else if(bg[x].status==STATUS_RUN){
					if(kill(bg[x].pd,0)<0){
							bg[x].status = STATUS_DONE;
							printf("[%d]+  Done\t\t%s\n",x, bg[x].cmd);
							resetbg(bg,x);
					}
					else{
						printf("[%d]+  Run\t\t%s\n", x,bg[x].cmd);
					}
					//printf("hello");
				}

			}
			break;

		case 2:
			endit();
			break;

		case 3:
			;
			int x = atoi(args[nofc-1]);
			if(x>0){
				if(x>arlen-1){
					printf("Incorrect Entry\n");
				}
				else{
					if(bg[x].status==-1 || bg[x].status==STATUS_DONE || bg[x].status==STATUS_TER){
						printf("Process doen't exist\n");
					}
					else{
						if(kill(bg[x].pd,0)<0){
							bg[x].status = STATUS_DONE;
							printf("Process has already completed\n");
						}
						else{
							kill(bg[x].pd,SIGKILL);
							bg[x].status = STATUS_TER;
						}
						size--;
					}
				}
			}
			break;

		case 4:
			;
			register HIST_ENTRY **the_list;
 			register int i;
 			the_list = history_list ();
          	if (the_list)
            for (i = 0; the_list[i]; i++)
              printf ("%d: %s\n", i + history_base, the_list[i]->line);
			break;

	}

}
