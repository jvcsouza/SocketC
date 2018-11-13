#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "socket.c"
#include "helpers.c"

#define BUFFER_SIZE 300
#define ARQUIVO "PLATEIA.txt"

char mensagem[BUFFER_SIZE];
typedef struct {
	char convidado[0];
	char nome[100];
	char prof[0];
	char email[100];
	char defic[0];
} pessoa;

void consultarAcentos(){
    strcpy(mensagem, "CONSULTA-ACENTOS ");
    printf("\n\n%s", mensagem);
    setMsg(mensagem);
    char **dados = split(getResposta(), '/');
    system("cls");
    printf("\n\t ** ACENTOS DISPONIVEIS **\n\t");
    printf("\n\n\tDEFICIENTES: %s.\n\t", dados[2]);
    printf("\n\tPROFESSORES E CONVIDADOS: %s.\n\t", dados[1]);
    printf("\n\tDEMAIS: %s.\n\t", dados[0]);
    pause(3);
}

void abrirTxt(){
	char cmd[20] = "start ";
	strcat(cmd, ARQUIVO);
	system(cmd);
}

void mostrarOps(){
    char op[3];
    system("cls");
    printf("\n\n\t\t%s\n", "ADMINISTRACAO DO SERVIDOR");
    printf("\n\n\t1) %s", "CONSULTAR ACENTOS DISPONIVEIS.");
    printf("\n\t2) %s", "ABRIR RELACAO DE ENTRADAS");
    printf("\n\t3) %s", "FINALIZAR PALESTRA E REALIZAR SORTEIO.");
    printf("\n\n\t%s", "Escolha uma opcao: ");
    scanf("%s", &op);
    switch(op[0]){
        case '1': consultarAcentos(); break;
        case '2': abrirTxt(); break;
        case '3': printf("Opcao 3"); break;
        default: printf("Opcao Invalida!");
    }
}

int main(){
    registra_winsock();
    while(true)
        mostrarOps();
    system("pause");
}
