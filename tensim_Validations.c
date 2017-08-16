#include "tensim_Routines.h"

void StringToUpperCase(char *string) {
	while (*string) {
		if (islower(*string))  
			*string = *string - 32;
			string++;
	}
}

void StringToLowerCase(char *string) {
	while (*string) {
		if (isupper(*string))  
			*string = *string + 32;
			string++;
	}
}

int IsCorrectYear(char *strYear) {
	if ( (strcmp(strYear, "1990") == 0) ||
		 (strcmp(strYear, "2000") == 0)) {
		 	return 1;
	}
	return 0;
}

int IsCorrectTitle(char *strTitle) {
	if ( (strcmp(strTitle, "AUS") == 0) ||
		 (strcmp(strTitle, "FRE") == 0) ||
		 (strcmp(strTitle, "WIM") == 0) ||
		 (strcmp(strTitle, "US") == 0)) {
			 return 1;
	}
	return 0;
}

int IsCorrectGender(char *strGender) {
	if ( (strcmp(strGender, "M") == 0) ||
		 (strcmp(strGender, "W") == 0)) {
			 return 1;
	}
	return 0;
}

void FixDataFile(char *title, char *decade) {

    //StringToLowerCase(title);
	if (strcmp(title, "US") == 0)
		sprintf(tensim_DataFile, "%sO%s.%s", title, decade, "data");
	else
		sprintf(tensim_DataFile, "%s%s.%s", title, decade, "data");
}

int ValidArguments(char *one, char *two) {
	if (IsCorrectYear(one)) {
		if (IsCorrectTitle(two)) {
			FixDataFile(two, one);
			return 1;	
		}
	}
	if (IsCorrectYear(two)) {
		if (IsCorrectTitle(one)) {
			FixDataFile(one, two);
			return 1;	
		}
	}

	return 0;
}

int CheckArguments(int nArgc, char **pArgv) {

	if (nArgc == NUMBER_OF_ARGS) {
		int i;
		for (i = 1; i <= NUMBER_OF_ARGS - 1 ; i++) {
			StringToUpperCase(pArgv[i]);
		}
	}

	if ((nArgc != NUMBER_OF_ARGS) || (ValidArguments(pArgv[1], pArgv[2]) == 0))
	{
		printf("Usage: tennisBot 1990|2000 aus|fre|wim|us (OR)\n");
		printf("       tennisBot aus|fre|wim|us 1990|2000\n");
		exit(0);
		return 0;
	}

	return 0;
}
