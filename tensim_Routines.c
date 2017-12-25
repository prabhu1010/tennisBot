#include "tensim_Routines.h"

void PrintWithDelay(char *str, int delay) {
	char *cPtr = str;
	while (*cPtr) {
		printf("%c", *cPtr++);
        if (*cPtr != ' ')
		    usleep(delay * 2000);
	}
}

int GetRand(int ceiling)
{
	return (rand()%ceiling);
}

void PrintChampionshipRound(int matchno) {
    if (matchno == 64 || matchno == 96 || matchno == 112 || matchno == 120 ||
        matchno == 124 || matchno == 126 || matchno == 127) {
        int i;
        for (i = 0; i < 128; i++)
            printf("%c", '-');
        printf("\n");
    }

    if (matchno < 64)
        printf("%-15s", "Round 1  ");
    else if (matchno < 96)
        printf("%-15s", "Round 2  ");
    else if (matchno < 112)
        printf("%-15s", "Round 3  ");
    else if (matchno < 120)
        printf("%-15s", "Round 4  ");
    else if (matchno < 124)
        printf("%-15s", "Quarterfinal  ");
    else if (matchno < 126)
        printf("%-15s", "Semifinal  ");
    else
        printf("%-15s", "Final  ");

}

void fixpositions()
{
	int i, j;
	char c;
	draw_row_no[0] = 0;
	draw_row_no[127] = 1;
	fptr = fopen("draw-pos.txt", "r");
	for (i=0; i<7;i++)
	{
		int row_no, low, high, rand_val, traverse_idx, iterations;
		struct list_row_node *head = NULL, *ptr, *prev, *new_node;
		//printf("i = %d\n",i);
		fscanf(fptr,"%d,%d", &low, &high);
		iterations = high - low + 1;
		//printf("low = %d, high = %d\n",low, high);
		while ((c = fgetc(fptr)) != '\n')
		{
			fscanf(fptr, "%d",&row_no);
			//printf("We read %d\n",row_no);
			new_node = (struct list_row_node *)malloc(sizeof (struct list_row_node));
			new_node->row_no = row_no;
			new_node->next = NULL;
			
			if (head == NULL)
			{
				head = new_node;
				//printf("new node at head, value = %d\n",head->row_no);
			}
			else
			{
				for (ptr = head; ptr->next; ptr = ptr->next);
				ptr->next = new_node;
				//printf("adding at end \n");
			}
		}
		//printf("reached newline\nPrinting...");
		//for (ptr = head; ptr; ptr=ptr->next)
			//printf("%d ",ptr->row_no);
		//printf("\n");
		for (j=0; j < iterations; )
		{
			// draw_row_no - 0 = 1, 127 = 2...32/95 = 3, 32/95 = 4   ;; low=5 high=8 
			//printf("high = %d, low = %d getting random\n",high, low);
			if (iterations > 1)
			{
					rand_val = GetRand(iterations); //returns 0 to n-1, which is 3

					for (traverse_idx = 0, ptr = head ; traverse_idx !=  rand_val; traverse_idx++)
					{
						prev = ptr;
						ptr=ptr->next;
					}

					draw_row_no[ptr->row_no - 1] = low - 1;
					if (rand_val == 0)
						head = head->next;
					else if (rand_val == high-low)
						prev->next = NULL;
					else  
						prev->next = ptr->next;
					free(ptr);	
			}
			else
			{
				draw_row_no[head->row_no - 1] = low-1;
				//printf("Assigning...index %d to %d\n", head->row_no-1, low-1);
				free(head);
			}
			iterations--;
			low++;
		}
	}
	for (i=0; i<64; i++)
	{ //Player sets will be {0,1} {2,3} {4,5}
		if (plr[draw_row_no[2*i]].rank < plr[draw_row_no[2*i+1]].rank)
		{	
			match[i].topDog = &plr[draw_row_no[2*i]]; match[i].underDog = &plr[draw_row_no[2*i+1]];
		}
		else
		{	
			match[i].topDog = &plr[draw_row_no[2*i+1]]; match[i].underDog = &plr[draw_row_no[2*i]];
		}
	}
    /* PR:Verify the schedule is generated correctly */

    printf("DRAW\n");
	for (i=0; i<64; i++)
		printf("%s %s (Rank %3d) vs %s %s (Rank %3d) %16s  %c", 
			match[i].topDog->fname, match[i].topDog->lname,match[i].topDog->rank,
			match[i].underDog->fname, match[i].underDog->lname,match[i].underDog->rank,
			"|", i%2? '\n' : ' ');
    printf("\n\n");


}

void init()
{
	int i, j;
	for (i=0; i<128; i++)
	{
		memset(plr[i].fullname,0,sizeof plr[i].fullname);
		memset(plr[i].fname,0,sizeof plr[i].fname);
		memset(plr[i].lname,0,sizeof plr[i].lname);
		plr[i].rank = plr[i].setswon = plr[i].isAdv = plr[i].onServ = 0;
		for (j=0; j<5; j++)
		plr[i].setscore[j].isTb = plr[i].setscore[j].gamesWon = 0;
		if (i < 127)
		{
			match[i].topDog = match[i].underDog = NULL; 
			strcpy(match[i].matchresult, "");	
			strcpy(match[i].matchscore, "");	
		}
	}

	deuce = 0;
	/*
	memset(svr.fname,0,sizeof svr.fname);
	memset(svr.lname,0,sizeof svr.lname);
	svr.rank = svr.setswon = svr.isadv = svr.onserv = 0;
	for (j=0; j<5; j++)
	svr.setscore[j].istb = svr.setscore[j].gameswon = 0;

	memset(rvr.fname,0,sizeof rvr.fname);
	memset(rvr.lname,0,sizeof rvr.lname);
	rvr.rank = rvr.setswon = rvr.isadv = rvr.onserv = 0;
	for (j=0; j<5; j++)
	rvr.setscore[j].istb = rvr.setscore[j].gameswon = 0;
	*/
}

void startmatch(int matchno)
{
	int i, j, randVal;
	char tmpScore[31] = "";
	matchOver = gameOver = currentSet = 0;
	

	if (matchno % 2 == 0)
		nextMatchno++;
	svr = match[matchno].topDog;
	rvr = match[matchno].underDog;

	for (i=0; i<2; i++)
	{
		svr->setswon = svr->gamescore = rvr->setswon = rvr->gamescore = 0;
		for (j=0;j<5;j++)
		{
			svr->setscore[j].gamesWon = rvr->setscore[j].gamesWon = 0;
			svr->setscore[j].isTb = rvr->setscore[j].isTb = 'n';
		}
	}
	svr->ceil=65; 
	//rvr->ceil=63;
	rvr->ceil=64-(20*(svr->LBScore - rvr->LBScore)/(plr[0].LBScore-plr[127].LBScore)); //ORIG
	svr->onServ = 1; 
	while (!matchOver)
	{
		svr->gamescore = svr->isAdv = rvr->gamescore = rvr->isAdv = 0;
		gameOver = deuce = 0;
		while (!gameOver)
		{
			randVal = rand() % 100 + 1;
		ptWinner = (randVal <= svr->ceil) ? svr : rvr;
			ptLoser = (ptWinner == svr) ? rvr : svr;
			if (ptWinner->setscore[currentSet].isTb == 'n')
			{
				ptWinner->gamescore++;
				//printf("%s: %d-%d ",svr->lname,svr->gamescore,rvr->gamescore); 
				if (ptWinner->gamescore == 3 && ptLoser->gamescore == 3) //Check for first deuce
				{ 
					deuce = 1; 
					//printf("Deuce ");
				}
				if (ptWinner->gamescore > 3)
				{
					if (ptWinner->gamescore == ptLoser->gamescore) //Check for nth deuce where n=2,3,.....
					{ 
						deuce=1;
						ptWinner->isAdv = ptLoser->isAdv = 0; 
						//printf("Deuce "); 
					}
					else if (ptWinner->gamescore >=4 && (ptWinner->gamescore-ptLoser->gamescore) == 1) //Advantage
					{ 
						deuce = 0; 
						ptWinner->isAdv = 1; 
						//printf("Advantage %s ", ptWinner->fname);
					} // Do gameOver check and if so, check if the Set is over
					else if (ptWinner->gamescore >=4 && (ptWinner->gamescore-ptLoser->gamescore) >= 2)
					{ 
						gameOver = 1;
						ptWinner->setscore[currentSet].gamesWon++;
						//printf("GAME %s - %d %d\n",ptWinner->lname, ptWinner->setscore[currentSet].gamesWon,ptLoser->setscore[currentSet].gamesWon);
					
						//Game is over but we are going into a Tie Break
						if (ptWinner->setscore[currentSet].gamesWon == 6 && ptLoser->setscore[currentSet].gamesWon == 6 && (currentSet < 4))
						{
							ptWinner->setscore[currentSet].isTb = ptLoser->setscore[currentSet].isTb = 'y';
							ptWinner->setscore[currentSet].gamesWon = ptLoser->setscore[currentSet].gamesWon = 0;
							gameOver = 0;
						}
						// No Tie Break, but check if set is over
						if ((ptWinner->setscore[currentSet].gamesWon - ptLoser->setscore[currentSet].gamesWon >= 2) && ptWinner->setscore[currentSet].gamesWon >= 6)
						{
							ptWinner->setswon++;
							//printf("SET %s - %d %d\n",ptWinner->lname, ptWinner->setscore[currentSet].gamesWon,ptLoser->setscore[currentSet].gamesWon);
							if (ptWinner->setswon == 3)
							{
								matchOver = 1;
                                PrintChampionshipRound(matchno);
								if (matchno < 120) {
									printf("%-25s (Rank %3d) beat %-25s (Rank %3d)    ", ptWinner->fullname, ptWinner->rank, ptLoser->fullname, ptLoser->rank);
								}
								else
								{
									sprintf(match[matchno].matchresult, "%-25s (Rank %3d) beat %-25s (Rank %3d)    ", 
													ptWinner->fullname, ptWinner->rank, ptLoser->fullname, ptLoser->rank);
									PrintWithDelay(match[matchno].matchresult, DELAY_IN_SECS);
								}

								for (j=0;j<=currentSet;j++)
								{
								    strcpy(tmpScore, "");	
									if (ptWinner->setscore[j].isTb == 'y')
									{
										if (ptWinner->setscore[j].gamesWon > ptLoser->setscore[j].gamesWon) {
											if (matchno < 120)
												printf("7-6(%d) ",ptLoser->setscore[j].gamesWon);
											else 
												sprintf(tmpScore, "7-6(%d) ",ptLoser->setscore[j].gamesWon);
										}
										else {
											if (matchno < 120)
												printf("6-7(%d) ",ptLoser->setscore[j].gamesWon);
											else 
												sprintf(tmpScore, "6-7(%d) ",ptLoser->setscore[j].gamesWon);
										}
									}
									else {
										if (matchno < 120)
						  					printf("%d-%d ",ptWinner->setscore[j].gamesWon, ptLoser->setscore[j].gamesWon);
										else
						  					sprintf(tmpScore, "%d-%d ",ptWinner->setscore[j].gamesWon, ptLoser->setscore[j].gamesWon);
									}

									//strcat(match[matchno].matchscore, tmpScore);
									PrintWithDelay(tmpScore, DELAY_IN_SECS);
								}
								//printf("%d-%d : %d-%d", ptWinner->LBScore, ptLoser->LBScore, ptWinner->ceil, ptLoser->ceil);
								//if (ptLoser->LBScore > ptWinner->LBScore) printf(" UPSET!!! ");
								printf("\n");
								if (match[nextMatchno].topDog == NULL)
									match[nextMatchno].topDog = ptWinner;
								else
								{			
										if (match[nextMatchno].topDog->rank < ptWinner->rank)
											match[nextMatchno].underDog = ptWinner;
										else 
										{
											match[nextMatchno].underDog = match[nextMatchno].topDog; match[nextMatchno].topDog = ptWinner;
										} 
								}
								//printf("\n next is %s vs %s\n", (match[nextMatchno].topDog? match[nextMatchno].topDog->lname : "next??"), (match[nextMatchno].underDog? match[nextMatchno].underDog->lname : "next??")); 
							} 
							else
								currentSet++; //set is won but match aint over yet, goto next set
						}
						ptWinner = svr; //Do this swap when game is over, no TB or game isnt over, TB starts
						svr = rvr; 
						rvr = ptWinner; //ptWinner is temp variable for swapping, it gets overwritten anyway
					}//block for checking if game is won
				}//block when point reaches 40+
			}//block for regular set, no TB yet
			else //Tiebreaker
			{
				ptWinner->setscore[currentSet].gamesWon++;
				//printf("%s 6-6(%d-%d) ",svr->fname, svr->setscore[currentSet].gamesWon, rvr->setscore[currentSet].gamesWon);

				if ((ptWinner->setscore[currentSet].gamesWon - ptLoser->setscore[currentSet].gamesWon) >= 2 && ptWinner->setscore[currentSet].gamesWon >= 7)
				{
					gameOver = 1; //to facilitate change of serve
					//ptWinner->setscore[currentSet].gamesWon = 7;
					ptWinner->setswon++;
					//printf("\nSET %s - 7-6(%d)\n",ptWinner->lname, ptLoser->setscore[currentSet].gamesWon);
					if (ptWinner->setswon == 3)
					{
						matchOver = 1;
                        PrintChampionshipRound(matchno);
						if (matchno < 120) {
							printf("%-25s (Rank %3d) beat %-25s (Rank %3d)    ", ptWinner->fullname, ptWinner->rank, ptLoser->fullname, ptLoser->rank);
						}
						else
						{
							sprintf(match[matchno].matchresult, "%-25s (Rank %3d) beat %-25s (Rank %3d)    ", 
									ptWinner->fullname, ptWinner->rank, ptLoser->fullname, ptLoser->rank);
							PrintWithDelay(match[matchno].matchresult, DELAY_IN_SECS);
						}
						//printf("%s %s(%d) beat %s %s(%d) ", ptWinner->fname, ptWinner->lname,ptWinner->rank, ptLoser->fname, ptLoser->lname,ptLoser->rank);
						//printf("%-25s (Rank %3d) beat %-25s (Rank %3d)    ", ptWinner->fullname, ptWinner->rank, ptLoser->fullname, ptLoser->rank);
						for (j=0;j<=currentSet;j++)
						{
							strcpy(tmpScore, "");	
							if (ptWinner->setscore[j].isTb == 'y')
							{
								if (ptWinner->setscore[j].gamesWon > ptLoser->setscore[j].gamesWon) {
									if (matchno < 120)
										printf("7-6(%d) ",ptLoser->setscore[j].gamesWon);
									else 
										sprintf(tmpScore, "7-6(%d) ",ptLoser->setscore[j].gamesWon);
								}
								else {
									if (matchno < 120)
										printf("6-7(%d) ",ptLoser->setscore[j].gamesWon);
									else 
										sprintf(tmpScore, "6-7(%d) ",ptLoser->setscore[j].gamesWon);
								}
							}
							else {
								if (matchno < 120)
									printf("%d-%d ",ptWinner->setscore[j].gamesWon, ptLoser->setscore[j].gamesWon);
								else
									sprintf(tmpScore, "%d-%d ",ptWinner->setscore[j].gamesWon, ptLoser->setscore[j].gamesWon);
							}

							//strcat(match[matchno].matchscore, tmpScore);
							PrintWithDelay(tmpScore, DELAY_IN_SECS);
						}
						printf("\n");
						if (match[nextMatchno].topDog == NULL)
							match[nextMatchno].topDog = ptWinner;
						else
						{			
								if (match[nextMatchno].topDog->rank < ptWinner->rank)
									match[nextMatchno].underDog = ptWinner;
								else 
								{
									match[nextMatchno].underDog = match[nextMatchno].topDog;match[nextMatchno].topDog = ptWinner;
								} 
						}
									//printf("\n next is %s vs %s\n", (match[nextMatchno].topDog? match[nextMatchno].topDog->lname : " "), (match[nextMatchno].underDog? match[nextMatchno].underDog->lname : " ")); 
					}
					else
					{
						currentSet++;
						ptWinner = svr; //TB ends, swap server for next set
						svr = rvr; 
						rvr = ptWinner; //ptWinner is temp variable for swapping, it gets overwritten anyway
					}
				}
				else if ((ptWinner->setscore[currentSet].gamesWon + ptLoser->setscore[currentSet].gamesWon) % 2 == 0)
				{   //TB going on, swap server every 2 points
					ptWinner = svr; 
					svr = rvr; 
					rvr = ptWinner; //ptWinner is temp variable for swapping, it gets overwritten anyway
				}
			}
		}
	}
}
