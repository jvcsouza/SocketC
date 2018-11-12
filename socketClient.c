#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include "helpers.c"
#include "socket.c"

#define BUFFER_SIZE 300

SOCKET conexao;
struct sockaddr_in server_address;
char mensagem[BUFFER_SIZE];
int controlePessoas = 0;

typedef struct {
	char convidado[0];
	char nome[100];
	char prof[0];
	char email[100];
	char defic[0];
} pessoa;

void registrarPessoa();
void pegarAcento(pessoa p){
	system("cls");
	char **infos = split(getResposta(), '/');
	printf("\n\t\t__________________________________________");
	printf("\n\t\t|                                        |");
	printf("\n\t\t|        TICKET %s EMITIDO            |", infos[2]);
	printf("\n\t\t|                                        |");
	printf("\n\t\t|                                        |");
	printf("\n\t\t|  EMITIDO PARA: %s.", infos[0]);
	int lenght = strlen(infos[0]) + 17;
	for(lenght; lenght < 40; lenght++) printf(" "); printf("|");
	printf("\n\t\t|  ACENTO: %s.                          |", infos[1]);
	printf("\n\t\t|  DATA: %s", infos[3]);
	printf("\t\t|                                        |");
	printf("\n\t\t|________________________________________|");
	pause(4);
}

void registrarPessoa() {
	// consultaAcentos();
	pessoa p;
	strcpy(mensagem, "REGISTRA ");
	system("cls");
	printf("\n\t\tPALETRA DE DIREITOS HUMANOS UNIP!\n\n");
	printf("\n\tEMITIR TICKET DE ENTRADA\n");
	printf("\n\tNOME COMPLETO: ");
	gets(p.nome);
	strcat(mensagem, p.nome);
	strcat(mensagem, "/");
	printf("\tEMAIL: ");
	gets(p.email);
	strcat(mensagem, p.email);
	strcat(mensagem, "/");
	printf("\tPROFESSOR ? (S / N): ");
	gets(p.prof);
	strcat(mensagem, p.prof);
	strcat(mensagem, "/");
	printf("\tCONVIDADO ? (S / N): ");
	gets(p.convidado);
	strcat(mensagem, p.convidado);
	strcat(mensagem, "/");
	printf("\tPOSSUI ALGUM TIPO DE DEFICIENCIA ? (S / N): ");
	gets(p.defic);
	strcat(mensagem, p.defic);
	printf("\n\tGERANDO TICKET..."); pause(1);
	printf("\n\tOBTENDO CADEIRA..."); pause(0.5);
	printf("\n\n%s", mensagem);
	setMsg(mensagem);
	pegarAcento(p);
}

void consultaAcentos(bool *acentos){
	strcpy(mensagem, "CONSULTA-ACENTOS");
	// transmite();
}

int main(int argc, char *argv[]) {
	registra_winsock();
	bool acentos = true;
	while(acentos){
		//consultaAcentos(&acentos); // RETORNA BOOL 
		registrarPessoa();
	}
}

