#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <stdbool.h>
#include <conio.h>
#include <locale.h>
#include <time.h>

#define BUFFER_SIZE 250
#define DEBUG

SOCKET escuta, conexao;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
char mensagem[BUFFER_SIZE];
char resposta[BUFFER_SIZE];
int limitePessoas;
bool salvarTxt;
int controleEntrada = 0, cadeiras[100] = {0}, i;

struct person {
	int cadeira;
	char nome[100];
	char email[100];
	char ticket[10];
	char hora[100];
	char prof[0];
	char deficiente[0];
	char convidado[0];
};
typedef struct person person;
person pessoa[100];

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

void processa_PAR_OU_IMPAR() {
	char numerostr[10];
	strcpy(numerostr, &mensagem[13]);
	int numero = atoi(numerostr);
	if (numero % 2 == 0) {
		strcpy(resposta, "PAR");
	} else {
		strcpy(resposta, "IMPAR");
	}
}

void processa_DOBRO() {
	char numerostr[10];
	strcpy(numerostr, &mensagem[6]);
	int numero = atoi(numerostr);
	int dobro = numero * 2;
	char dobrostr[10];
	itoa(dobro, dobrostr, 10); // 10 = Base 10
	strcpy(resposta, dobrostr);
}

void gerarTicket(){

}
void registrarEntrada(person p){
	strcpy(pessoa[controleEntrada].nome, "d");
	// strcpy()
}
void separaDados(char * string){
	person p;
	char comando[20];
	char nome[100], email[100], prof[0] = "", conv[0] = "", defic[0] = "";
	int i, separacao = 0;
	for (i = 9; i < BUFFER_SIZE; i++) {
		if (mensagem[i] == '-') {
			nome[i - 9] = '\0';
			strcpy(p.nome, nome);
			i++;
			int j = 0;
			for(j = i; j < strlen(mensagem); j++){
				if(mensagem[j] == '-'){
					email[j - i] = '\0';
					strcpy(p.email, email);
					strncpy(p.prof, &mensagem[j + 1], 1);
					strcpy(p.deficiente, &mensagem[j + 5]);
					// char aux[3];
					strncpy(p.convidado, &mensagem[j + 3], 1);
					printf("\na: %s", p.prof);
					printf("\naa: %s", p.convidado);
					printf("\naa: %s", p.deficiente);
					// j++;
					// int k = 0;
					// for(k = j; k < strlen(mensagem); k++){
					// 	if(mensagem[k] == '-'){
					// 		prof[k - j - 1] = '\0';
					// 		k++;
					// 		int l = 0;
					// 		for(l = k; l < strlen(mensagem); l++){
					// 			if(mensagem[l] == '-'){
					// 				conv[l - k] = '\0';
					// 				printf("\nCONV: %i", l);
					// 				l++;
					// 				int m = 0;
					// 				for(m = l; m < strlen(mensagem); m++){
					// 					if(mensagem[m] == '-'){
					// 						defic[m - l] = '\0';
					// 						printf("\nDEFIC: %i", m);
					// 						break;
					// 					}else{
					// 						defic[m - l] = mensagem[m];
					// 					}
					// 				}
					// 				break;
					// 			}else{
					// 				conv[l - k] = mensagem[l];
					// 			}
					// 		}
					// 		break;
					// 	}else{
					// 		prof[k - j] = mensagem[k];
					// 	}
					// }
					break;
				}else{
					email[j - i] = mensagem[j];
					
				}
			}
			break;
		} else {
			nome[i - 9] = mensagem[i];
		}
	}
	printf("\n\n\nNome: %s", p.nome);
	printf("\nEmail: %s", p.email);
	// printf("\nProf: %s", (char)p.prof[0]);
	// printf("\nConvidado: %s", (char)p.convidado[0]);
	// printf("\nDeficiente: %s", (char)p.deficiente[0]);
	strcpy(resposta, "system('cls');\nTICKET #4215481.\nEMITIDO PARA: ");
	strcat(resposta, p.nome);
	strcat(resposta, ".\nCadeira: 12");
	// registrarEntrada(p);
}

void processa() {
	#ifdef DEBUG
	printf("%s\n", mensagem);
	#endif
	char comando[20];
	int i;
	for (i = 0; i < BUFFER_SIZE; i++) {
		if (mensagem[i] == ' ') {
			comando[i] = '\0';
			break;
		} else {
			comando[i] = mensagem[i];
		}
	}
	if (strcmp(comando, "PAR-OU-IMPAR") == 0) processa_PAR_OU_IMPAR();
	else if (strcmp(comando, "DOBRO") == 0) processa_DOBRO();
	else if (!strcmp(comando, "REGISTRA")) separaDados(mensagem);
	else {
		strcpy(resposta, "ERRO");
	}
}

void envia_resposta() {
	send(conexao, resposta, BUFFER_SIZE, 0);
}

void pause (float delay1) {
   if (delay1 < 0.001) return; // pode ser ajustado e/ou evita-se valores negativos.
   float inst1 = 0, inst2 = 0;
   inst1 = (float) clock() / (float) CLOCKS_PER_SEC;
   while (inst2 - inst1 < delay1) inst2 = (float) clock() / (float) CLOCKS_PER_SEC;
   return;
}

void fecha_conexao() {
	closesocket(conexao);
}

void configurarServidor(){
	char aux[0];
	printf("\n\tConfiguração Inicial do Servidor!\n");
	printf("\nCapacidade do Local: ");
	scanf("%i", &limitePessoas);
	printf("Deseja Salvar os Tickets em um Arquivo de Texto (S / N) : ");
	scanf("%i", &aux[0]);
	if(!strcmp(aux, "s") || !strcmp(aux, "S")) salvarTxt = true;
	printf("Iniciando Servidor...");
	pause(2);
	system("cls");
}

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "");
	configurarServidor();
	printf("\n\tSERVIDOR EM FUNCIONAMENTO\n");
	printf("\nLogs de Entrada: ...\n\n");
	registra_winsock();
	cria_socket_escuta();
	while (true) {
		aguarda_conexao();
		le_mensagem();
		processa(); // Trabalhar aqui...
		envia_resposta();
		fecha_conexao();
	}
	getch();
	return 0;
}

