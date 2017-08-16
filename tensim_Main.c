#include "tensim_Routines.h"


int main(int argc, char *argv[])
{
	int i, j;
	char c;

	CheckArguments(argc, argv);
	puts(tensim_DataFile);

	init();
	srand((unsigned)time(NULL));
	nextMatchno = 63;
	fptr = fopen(tensim_DataFile, "r");

	for (i=0; i<128; i++)
	{
		j=0;
		while ((c=fgetc(fptr)) != ' ') 
			plr[i].fname[j++] = c;
		plr[i].fname[j] = '\0';

		j=0;
		while ((c=fgetc(fptr)) != ',') 
			plr[i].lname[j++] = c;
		plr[i].lname[j] = '\0';
        sprintf(plr[i].fullname,"%s %s",plr[i].fname, plr[i].lname);
		fscanf(fptr, "%d",&plr[i].LBScore);

		while ((c=fgetc(fptr)) != '\n');
		plr[i].rank = i+1;
	}
	fclose(fptr);

	fixpositions();

	//for (i=0; i<127; i++)	 ORIG
	for (i=0; i<127; i++)	 
		startmatch(i);

	exit(0);
}
