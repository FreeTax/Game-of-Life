/*
FRANCESCO MAZZOLA
GAME OF LIFE
*/
#include <iostream>
#include <process.h>
#include<time.h>
#include <conio.h>
#include <windows.h>
int iMatrix[20][20];
const int iSize = 19;

void full(int i, int j, bool a, int max) { //funzione che riempie la matrice a discrepanza di parametri passati dal main 
	srand((unsigned)time(NULL));
	do{
		do{
			if (a) //in caso che il main lo richieda il programma riempierà con numeri randomici (0,1) le celle della matrice per la generazione dell'ambiente di gioco
				iMatrix[j++][i] = rand() % 2;
			else// prima di riempire in maniera randomica il programma riempie la matrice con tutti zeri in modo da riuscire a gestire i bordi  
				iMatrix[j++][i] = 0;
		} while (j < max);
		i++;
	} while (i < max);
}

unsigned int __stdcall print(void* data) {//funzione di stampa che stampa tutta la matrice meno una riga per direzione che sono i bordi. i bordi sono inizializati a zero
	do {
		HANDLE& mutex = *(HANDLE*)data;//occupazione della matrice
		Sleep(200);
		system("cls");
		for (int i = 1; i < iSize; i++) {
			for (int j = 1; j < iSize; j++) {
				if (iMatrix[j][i] == 1)
					std::cout << 'o';
				else
					std::cout << '.';
			}
			std::cout << std::endl;
		}
		ReleaseMutex(mutex);//rilascio della matrice
	} while (true);
}

unsigned int __stdcall controll(void* data) {//funzione che esegue il controllo delle celle meno le celle di bordo
	do {
		HANDLE& mutex = *(HANDLE*)data;// occupazione della matrice
		int a;
		for (int i = 1; i < iSize; i++) {
			for (int j = 1; j < iSize; j++) {
				a = iMatrix[j - 1][i - 1] + iMatrix[j - 1][i] + iMatrix[j - 1][i + 1] + iMatrix[j][i - 1] + iMatrix[j][i + 1] + iMatrix[j + 1][i - 1] + iMatrix[j + 1][i] + iMatrix[j + 1][i + 1];// il programma esegue la somma dei valori delle celle adiacenti e controlla il valore e, in conseguenza a ciò, esegue determinate azioni
				if (a < 2 && iMatrix[i][j] == 1 || a>3 && iMatrix[i][j] == 1)
					iMatrix[i][j] = 0;
				else if (a == 3 && iMatrix[i][j] == 0)
					iMatrix[i][j] = 1;
				else
					iMatrix[i][j] = 1;
			}
		}
		ReleaseMutex(mutex);//rilascio della matrice
	} while (true);
}
void main() {//main
	full(0, 0, false, 20);
	full(1, 1, true, 19);
	HANDLE control, prin, mutex = CreateMutex(0, 0, 0);
	control = (HANDLE)_beginthreadex(0, 0, &controll, &mutex, 0, 0);//lancio di un thread per eseguire il controllo delle celle
	prin = (HANDLE)_beginthreadex(0, 0, &print, &mutex, 0, 0);//lancio di un thread per la stampa delle celle
	system("PAUSE"); 
}
