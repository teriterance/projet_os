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
#include <dirent.h>
#include <arpa/inet.h>

typedef struct FILE_COPY
{
    /* data */
    //son nom
    char filename[50];
    //son id
    int id;
    //la date de son envoie
    time_t time;
}file_copy;

//declaration d'un type pour des messages courts
typedef enum short_m {OUI, NON} short_m;