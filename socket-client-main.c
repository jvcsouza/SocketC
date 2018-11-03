#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <string.h>

#define BUFFER_SIZE 250

SOCKET conexao;
struct sockaddr_in server_address;
char mensagem[BUFFER_SIZE];
char resposta[BUFFER_SIZE];
int controlePessoas = 0;

struct person {
	char convidado[0];
	char nome[100];
	char prof[0];
	char email[100];
	char defic[0];
};
typedef struct person pessoa;

void registra_winsock() { // Inicializa o uso da DLL Winsock
	WSADATA wsadata;
	if (WSAStartup(0x101, (LPWSADATA)&wsadata) != 0) {
    	printf("Winsock Error\n");
    	exit(1);
	}
}
void cria_socket_conexao() {
	conexao = socket(AF_INET, SOCK_STREAM, 0); // TCP
	if (conexao < 0) {
		printf("Socket Error\n");
		exit(1);
	}
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP do servidor
	server_address.sin_port = htons(33000); // N�mero da porta
}
void abre_conexao() {
	int len = sizeof(server_address);
	int result = connect(conexao, (struct sockaddr *)&server_address, len);
	if (result < 0) {
		printf("Connect Error");
		exit(1);
	}
}
void envia_mensagem() {
	send(conexao, mensagem, BUFFER_SIZE, 0);
}
void le_resposta() {
	recv(conexao, resposta, BUFFER_SIZE, 0);
}
void fecha_conexao() {
	closesocket(conexao);
}
void transmite() {
	cria_socket_conexao();
	abre_conexao();
	envia_mensagem();
	le_resposta();
	fecha_conexao();
}

void pause (float delay1) {
   if (delay1 < 0.001) return; // pode ser ajustado e/ou evita-se valores negativos.
   float inst1 = 0, inst2 = 0;
   inst1 = (float) clock() / (float) CLOCKS_PER_SEC;
   while (inst2 - inst1 < delay1) inst2 = (float) clock() / (float) CLOCKS_PER_SEC;
   return;
}

void registrarPessoa();
void pegarAcento(pessoa p){
	transmite();
	printf("\n%s\n", resposta);
	pause(4);
	registrarPessoa();
}

void registrarPessoa() {
	pessoa p;
	strcpy(mensagem, "REGISTRA ");
	system("cls");
	printf("\n\t\tPALETRA DE DIREITOS HUMANOS UNIP!\n\n");
	printf("\n\tEmitir Ticket de Entrada\n");
	printf("\n\tNome Completo: ");
	gets(p.nome);
	strcat(mensagem, p.nome);
	strcat(mensagem, "-");
	printf("\tEmail: ");
	gets(p.email);
	strcat(mensagem, p.email);
	strcat(mensagem, "-");
	printf("\tProfessor ? (S / N): ");
	gets(p.prof);
	strcat(mensagem, p.prof);
	strcat(mensagem, "-");
	printf("\tConvidado ? (S / N): ");
	gets(p.convidado);
	strcat(mensagem, p.convidado);
	strcat(mensagem, "-");
	printf("\tPossui Algum tipo de Deficiencia ? (S / N): ");
	gets(p.defic);
	strcat(mensagem, p.defic);
	printf("\n\tGerando Ticket..."); pause(1);
	printf("\n\tObtendo Cadeira..."); pause(0.5);
	printf("\n\n%s", mensagem);
	pegarAcento(p);
	getch();
}

void opc_par_ou_impar() {
	int numero;
	char numerostr[10];
	printf("Numero: ");
	scanf("%i", &numero);
	itoa(numero, numerostr, 10); // 10 = Base 10
	strcpy(mensagem, "PAR-OU-IMPAR ");
	strcpy(&mensagem[13], numerostr);
	transmite();
	printf("%s\n", resposta);
}

void opc_dobro() {
	int numero;
	char numerostr[10];
	printf("Numero: ");
	scanf("%i", &numero);
	itoa(numero, numerostr, 10); // 10 = Base 10
	strcpy(mensagem, "DOBRO ");
	strcpy(&mensagem[6], numerostr);
	transmite();
	printf("%s\n", resposta);
}

int main(int argc, char *argv[]) {
	registra_winsock();
	registrarPessoa();
}

