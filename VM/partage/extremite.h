#ifndef ___EXTREMITE___
#define ___EXTREMITE___

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

#include "iftun.h"

#define MAXLIGNE 2048

void ext_out(char* port, int tunfd);
void ext_in(char * hote, char* port, int tunfd);
void echo(int n, int tunfd);

#endif