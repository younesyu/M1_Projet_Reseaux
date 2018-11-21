#include "iftun.h"
#include "extremite.h"
#include <stdio.h>

#define MAX_LENGHT_READ 300

typedef struct {
    char tun[MAX_LENGHT_READ];
    char inport[MAX_LENGHT_READ];
    char outip[MAX_LENGHT_READ];
    char outport[MAX_LENGHT_READ];
} config;

config loadConfiguration(char *fichier) {

    config configurationLoaded;


    FILE * file = fopen(fichier, "r");

    if ( file == NULL )
    {
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }

    char buffer[MAX_LENGHT_READ];
    int indiceBuffer = 0;

    while ( fgets(buffer, MAX_LENGHT_READ, file) != NULL ) {
        if (buffer[0] != '#') {
            char arg[MAX_LENGHT_READ];
            int indiceArg = 0;
            char value[MAX_LENGHT_READ];
            int indiceValue = 0;
            int partie1 = 1;
            while ( buffer[indiceBuffer] != '\0' ) {
                if (buffer[indiceBuffer] != ' ' && buffer[indiceBuffer] != '\n' && buffer[indiceBuffer] != '\t') {
                    if (buffer[indiceBuffer] == '=') partie1 = 0;
                    else {
                        if (partie1 == 1) {
                            arg[indiceArg++] = buffer[indiceBuffer];
                        }
                        else {
                            value[indiceValue++] = buffer[indiceBuffer];
                        }
                    }
                }
                indiceBuffer++;
            }

            indiceBuffer=0;

            arg[indiceArg] = '\0';
            value[indiceValue-1] = '\0';
            printf("Valeur lue %s (=) %s et\n\n", arg, value);

            if ( strcmp(arg, "tun")  == 0  ) {
                strcpy(configurationLoaded.tun, value);
            }
            else if ( strcmp(arg, "inport")  == 0  ) {
                strcpy(configurationLoaded.inport, value);
            }
            else if ( strcmp(arg, "outip")  == 0  ) {
                strcpy(configurationLoaded.outip, value);
            }
            else if ( strcmp(arg, "outport") == 0 ) {
                strcpy(configurationLoaded.outport, value);
            }
        }
    }

    return configurationLoaded;
}

int main(int argc, char ** argv)
{

    if (argc-1 != 1) {
        printf("Usage : %s <fichier_config>\n", argv[0]);
        exit(1);
    }

    int tunfd;
    char tunName[MAX_LENGHT_READ];
    char inport[MAX_LENGHT_READ];
    char outip[MAX_LENGHT_READ];
    char outport[MAX_LENGHT_READ];

    config argumentFichier = loadConfiguration(argv[1]);

    strcpy(tunName, argumentFichier.tun);
    strcpy(inport, argumentFichier.inport);
    strcpy(outip, argumentFichier.outip);
    strcpy(outport, argumentFichier.outport);

    printf("Lecture fichier configuration ...\n");
    printf("Arguments lue : \nTunel Name : %sNON\nIn port : %sOUI\n\tOut Ip : %s\nOut port : %s\n", argumentFichier.tun, inport, outip, outport);

    printf("Création de %s\n",tunName);
    tunfd = tun_alloc(tunName);

    printf("Faire la configuration de %s...\n",tunName);

    if (fork() != 0)
    {
        ext_out(outport, tunfd);
    }
    else
    {
        sleep(1);
        ext_in (outip, outport, tunfd);
    }
	
    return 0;
}
