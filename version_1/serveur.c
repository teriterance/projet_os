#include "serveur.h"

int main(int argc, char * argv[]){
    int socket_RV, socket_service;
    struct sockaddr_in adr, adresse;
    socklen_t lgadresse=sizeof(struct sockaddr_in);
    if ((socket_RV=socket(AF_INET, SOCK_STREAM, 0)) ==-1){
        perror("socket rendez-vous");
        exit(1);
    }
    adr.sin_family=AF_INET;
    adr.sin_port=htons(6543);
    adr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(socket_RV, (struct sockaddr *) &adr, sizeof(adr))==-1){
    perror("bind");
    exit(1);
    }

    if (listen(socket_RV,1)==-1){
        perror("listen");
        exit(1);
    }
  
    socket_service=accept(socket_RV, (struct sockaddr *)&adresse, &lgadresse);
    close(socket_RV);

    file_copy file;
    file.filename = "tt.txt";
    file.id = 0;
    file.time = 0;
    send(socket_service, &file, sizeof(file_copy), 0);
    printf("send");
    
    close(socket_service);
    close(socket_RV);

    return EXIT_SUCCESS;
}