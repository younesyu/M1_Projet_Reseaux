#include "iftun.h"

int tun_alloc(char *dev)
{
	struct ifreq ifr;
	int fd, err;

	if( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
	{
		perror("création tun");
		exit(1);
	}

	memset(&ifr, 0, sizeof(ifr));

	/*
	 * Flags: IFF_TUN   - TUN device (no Ethernet headers) 
	 *        IFF_TAP   - TAP device  
	 *
	 *        IFF_NO_PI - Do not provide packet information  
	 */
	ifr.ifr_flags = IFF_TUN | IFF_NO_PI; 

	if( *dev )
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);

	if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 )
	{
		close(fd);
		return err;
	}

	strcpy(dev, ifr.ifr_name);
	return fd;

	return 0;
}

int transfert (int src, int dest)
{
	char buffer[BUFFER_SIZE];
	int rep;
	while(1)
	{
		if ( (rep = read(src, buffer, BUFFER_SIZE) ) == -1 )
			return 1;
		
		printf("Lu in : %s\n", buffer);
		write(dest, buffer, rep);
	}
}










