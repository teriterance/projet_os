#include "client.h"


int main(int argc, char * argv[]){
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrClient;
    
    addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrClient.sin_family = AF_INET;
    addrClient.sin_port = htons(6543);

    connect(socketClient, (const struct sockaddr*)&addrClient, sizeof(addrClient));
    printf("connete \n");

    file_copy fichier;
    recv(socketClient, &fichier, sizeof(file_copy), 0);
    printf("%s %d %ld", fichier.filename, fichier.id, fichier.time);

    close(socketClient);

    return EXIT_SUCCESS;
}