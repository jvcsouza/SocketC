#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <stdbool.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include "helpers.c"

#define BUFFER_SIZE 300
#define DEBUG
#define ARQUIVO "PLATEIA.txt"
SOCKET escuta, conexao;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
char mensagem[BUFFER_SIZE];
char resposta[BUFFER_SIZE];
int limitePessoas;
bool salvarTxt;
int controleEntrada = 0, 
reservadoProfConv = 0,
reservadoDef = 0, i, lotacao;
FILE *arqv;
char *nomeArqv;
int reservDef = 0;

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
person sorteio[BUFFER_SIZE];
int auxSorteio = 0;

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

// Gera um numero aletorio de 4 a 5 digitos
// baseando-se nos milissengundos
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
	printf("\n\t%s\t%s ENTROU!\n\tTICKET: %s!\n\tCADEIRA %i!\n\t\t",
		p.data, p.nome, p.ticket, p.cadeira);
	plateia[p.cadeira - 1] = p;
	if(!strcmp(strupr(p.convidado), "N"))
		sorteio[auxSorteio++] = p;
}

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

// Função que retorna data e hora
char * getData(){
	time_t t;
	time(&t);
	struct tm *data;
	data = localtime(&t);
	return asctime(data); // retorna uma string com '\n'
}

void criarArquivo(){
	char a[25];
	strncpy(a, getData(), 24);
	a[24] = '\0';
	salvarTxt = true;
	arqv = fopen(ARQUIVO, "w");
	fprintf(arqv, "\t***********************************");
	fprintf(arqv, "\n\t*                                 *");
	fprintf(arqv, "\n\t*    EXPECTADORES PALESTRA UNIP   *");
	fprintf(arqv, "\n\t*    %s     *\n", a);
	fprintf(arqv, "\t*                                 *\n");
	fprintf(arqv, "\t***********************************\n");
	fclose(arqv);
}

// Registra a entrada da pessoa
// e a coloca na cadeira respectiva
// conforme as informações recebidas
void registrarEntrada(person p){
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
				reservDef--;
			}
		}else{
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

// Lê a mensagem recebida e separa
// dados referente a pessoa que será cadastrada
// salvando-os em maiusculo no struct
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
	char aux[6];
	int acentos = 0;
	for(i = 0; i < lotacao; i++)
		if(plateia[i].cadeira == 0)
			acentos++;
	itoa(acentos - (reservadoProfConv + reservDef), aux, 10);
	strcpy(resposta, aux);
	strcat(resposta, "/");
	itoa(reservadoProfConv, aux, 10);
	strcat(resposta, aux);
	strcat(resposta, "/");
	itoa(reservDef, aux, 10);
	strcat(resposta, aux);
}

// Responsavel por pegar um registro
// aleatorio dentro do struct 'plateia'
void sortearParticipante(){
	srand(time(NULL));
	person p = sorteio[rand() % auxSorteio];
	strcpy(resposta, p.nome);
	strcat(resposta, "/");
	char cad[3];
	itoa(p.cadeira, cad, 10);
	strcat(resposta, cad);
	strcat(resposta, "/");
	strcat(resposta, p.ticket);
}

// Lê a string recebida do client, interpreta
// o comando e envia para a função solicitada
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
	else if (!strcmp(comando, "SORTEAR")) sortearParticipante();
	else strcpy(resposta, "ERRO/ -- / -- / -- /");
}

void envia_resposta() {
	send(conexao, resposta, BUFFER_SIZE, 0);
}
void fecha_conexao() {
	closesocket(conexao);
}

// Função responsavel por alocar o vetor
// de struct 'person' conforme a capacidade digitada
void inicializarStruct(int limite){
	person **pe = (person**)malloc(limite * sizeof(person));
	int lim = limite > 300 ? limite : 300;
	for(i = 0; i <= limite; i++)
		*(pe + i) = (person*)malloc(lim*sizeof(person));
	plateia = *pe;
	for(i -= 1; i >= 0; i--)
		plateia[i].cadeira = 0;
}

// Função responsavel por definir as configurações
// inicias do servidor e separa os acentos diponiveis
// entre convidados, professores e deficientes
void configurarServidor(){
	char aux[0];
	printf("\n\t\tCONFIGURACAO INICIAL DO SERVIDOR!\n");
	printf("\n\tCAPACIDADE DO LOCAL: ");
	scanf("%i", &limitePessoas);
	reservadoDef = (limitePessoas * 0.05);
	reservDef = reservadoDef;
	reservadoProfConv = ((limitePessoas - reservadoDef) * 0.25);
	controleEntrada = limitePessoas - reservadoProfConv;
	inicializarStruct(limitePessoas);
	lotacao = limitePessoas;
	printf("\n\tESPACOS RESERVADOS:\n\n\tDEFICIENTES: %i.\n", reservadoDef);
	printf("\tPROFESSORES/CONVIDADOS: %i.\n", reservadoProfConv);
	printf("\tDEMAIS EXPECTADORES: %i.\n\n", controleEntrada - reservadoDef);
	printf("\tSALVAR TICKETS EM ARQUIVO DE TEXTO (S / N) : ");
	scanf("%s", &aux);
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
	printf("\n\n\tLOGS DE ENTRADA: \n\n");
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
