
char intToChar (int i) {
    return (i + 48);
}

void taillentete_in (char * buffer, int tailleRep) {
    char offsetted_buffer[BUFFER_SIZE];
    
    offsetted_buffer[0] = intToChar((tailleRep % 100) / 10);
    offsetted_buffer[1] = intToChar(tailleRep % 10);
    
    // for(int i = 0; i < tailleRep; i++) 
    //     offsetted_buffer[i + 2] = buffer[i];
    // Very experimental!!!!!
    strcpy(offsetted_buffer + 2, buffer);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    strcpy(buffer, offsetted_buffer);


}

int tailleentete_out (int src, char * buffer) {
    int lu, tailleEntete;
    char enteteStr[2];

    lu = read(src, enteteStr, 2);

    tailleEntete = atoi(enteteStr[0]) * 10 + atoi(enteteStr[1]);

    return tailleEntete;
}