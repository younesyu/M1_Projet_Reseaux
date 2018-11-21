#include "extremite.h"

void ext_out(char* port, int tunfd)
{
    int s,n; /* descripteurs de socket */
    int len,on; /* utilitaires divers */
    struct addrinfo * resol; /* résolution */
    struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
        PF_INET6,SOCK_STREAM,0, /* IP mode connecté */
        0,NULL,NULL,NULL
    };
    struct sockaddr_in6 client; /* adresse de socket du client */

    int err; /* code d'erreur */

    err = getaddrinfo(NULL,port,&indic,&resol);
    if (err < 0 ) {
        fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
        exit(2);
    }

    /* Création de la socket, de type TCP / IP */
    if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0) {
        perror("allocation de socket");
        exit(3);
    }
    fprintf(stderr,"le n° de la socket est : %i\n",s);

    /* On rend le port réutilisable rapidement /!\ */
    on = 1;
    if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0) {
        perror("option socket");
        exit(4);
    }
    fprintf(stderr,"Option(s) OK!\n");

    /* Association de la socket s à  l'adresse obtenue par résolution */
    if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
        perror("bind");
        exit(5);
    }
    freeaddrinfo(resol); /* /!\ Libération mémoire */
    fprintf(stderr,"bind!\n");

    /* la socket est prête à  recevoir */
    if (listen(s,SOMAXCONN)<0) {
        perror("listen");
        exit(6);
    }
    fprintf(stderr,"listen!\n");

    while (1) {
        /* attendre et gérer indéfiniment les connexions entrantes */
        len=sizeof(struct sockaddr_in6);
        if ( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 ) {
            perror("accept");
            exit(7);
        }

        printf("AAAAAAAAAAA Nouvelle connexion de ext-out AAAAAAAAAAAAAA\n");

        /* traitement */
        if ( fork() == 0 )
        {
            echo(n, tunfd);
            break;
        }
    }
// return EXIT_SUCCESS;
}

/* echo des messages reà§us (le tout via le descripteur f) */
void echo(int n, int tunfd)
{
    ssize_t lu; /* nb d'octets reçus */
    char tampon[MAXLIGNE+1];
    printf("Echo OK\n");
    while (1)
    {
        lu = read(n, tampon, MAXLIGNE);
        if (lu > 0 )
        {
            tampon[lu] = '\0';
            printf("Lu out : %s\n", tampon);
            write( tunfd, tampon, lu );
        }
        else
            break;
    }

    /* le correspondant a quitté */
    close(n);
}

void ext_in(char * hote, char* port, int tunfd)
{
    struct addrinfo *resol; /* struct pour la résolution de nom */
    int sock; /* descripteur de socket */

    struct addrinfo hints = {AI_PASSIVE, 			 /* Toute interface */
        PF_INET6, SOCK_STREAM,0, /* IP mode connecté */
        0,NULL,NULL,NULL
    };

    /* Résolution de l'hôte */
    if ( getaddrinfo(hote,port,&hints, &resol) < 0 ) {
        perror("résolution adresse");
        exit(2);
    }

    /* Création de la socket, de type TCP / IP */
    /* On ne considère que la première adresse renvoyée par getaddrinfo */
    if ((sock=socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol)) < 0) {
        perror("allocation de socket");
        exit(3);
    }

    while (connect(sock,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
        printf("Erreur lors de la connexion, tentative de reconnexion\n");
        sleep(1);
    }
    freeaddrinfo(resol); /* /!\ Libération mémoire */

    transfert(tunfd, sock);

    /* Destruction de la socket */
    close(sock);

    fprintf(stderr,"Fin de la session.\n");
// return EXIT_SUCCESS;
}
