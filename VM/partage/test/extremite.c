#include "extremite.h"

void ext_out (char* port, int tun)
{
	int sock_serveur, sock_client; /* descripteurs de socket */
	int len, on; 				   /* utilitaires divers */
	struct addrinfo * resol;	   /* résolution */

	struct addrinfo indic = {AI_PASSIVE,            /* Toute interface */
							PF_INET6,SOCK_STREAM,0, /* IP mode connecté */
							0,NULL,NULL,NULL};
	struct sockaddr_in6 client; /* adresse de socket du client */
	int err;                    /* code d'erreur */

	err = getaddrinfo(NULL, port, &indic, &resol); 

	if (err < 0)
	{
		fprintf(stderr, "Résolution: %s\n", gai_strerror(err));
		exit(2);
	}

	/* Création de la socket, de type TCP / IP */
	if ((sock_serveur = socket(resol->ai_family, resol->ai_socktype, resol->ai_protocol)) < 0)
	{
		perror("allocation de socket");
		exit(3);
	}

	fprintf(stderr, "le n° de la socket est : %i\n", sock_serveur);

	/* On rend le port réutilisable rapidement /!\ */
	on = 1;
	if (setsockopt(sock_serveur, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		perror("option socket");
		exit(4);
	}
	
	fprintf(stderr, "Option(s) OK!\n");

	/* Association de la socket s à l'adresse obtenue par résolution */
	if (bind(sock_serveur, resol->ai_addr, sizeof(struct sockaddr_in6)) < 0)
	{
		perror("bind");
		exit(5);
	}

	freeaddrinfo(resol); /* /!\ Libération mémoire */
	fprintf(stderr,"bind!\n");

	/* la socket est prête à recevoir */
	if ( listen(sock_serveur, SOMAXCONN ) < 0)
	{
		perror("listen");
		exit(6);
	}

	fprintf(stderr,"listen!\n");
	while(1)
	{
		/* attendre et gérer indéfiniment les connexions entrantes */
		len = sizeof(struct sockaddr_in6);
		if( (sock_client = accept(sock_serveur, (struct sockaddr *) &client, (socklen_t*) &len)) < 0 ) 
		{
			perror("accept");
			exit(7);
		}

		printf("Nouveau client de ext-out\n");

		/* traitement */
		if( fork() == 0 )
		{
			ext_out_handler(sock_client, tun);
			break;
		}
	}
}

void ext_out_handler(int sock_client, int tun)
{
	ssize_t lu; /* nb d'octets reçus */
	char tampon[MAXLIGNE+1]; 
	printf("Handler OK\n");
	while (1)
	{
		lu = read(sock_client, tampon, MAXLIGNE);
		if (lu > 0 )
		{
			tampon[lu] = '\0';
			printf("Lu out : %s\n", tampon);
			write( tun, tampon, lu );
		}
		else 
			break;
	}

	/* le correspondant a quitté */
	close(sock_client);
}


void ext_in(char* ip, char* port, int tun)
{
	struct addrinfo *resol; /* struct pour la résolution de nom */
	int sock;            	/* descripteur de socket */

	struct addrinfo indic = {AI_PASSIVE, 			 /* Toute interface */
							PF_INET6, SOCK_STREAM,0, /* IP mode connecté */
							0,NULL,NULL,NULL};

	/* Résolution de l'hôte */
	if ( getaddrinfo(ip, port, &indic, &resol) < 0 )
	{
		perror("résolution adresse");
		exit(2);
	}

	/* Création de la socket, de type TCP / IP */
	if ((sock = socket(resol->ai_family, resol->ai_socktype, resol->ai_protocol)) < 0) 
	{
		perror("allocation de socket");
		exit(3);
	}

	while (connect(sock, resol->ai_addr, sizeof(struct sockaddr_in6)) < 0)
	{
		printf("Erreur de connexion, Reconnexion...\n");
		sleep(1);
	}

	freeaddrinfo(resol); /* /!\ Libération mémoire */


	transfert(tun, sock);

	close(sock);
}

















