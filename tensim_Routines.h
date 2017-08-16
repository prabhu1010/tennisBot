#ifndef TENSIM_ROUTINES_H
#define TENSIM_ROUTINES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#define NUMBER_OF_ARGS 3
#define DELAY_IN_SECS 20

void init();
void fixpositions();
void startmatch();
void usleep(int);
int CheckArguments(int, char**);

char tensim_DataFile[13];

struct list_row_node {
    int row_no;
    struct list_row_node *next;
};

struct set {
    int isTb;
    int gamesWon;
};

struct player {
    char fullname[70];
    char fname[35];
    char lname[35];
    int LBScore;
    int rank;
    struct set setscore[5];
    int gamescore;
    int setswon;
    int isAdv;
    int onServ;
    int onRerv;
    int ceil;
};

struct {
    struct player *topDog;
    struct player *underDog;
    char matchresult[130];
    char matchscore[30];
} match[127];

FILE *fptr;	
int draw_row_no[128];
int deuce, matchOver, gameOver, currentSet, nextMatchno;

struct player plr[128];
struct player *svr, *rvr, *ptWinner, *ptLoser;

#endif
