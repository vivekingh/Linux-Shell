void endit(){
	exit(-1);
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

void runbuiltin(int c,char **args){

	switch(c){

		case 0:
			chdir(args[1]);
			break;

		case 1:
			break;

		case 2:
			endit();
			break;

		case 3:
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
