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

void ext_out(char*, int);
void ext_out_handler(int, int);
void ext_in(char*, char*, int);

#endif
