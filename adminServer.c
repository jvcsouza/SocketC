#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket.c"
#include "helpers.c"

#define BUFFER_SIZE 300

char mensagem[BUFFER_SIZE];
typedef struct {
	char convidado[0];
	char nome[100];
	char prof[0];
	char email[100];
	char defic[0];
} pessoa;

void mostrarOps(){
    char op[3];
    system("cls");
    printf("\n\t\t%s\n", "ADMINISTRACAO DO SERVIDOR");
    printf("\n\n\t1) %s", "Opcao 1");
    printf("\n\t2) %s", "Opcao 2");
    printf("\n\t3) %s", "Opcao 3");
    printf("\n\n\t%s", "Escolha uma opcao: ");
    scanf("%s", &op);
    switch(op[0]){
        case '1':
            printf("Opcao 1");
            break;
        case '2':
            printf("Opcao 2");
            break;
        case '3':
            printf("Opcao 3");
            break;
        default:
            printf("Opcao Invalida!");
    }
}

int main(){
    registra_winsock();
    mostrarOps();
    system("pause");
}