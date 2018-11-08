#include "extremite.h"

/* taille maximale des lignes */
#define MAXLIGNE 80
#define CIAO "Au revoir ...\n"
#define BUFFER_SIZE 1024

int ext-out()
{
  int s,n; /* descripteurs de socket */
  int len,on; /* utilitaires divers */
  struct addrinfo * resol; /* résolution */
  struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
                           PF_INET,SOCK_STREAM,0, /* IP mode connecté */
                           0,NULL,NULL,NULL};
  struct sockaddr_in client; /* adresse de socket du client */
  const char * port = "123"; /* Port pour le service */
  int err; /* code d'erreur */
  
  fprintf(stderr,"Ecoute sur le port %s\n", port);
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
  if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in))<0) {
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

  while(1) {
    /* attendre et gérer indéfiniment les connexions entrantes */
    len=sizeof(struct sockaddr_in);
    if( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 ) {
      perror("accept");
      exit(7);
    }
    /* Nom réseau du client */
    char hotec[NI_MAXHOST];  char portc[NI_MAXSERV];
    err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
    if (err < 0 ){
      fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
    }else{ 
      fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
    }
    /* traitement */
    echo(n,hotec,portc);
  }
  return EXIT_SUCCESS;
}

/* echo des messages reà§us (le tout via le descripteur f) */
void echo(int f, char* hote, char* port)
{
  ssize_t lu; /* nb d'octets reà§us */
  char msg[MAXLIGNE+1]; /* tampons pour les communications */ 
  char tampon[MAXLIGNE+1]; 
  int pid = getpid(); /* pid du processus */
  int compteur=0;
  
  /* message d'accueil */
  snprintf(msg,MAXLIGNE,"Bonjour %s! (vous utilisez le port %s)\n",hote,port);
  /* envoi du message d'accueil */
  send(f,msg,strlen(msg),0);
  
  do { /* Faire echo et logguer */
    lu = recv(f,tampon,MAXLIGNE,0);
    if (lu > 0 )
      {
        compteur++;
        tampon[lu] = '\0';
        /* log */
        fprintf(stderr,"[%s:%s](%i): %3i :%s",hote,port,pid,compteur,tampon);
        snprintf(msg,MAXLIGNE,"> %s",tampon);
        /* echo vers le client */
        send(f, msg, strlen(msg),0);
      } else {
        break;
      }
  } while ( 1 );
       
  /* le correspondant a quitté */
  send(f,CIAO,strlen(CIAO),0);
  close(f);
  fprintf(stderr,"[%s:%s](%i): Terminé.\n",hote,port,pid);
}

int ext-in(char * hote, int tunfd)
{
  //TODO
  /* Trouver l'hôte */
  /*const char * hote = ""; /* nom d'hôte du  serveur */ 
  const char * port = "123"; /* port TCP du serveur */   
  char ip[NI_MAXHOST]; /* adresse IPv4 en notation pointée */
  struct addrinfo *resol; /* struct pour la résolution de nom */
  int sock; /* descripteur de socket */
  
  /* Résolution de l'hôte */
  if ( getaddrinfo(hote,port,NULL, &resol) < 0 ) {
    perror("résolution adresse");
    exit(2);
  }

  /* On extrait l'addresse IP */
  sprintf(ip,"%s",inet_ntoa(((struct sockaddr_in*)resol->ai_addr)->sin_addr));

  /* Création de la socket, de type TCP / IP */
  /* On ne considère que la première adresse renvoyée par getaddrinfo */
  if ((sock=socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol)) < 0) {
    perror("allocation de socket");
    exit(3);
  }
  fprintf(stderr,"le n° de la socket est : %i\n",s);

  /* Connexion */
  fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s\n\n",
	  hote,ip,port);
  if (connect(sock,resol->ai_addr,sizeof(struct sockaddr_in))<0) {
    perror("connexion");
    exit(4);
  }
  freeaddrinfo(resol); /* /!\ Libération mémoire */

  /* Session */
    /* Lecture du trafic du tunnel */
    char buffer[BUFFER_SIZE];
    int rep;

    while (rep = read(tunfd, buffer, BUFFER_SIZE) == -1)) {
      write(sock, buffer, rep);
    }

  /* Destruction de la socket */
  close(s);

  fprintf(stderr,"Fin de la session.\n");
  return EXIT_SUCCESS;
}
