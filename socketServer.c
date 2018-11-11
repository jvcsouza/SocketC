﻿#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <stdbool.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include "helpers.c"
// #include <iostream>

#define BUFFER_SIZE 300
#define DEBUG
#define ARQUIVO "PLATEIA.txt"
// using namespace std;
SOCKET escuta, conexao;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
char mensagem[BUFFER_SIZE];
char resposta[BUFFER_SIZE];
// HANDLE thread[0];
// DWORD dwThreadID;
int limitePessoas;
bool salvarTxt;
int controleEntrada = 0, 
reservadoProfConv = 0,
reservadoDef = 0, i, lotacao;
FILE *arqv;
char *nomeArqv;
bool auxStruct = true;

typedef struct {
	int cadeira;
	char nome[100];
	char email[100];
	char ticket[10];
	char data[30];
	char prof[2];
	char deficiente[2];
	char convidado[2];
} person;

person *plateia;

void registra_winsock() { // Inicializa o uso da DLL Winsock
	WSADATA wsadata;
	if (WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
		printf("Winsock Error\n");
		exit(1);
	}
}
void cria_socket_escuta() {
	escuta = socket(AF_INET, SOCK_STREAM, 0); // TCP
	if (escuta < 0) {
		printf("Socket Error\n");
		exit(1);
	}
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP do servidor
	server_address.sin_port = htons(33000); // N�mero da porta
	int len = sizeof(server_address);
	if (bind(escuta, (struct sockaddr *)&server_address, len) < 0) {
		printf("Bind Error\n");
		exit(1);
	}
	if (listen(escuta, 5) < 0) {
		printf("Listen Error\n");
		exit(1);
	}
}
void aguarda_conexao() {
	int len = sizeof(client_address);
	conexao = accept(escuta, (struct sockaddr *)&client_address, &len);
	if (conexao < 0) {
		printf("Accept Error\n");
		exit(1);
	}
}
void le_mensagem() {
	recv(conexao, mensagem, BUFFER_SIZE, 0);
}

char * gerarTicket(){
	srand(time(NULL));
	int numero = rand() * 2 / 4;
	char * ticket;
	char retorno[20];
	itoa(numero, retorno, 10);
	ticket = retorno;
	return ticket;
}

void montarResposta(person p){
	char cadeira[6];
	itoa(p.cadeira, cadeira, 10);
	strcpy(resposta, p.nome);
	strcat(resposta, "/");
	strcat(resposta, cadeira);
	strcat(resposta, "/");
	strcat(resposta, p.ticket);
	strcat(resposta, "/");
	strcat(resposta, p.data);
	printf("\t_______________________________\n");
	printf("\n\t%s\t%s ENTROU!\n\tTICKET: %s!\n\tCADEIRA %i!\n",
		p.data, p.nome, p.ticket, p.cadeira);
	printf("\t_______________________________\n\t");
	plateia[p.cadeira - 1] = p;
}

// void * salvarEmArquivo(void * param){
// 	std::thread(salvarEmArquivo);
// 	return 0;
// }
// void * salvarEmArquivo (void * param);
// void criarThread(person p){
// 	thread[0] = CreateThread(NULL,
// 		0,
// 		salvarEmArquivo,
// 		p,
// 		0,
// 		&dwThreadID);
// 	sleep(0);
// }

void salvarEmArquivo(person p){
	arqv = fopen(ARQUIVO, "a");
	fprintf(arqv, "\n%s\n", "-------------------------");
	fprintf(arqv, "ENTRADA: %s", p.data);
	fprintf(arqv, "NOME: %s\n", p.nome);
	fprintf(arqv, "ACENTO: %i\n", p.cadeira);
	fprintf(arqv, "EMAIL: %s\n", p.email);
	fprintf(arqv, "TICKET: %s\n", p.ticket);
	if(!strcmp(p.deficiente, "S") || !strcmp(p.convidado, "S") || !strcmp(p.prof, "S"))
		fprintf(arqv, "** ACENTO PREFERENCIAL **\n");
	printf("\n");
	fclose(arqv);
}

char * getData(){
	time_t t;
	time(&t);
	struct tm *data;
	data = localtime(&t);
	return asctime(data);
}

void criarArquivo(){
	char a[25];
	strncpy(a, getData(), 24);
	a[24] = '\0';
	salvarTxt = true;
	arqv = fopen(ARQUIVO, "w");
	fprintf(arqv, "\t***********************************");
	fprintf(arqv, "\n\t*                                 *");
	fprintf(arqv, "\n\t*    EXPECTADORES PALETRA UNIP    *");
	fprintf(arqv, "\n\t*    %s     *\n", a);
	fprintf(arqv, "\t*                                 *\n");
	fprintf(arqv, "\t***********************************\n");
	fclose(arqv);
}

void inicializarStruct();
void registrarEntrada(person p){
	if(auxStruct){ 
		for(i = 0; i <= lotacao; i++) plateia[i].cadeira = (int) 0;
		auxStruct = false; 
	}
	if(limitePessoas > 0){
		if(!strcmp(p.convidado, "S") || !strcmp(p.prof, "S")){
			for(i = 0; i < reservadoProfConv; i++){
				if(plateia[i].cadeira == 0){
					p.cadeira = i + 1;
					reservadoProfConv--;
					break;
				}
			}
		}else if(!strcmp(p.deficiente, "S")){
			if(plateia[reservadoDef].cadeira == 0){
				p.cadeira = reservadoDef + 1;
				reservadoDef++;
			}
		}else{
			for(i = 0; i < lotacao; i++) printf("%i\n", plateia[i].cadeira);
			getch();
			if(plateia[controleEntrada].cadeira == 0 && controleEntrada < lotacao){
				p.cadeira = controleEntrada + 1;
				controleEntrada++;
			}
		}
		limitePessoas--;
		strcpy(p.ticket, "#");
		strcat(p.ticket, gerarTicket());
		strcpy(p.data, getData());
		montarResposta(p);
		if(salvarTxt) salvarEmArquivo(p);
	}else strcpy(resposta, "LOTACAO ATINGINDA!");
}

void separaDados(char * string){
	person p;
	strcpy(string, &mensagem[9]);
	char **dados = split(string, '/');
	strcpy(p.nome, strupr(dados[0]));
	strcpy(p.email, strupr(dados[1]));
	strcpy(p.prof, strupr(dados[2]));
	strcpy(p.convidado, strupr(dados[3]));
	strcpy(p.deficiente, strupr(dados[4]));
	registrarEntrada(p);
}

int consultaAcentos(){
	char acentos[6];
	itoa(controleEntrada, acentos, 10);
	strcpy(resposta, acentos);
}

void processa() {
	char comando[20];
	int i;
	for (i = 0; i < BUFFER_SIZE; i++) {
		if (mensagem[i] == ' ') {
			comando[i] = '\0';
			break;
		} else comando[i] = mensagem[i];
	}
	if (!strcmp(comando, "REGISTRA")) separaDados(mensagem);
	else if (!strcmp(comando, "CONSULTA-ACENTOS")) consultaAcentos();
	else strcpy(resposta, "ERRO");
}

void envia_resposta() {
	send(conexao, resposta, BUFFER_SIZE, 0);
}
void fecha_conexao() {
	closesocket(conexao);
}
void inicializarStruct(int limite){
	person pessoas[limite];
	plateia = pessoas;
	for(i = 0; i <= limite; i++)
		plateia[i].cadeira = 0;
}

void configurarServidor(){
	char aux[0];
	printf("\n\t\tCONFIGURACAO INICIAL DO SERVIDOR!\n");
	printf("\n\tCAPACIDADE DO LOCAL: ");
	scanf("%i", &limitePessoas);
	reservadoDef = (limitePessoas * 0.05);
	reservadoProfConv = ((limitePessoas - reservadoDef) * 0.25);
	controleEntrada = limitePessoas - reservadoProfConv;
	inicializarStruct(limitePessoas);
	lotacao = limitePessoas;
	printf("\n\tESPACOS RESERVADOS:\n\n\tDEFICIENTES: %i.\n", reservadoDef);
	printf("\tPROFESSORES/CONVIDADOS: %i.\n", reservadoProfConv);
	printf("\tDEMAIS EXPECTADORES: %i.\n\n", controleEntrada - reservadoDef);
	printf("\tSALVAR TICKETS EM ARQUIVO DE TEXTO (S / N) : ");
	scanf("%s", &aux[0]);
	if(!strcmp(strupr(aux), "S")) criarArquivo();
	printf("\n\tINICIANDO SERVIDOR...");
	pause(2);
	system("cls");
}

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "");
	configurarServidor();
	registra_winsock();
	cria_socket_escuta();
	printf("\n\t\tSERVIDOR EM FUNCIONAMENTO\n");
	printf("\n\n\tLOGS DE ENTRADA: \n\n\t");
	while (true) {
		aguarda_conexao();
		le_mensagem();
		processa();
		envia_resposta();
		fecha_conexao();
	}
	getch();
	return 0;
}