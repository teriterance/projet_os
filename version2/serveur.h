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
#include <time.h>

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
typedef enum short_m {OUI, NON, SOR} short_m;

int compareFile(FILE * fPtr1, FILE * fPtr2)
{
    char ch1, ch2;
    do
    {
        // Input character from both files
        ch1 = fgetc(fPtr1);
        ch2 = fgetc(fPtr2);

        // If characters are not same then return -1
        if (ch1 != ch2)
            return -1;

    } while (ch1 != EOF && ch2 != EOF);

    if(ch1 != ch2)
        return -1;

    return 0;
}