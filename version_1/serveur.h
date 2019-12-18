#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

typedef struct FILE_COPY
{
    /* data */
    //son nom
    char filename;
    //son id
    int id;
    //la date de son envoie
    time_t time;
}file_copy;