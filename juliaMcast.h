#ifndef juliaMcast_h__
#define juliaMcast_h__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for usleep() */
#include <sys/socket.h>
#include <netdb.h>
#include "msock.h"

extern int advertise();
extern int testlib();
extern int discover();
extern int advertisePresence(char *, char *);
extern int discoverNeighbors(char *, char *);

#endif

