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
char **dados;
int acntsGeral, acntsProf, acntsDef;

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

void exibirErro(char msg[]){
	printf("\n\tVagas para %s Esgotadas.\n\tTe encaminhamos para um acento não-preferencial\n", msg);
}

void registrarPessoa() {
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
	if(!strcmp(strupr(p.prof), "S") && acntsProf <= 0)
		if(acntsGeral > 0) strcpy(p.prof, "N");
		else exibirErro("PROFESSORES");
	strcat(mensagem, p.prof);
	strcat(mensagem, "/");
	printf("\tCONVIDADO ? (S / N): ");
	gets(p.convidado);
	if(!strcmp(strupr(p.convidado), "S") && acntsProf <= 0)
		if(acntsGeral > 0) strcpy(p.convidado, "N");
		else exibirErro("CONVIDADOS");
	strcat(mensagem, p.convidado);
	strcat(mensagem, "/");
	printf("\tPOSSUI ALGUM TIPO DE DEFICIENCIA ? (S / N): ");
	gets(p.defic);
	if(!strcmp(strupr(p.defic), "S") && acntsDef <= 0)
		if(acntsGeral > 0) strcpy(p.defic, "N");
		else exibirErro("DEFICIENTES");
	strcat(mensagem, p.defic);
	printf("\n\tGERANDO TICKET..."); pause(1);
	printf("\n\tOBTENDO CADEIRA..."); pause(0.5);
	printf("\n\n%s", mensagem);
	setMsg(mensagem);
	pegarAcento(p);
}

// Consulta os acentos disponiveis
// para cada categoria a cada cadastro realizado
int consultaAcentos(){
	strcpy(mensagem, "CONSULTA-ACENTOS ");
	setMsg(mensagem);
	dados = split(getResposta(), '/');
	acntsGeral = atoi(dados[0]);
	acntsProf = atoi(dados[1]);
	acntsDef = atoi(dados[2]);
	return (acntsDef + acntsProf + acntsGeral);
}

// No inicio da aplicação a mesma faz uma
// verificação de acentos disponiveis no servidor
int main(int argc, char *argv[]) {
	registra_winsock();
	int acentos = consultaAcentos();
	while(acentos){
		registrarPessoa();
		acentos--;
		acentos = consultaAcentos();
	}
	system("cls");
	printf("\n\n\n\t************************************\n");
	printf("\t************************************\n");
	printf("\t*********                  *********\n");
	printf("\t**********  TEATRO CHEIO  **********\n");
	printf("\t*********                  *********\n");
	printf("\t************************************\n");
	printf("\t************************************\n");
	getch();
}

