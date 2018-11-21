#include "init-from-file.h"


void deleteSpaceAndComment(char ** chaine)
{
	int i = 0, j = 0;
	char res[MAX_LINE_LENGTH];

	while( (*chaine)[i] != '\0' )
	{
		if( (*chaine)[i] == '#' )
			break;

		if( ((*chaine)[i] != ' ') && ((*chaine)[i] != '\t') && ((*chaine)[i] != '\n') ) 
			res[j++] = (*chaine)[i];
		i++;
	}

	res[j] = '\0';
	i = 0;

	while( res[i] != '\0' )
	{
		(*chaine)[i] = res[i];
		i++;
	}

	(*chaine)[i] = '\0';
}


void getArgs(char * chaine, char *** res)
{
	int i = 0;
	int j = 0;
	int a = 0;

	while( chaine[i] != '\0' )
	{
		if( chaine[i] == '=' )
		{
			(*res)[a][j] = '\0';
			a++;
			j = 0;
			i++;
		}
		else
			(*res)[a][j++] = chaine[i++];
	}

	(*res)[a][j] = '\0';
}


// return value from the config file.
int parseFile(char* file_name, char *** res)
{
	FILE * file;

	char* buffer = calloc(MAX_LINE_LENGTH, sizeof(char));

	char** temp = calloc(2, sizeof(char*));
	temp[0] = calloc(MAX_LINE_LENGTH, sizeof(char));
	temp[1] = calloc(MAX_LINE_LENGTH, sizeof(char));

	int i = 0;

	file = fopen(file_name, "r");
	
	if( file != NULL )
	{
		while( fgets(buffer, MAX_LINE_LENGTH, file) != NULL )
		{
			deleteSpaceAndComment(&buffer);
			getArgs(buffer, (&temp));
			sprintf((*res)[i], temp[0]);
			sprintf((*res)[i+1], temp[1]);
			i = i + 2;
		}

		fclose(file);
	}
	else
	{
		printf("Erreur à l'ouverture du fichier : %s\n", file_name);
		exit(1);
	}

	free(buffer);
	free(temp[0]);
	free(temp[1]);
	free(temp);

	return i;
}


arg giveArgs(char ** args, int len)
{
	arg res;
	int i = 0;
	while( i < len )
	{
		if( strcmp(args[i], "port_serveur_distant")  == 0  )
			res.port_serveur_distant = args[i+1];
		else if( strcmp(args[i], "port_serveur_local")  == 0  )
			res.port_serveur_local = args[i+1];
		else if( strcmp(args[i], "ip_serveur_distant")  == 0  )
			res.ip_serveur_distant = args[i+1];
		else if( strcmp(args[i], "interface_name") == 0 )
			res.interface_name = args[i+1];
		i += 2;
	}
	return res;
}








