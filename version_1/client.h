#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

//introduction d'une macro de cast 
#define h_addr h_addr_list[0]

typedef struct FILE_COPY
{
    /* data */
    //son nom
    char* filename;
    //son id
    int id;
    //la date de son envoie
    time_t time;
}file_copy;