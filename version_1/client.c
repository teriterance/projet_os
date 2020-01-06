#include "client.h"


int main(int argc, char * argv[]){
    //affichage du message de bienvenu
    printf("    _____ _      _            _\n   / ____| |    (_)          | |  \n  | |    | |     _  ___ _ __ | |_ \n  | |    | |    | |/ _ \\ '_ \\| __|\n  | |____| |____| |  __/ | | | |_ \n   \\_____|______|_|\\___|_| |_|\\__|\n ");

    //on lance la procedure de connection 
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrClient;
    
    //addresse de connection , et port de connection 
    addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrClient.sin_family = AF_INET;
    addrClient.sin_port = htons(6543);

    //essay de connection au serveur
    if(connect(socketClient, (const struct sockaddr*)&addrClient, sizeof(addrClient)) == -1){
        //si il y a echec de connection 
        printf("Connection imposible\n");
        return errno;
    }
    printf("Connection reussi sur le serveur a l'addresse 127.0.0.1 sur le port 6543 \n");

    //on envoie notre nom au serveur
    //premierement on recupere le nom de l'utilisateur
    char nom[50];
    printf("nous alons vous connecter veuillez renseigner votre nom d'utilisateur \n");
    scanf("%s", nom);
    send(socketClient, nom, 50*sizeof(char), 0);
    
    while (1)
    {
        /* le fonctionement interne du client */
        //il attend l'ordre
        char choix[5];
        printf("Que voulez vous faire \n   Rest- restaurer un fichier\n   Lire- Lire un fichier du serveur\n");
        scanf("%s", choix);

        if(strcmp(choix, "Rest") == 0){
            //on signal a la socket notre choix
            short_m choix = OUI;
            send(socketClient, &choix, sizeof(short_m), 0);

            //on fait une structure de fichier
            file_copy fichier;
            printf("entrer le nom de votre fichier\n");
            scanf("%s", fichier.filename);
            printf("%s\n", fichier.filename);

            //on lui affecte un id par defaut 
            fichier.id = 0;
            //on lui affecte un temps qui est celui de la machine
            fichier.time = 0;
            //on envoie la structure en question
            send(socketClient, &fichier, sizeof(file_copy), 0);

            //on attend la reponse du serveur qui est soit 0 oui ce sera possible, soit 1 non impossible
            short_m reponce;
            recv(socketClient, &reponce, sizeof(short_m), 0);
            if(reponce == NON){
                //le repertoire de contient pas le fichier, le serveur a trouve une erreur 
                printf("Impossible de trouver ce fichier dans votre repertoire sur le serveur");
                //////////
                exit(1);
            }

            //on recois la taille de notre fichier 
            long taille_fichier;
            recv(socketClient, &taille_fichier, sizeof(long), 0);

            //on recois notre message
            char * contenu_fichier[taille_fichier];
            recv(socketClient, contenu_fichier, taille_fichier*sizeof(char), 0);

            //on ouvre le fichier en question (en ecriture)
            FILE *file_int = fopen(fichier.filename, "w");
            fwrite(contenu_fichier,sizeof(char),sizeof(contenu_fichier),file_int);
            //on a fini l'ecriture donc on ferme le fichier
            fclose(file_int);
            printf("la copie du fichier depuis le serveur s'est bien deroule");
        }
        else{
            if(strcmp(choix, "Lire") == 0){
                //on signal a la socket notre choix
                short_m choix = NON;
                send(socketClient, &choix, sizeof(file_copy), 0);

                //on fait une structure de fichier
                file_copy fichier;
                //on recois la structure en question
                scanf("entrer le nom de votre fichier %s \n",fichier.filename);
                //on lui affecte un id par defaut 
                fichier.id = 0;
                //on lui affecte un temps qui est celui de la machine
                fichier.time = 0;
                send(socketClient, &fichier, sizeof(file_copy), 0);

                //on attend la reponse du serveur qui est soit 0 oui ce sera possible, soit 1 non impossible
                short_m reponce;
                recv(socketClient, &reponce, sizeof(short_m), 0);
                if(reponce == NON){
                    //le repertoire de contient pas le fichier, le serveur a trouve une erreur 
                    printf("Impossible d'ecrire ce fichier dans votre repertoire sur le serveur");
                    break;
                }   
                //on recois la taille du fichier 
                unsigned int f_taille;
                recv(socketClient, &f_taille,sizeof(unsigned int), 0);

                //on ouvre le fichier en question (en ecriture)
                FILE *file_int = fopen(fichier.filename, "r");
                int tmp = 0 ;
                do{   
                    //on recois le fichier caractere par caractere
                    char caractere;
                    caractere = fgetc(file_int);
                    send(socketClient, &caractere, sizeof(char), 0);
                } while ( ++tmp <= f_taille );
                //on a fini l'ecriture donc on ferme le fichier
                fclose(file_int);
                printf("la copie du fichier depuis le serveur s'est bien deroule");
            }
        }
        
    }
    close(socketClient);
    
    return EXIT_SUCCESS;
}