#ifndef ___INIT_FROM_FILE___
#define ___INIT_FROM_FILE___

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 500
#define MAX_ARGS 40


typedef struct{
	char * port_serveur_local;
	char * port_serveur_distant;
	char * ip_serveur_distant;
	char * interface_name;
} arg;

void deleteSpaceAndComment(char **);
void getArgs(char *, char ***);

// return value from the config file.
int parseFile(char*, char ***);


arg giveArgs(char **, int);


#endif
