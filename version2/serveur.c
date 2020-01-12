#include "serveur.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char * argv[]){
    //ecrant d'aceuil
    printf("   _____                       \n  / ____|                                \n | (___   ___ _ ____   _____ _   _ _ __  \n  \\___ \\ / _ \\ '__\\ \\ / / _ \\ | | | '__| \n  ____) |  __/ |   \\ V /  __/ |_| | |    \n |_____/ \\___|_|    \\_/ \\___|\\__,_|_|\n");
    //creation des sockets
    int socket_RV, socket_service;
    struct sockaddr_in adr, adresse;
    socklen_t lgadresse=sizeof(struct sockaddr_in);
    if ((socket_RV=socket(AF_INET, SOCK_STREAM, 0)) ==-1){
        perror("socket rendez-vous");
        exit(1);
    }

    //initiallisation de l'addresse de connection 
    adr.sin_family=AF_INET;
    adr.sin_port=htons(6543);
    adr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //on fait une liaison pour que notre serveur soit le seul autorise a effectuer une tache sur l'addresse 
    if (bind(socket_RV, (struct sockaddr *) &adr, sizeof(adr))==-1){
    perror("bind");
    exit(1);
    }

    //on ecoute et on attend une connection 
    if (listen(socket_RV,1)==-1){
        perror("listen");
        exit(1);
    }
    
    //si la connection est correct on l'accepte 
    socket_service=accept(socket_RV, (struct sockaddr *)&adresse, &lgadresse);
    if(socket_service == -1){
        printf("echec de connection");
        exit(1);
    }
    
    //on recois le nom d'utilisateur
    char user_name[50];
    recv(socket_service, user_name, 50*sizeof(char), 0);
    printf("connection de l'utilisateur %s\n", user_name);

    while (1)
    {
        struct dirent *lecture2;
        DIR *rep2;
        rep2 = opendir(user_name);
        while ((lecture2 = readdir(rep2))) {
            send(socket_service, lecture2->d_name, 256*sizeof(char),0);
        }
        char ficher_tmp[256];

        /* boulce infine de focntionnement du programme  */
        short_m choix;
        recv(socket_service, &choix, sizeof(short_m), 0);

        //on attend la structure du fichier a recevoir
        file_copy struct_fichier;
        recv(socket_service, &struct_fichier, sizeof(file_copy), 0);

        if (choix == OUI){
            /*lecture d'un fichier depuis le serveu*/
            char file_loc[50];
            strcpy(file_loc, user_name);
            strcat(file_loc, "/");
            strcat(file_loc, struct_fichier.filename);
            printf("ouverteure du fichier %s/n", file_loc);
            
            /**/
            FILE *file_out = fopen(file_loc, "r");
            if(file_out !=  NULL){
                short_m n = OUI;
                send(socket_service, &n , sizeof(short_m), 0);
            }
            else{
                short_m n = NON;
                send(socket_service, &n, sizeof(short_m), 0);
            }

            //on lit le fichier et on garde sa taille
            fseek(file_out,  0L, SEEK_END);
            unsigned int  sz = ftell(file_out);
            //retour du curseur au debut 
            fseek(file_out,  0L, SEEK_SET);
            //on envoie la taille en question 
            send(socket_service, &sz, sizeof(unsigned int), 0);
            //envoi du fichier en question
            char caractere;
            do{
                caractere = fgetc(file_out);
                send(socket_service, &caractere, sizeof(char), 0);
            }while (caractere != EOF);
            fclose(file_out);

            printf("fin de l'envoi du fichier");
        }
        else{
            /* ecriture d'un fichier depuis le client */
            if (choix ==  NON){
                char file_loc[50];
                strcpy(file_loc, user_name);
                strcat(file_loc, "/");
                strcat(file_loc, struct_fichier.filename);
                printf("ouverteure du fichier %s\n", file_loc);
                //crationd e ;
                char timeString[9];

                //rejout de l'heure sur le nom du fichier 
                strftime(timeString, sizeof(timeString), "%H:%M:%S", gmtime(&struct_fichier.time));
                strcat(file_loc, "_");
                strcat(file_loc, timeString);

                //on creai le dossier contenant les ellements du dossier utilisateur
                char folder_loc[50];
                strcat(folder_loc, "./");
                strcpy(folder_loc, user_name);

                struct stat st = {0};
                if(stat(folder_loc, &st)){
                mkdir(folder_loc, 0777);
                }
                /**On ouvre le fichier**/
                FILE *file_in = fopen(file_loc, "a+");
                if(file_in !=  NULL){
                    short_m n = OUI;
                    send(socket_service, &n , sizeof(short_m), 0);
                }
                else{
                    short_m n = NON;
                    send(socket_service, &n, sizeof(short_m), 0); 
                }
                
                //on recois la taille de notre fichier 
                unsigned int taille_fichier;
                recv(socket_service, &taille_fichier, sizeof(unsigned int), 0);
                printf("voici la taille du fichir a recevoir%d\n",taille_fichier);

                //on recois notre message
                //on ouvre le fichier en question (en ecriture)
                char caractere;
                do{
                    recv(socket_service, &caractere, sizeof(char), 0);
                    printf("%c", caractere);
                    if(caractere != EOF)
                    fprintf(file_in,"%c",caractere);
                }while(caractere != EOF);

                //on a fini l'ecriture donc on ferme le fichier
                fclose(file_in);
                printf("la copie du fichier depuis le serveur s'est bien deroule");

                
                //supression en cas de doublon de meme version 
                struct dirent *lecture;
                DIR *rep;
                rep = opendir(user_name);
                while ((lecture = readdir(rep))) {
                    char ficher_compar[50];
                    strcpy(ficher_compar, user_name);
                    strcat(ficher_compar, "/");
                    strcat(ficher_compar, lecture->d_name);
                    printf("\n%s %s\n", ficher_compar, file_loc);
                    if(strcmp(ficher_compar, file_loc) != 0){
                        if (compareFile(fopen(file_loc, "r"), fopen(ficher_compar, "r")) == 0){
                            printf("je suprime un doublon");
                            remove(file_loc);
                            break;
                        }
                    }
                }
                
            }else
            {
                /* code */
                close(socket_service);
            }
            
        }
    }
    close(socket_RV);

    return EXIT_SUCCESS;
}