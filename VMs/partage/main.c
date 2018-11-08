#include "iftun.h"
#include <stdio.h>

int main(int argc, char ** argv)
{
	 int tunfd;
  printf("Creation de %s\n",argv[1]);
  tunfd = tun_alloc(argv[1]);
  printf("Faire la configuration de %s...\n",argv[1]);
  printf("Appuyez sur une touche pour continuer\n");
  getchar();
  getchar();
  printf("Interface %s Configuree:\n",argv[1]);
  system("ip addr");
  printf("Appuyez sur une touche pour terminer\n");
  getchar();

	return 0;
}
