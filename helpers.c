#include <time.h>

void pause (float seg) {
   if (seg < 0.001) return;
   float inst1 = 0, inst2 = 0;
   inst1 = (float) clock() / (float) CLOCKS_PER_SEC;
   while (inst2 - inst1 < seg) inst2 = (float) clock() / (float) CLOCKS_PER_SEC;
   return;
}

char **split(char frase[], char separador){
    int i, j, k, contsep = 0;
    for(i = 0, contsep = 0; i < strlen(frase); i++)
    	if(frase[i] == separador)
        	contsep++;
    char  aux[contsep][20];
    char **result = (char**)malloc(contsep*sizeof(char));
    if(contsep){
        for(i = 0; i <= contsep; i++)
        	*(result + i) = (char*)malloc(40*sizeof(char));
        for(i=0,k=0,j=0; i < strlen(frase); i++)
           	if(frase[i] != separador){
                aux[k][j] = frase[i];
                j++;
           	}else{
            	aux[k][j] = 0;
            	k++;
            	j=0;
           	}
        aux[k][j] = 0;
        for(i=0;i<=contsep;i++)
        	*(result+i) = strcpy(*(result+i), aux[i]);
	   	return result;
    }else
        printf("Nenhum Separador Encontrado");
}

