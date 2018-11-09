#include <windows.h>

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

typedef DWORD WINAPI(*funcao)(person);
HANDLE thread[0];
DWORD dwThreadID;

void criarThread(funcao fun, person p){
	thread[(DWORD)0] = CreateThread(NULL,
		0,
		&fun,
		p,
		NULL,
		&dwThreadID);
	sleep(0);
}