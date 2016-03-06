#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "structures.h"
#include "harddisk.h"
#include "commands.h"


int main(int argc, char *argv[]){
	/* Se primesc ca parametri din linia de comanda fisierul de input si cel de output */
	freopen(argv[1], "r", stdin);
	freopen(argv[2], "w", stdout);
	/* Se preiau din fisier unul dintre cazurile coada/stiva si numarul de linii al hardDisk-ului */
	int type, numberOfLines;
	scanf("%d %d", &type, &numberOfLines);
	getchar();
	/* Initializare hardDisk */
	line *hardDisk;
	createHard(&hardDisk, numberOfLines);
	/* Initializare cursor la pozitia (0, 0) */
	struct cursor *cursor = malloc(sizeof(cursor));
	cursor->DL = hardDisk;
	cursor->CL = cursor->DL->circular;
	/* Cazul coada */
	if(type == 1){
		struct command *queue = NULL;
		struct command *currentCommand = malloc(sizeof(command));
		currentCommand->isExecuted = 1;
		int waitTimer = 0;
		char *buffer = malloc(4096 * sizeof(char));
		while(1){
			/* Timp de asteptare 0 => adaugam o comanda in coada */
			while((waitTimer == 0) && (!feof(stdin))){
				struct command *new = malloc(sizeof(command));
				fgets(buffer, 4096, stdin);
				char *cuv = NULL;
				cuv = strtok(buffer, " \r\n\t");
				if (cuv == NULL) {
					continue;
				}
				/* De tip read - "::r" / "::d" */
				if(strcmp(cuv, "::r") == 0 || strcmp(cuv, "::d") == 0){
					new->type = strdup(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexDL = atoi(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexCL = atoi(cuv);
					fgets(buffer, 64, stdin);
					new->waitTimer = atoi(buffer);
				/* De tip write - "::w" */
				} else if(strcmp(cuv, "::w") == 0){ 
					new->type = strdup(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexDL = atoi(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexCL = atoi(cuv);
					cuv = strtok(0, " \r\n\t");
					new->data = strdup(cuv);
					fgets(buffer, 64, stdin);
					new->waitTimer = atoi(buffer);
				/* BONUS - Multi read */
				}else if(strcmp(cuv, "::mr") == 0){
					new->type = strdup(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexDL = atoi(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexCL = atoi(cuv);
					cuv = strtok(0, " \r\n\t");
					new->no = atoi(cuv);
					fgets(buffer, 64, stdin);
					new->waitTimer = atoi(buffer);
				/* BONUS - Multi write */
				} else if(strcmp(cuv, "::mw") == 0){
					new->type = strdup(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexDL = atoi(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexCL = atoi(cuv);
					new->data = strdup(cuv + strlen(cuv) + 1);
					fgets(buffer, 64, stdin);
					new->waitTimer = atoi(buffer);
				/* De tip end  - "::e" */
				} else if(strcmp(cuv, "::e") == 0){
					new->type = strdup(cuv);
				} else {
					continue;
				}
				new->isExecuted = 0;
				pushQueueCommand(&queue, new);
				waitTimer = new->waitTimer;
			}
			/* Comanda care nu este executata */
			if(currentCommand->isExecuted == 0){
				/* Mutare cursor pe linia necesara */
				if(currentCommand->indexDL != cursor->DL->index){
					/* Mutare cursor la adresa 0, inainte de a ne muta pe linie */
					if(cursor->CL->index != 0){
						cursor->CL = moveOneAddress(cursor->CL);
					}
					else{ 
						cursor->DL = moveOneLine(cursor->DL, currentCommand->indexDL);
						cursor->CL = cursor->DL->circular;
					}
				}
				else{ /* Mutare cursor pe adresa necesara */
					if(cursor->CL->index != currentCommand->indexCL)
						cursor->CL = moveOneAddress(cursor->CL);
					else { /* Cursorul se afla pe pozitia buna => executa comanda */
						if(strcmp(currentCommand->type, "::r") == 0){
							readData(&cursor);
							free(currentCommand->type);
							currentCommand->isExecuted = 1;
						}
						if(strcmp(currentCommand->type, "::w") == 0){
							writeData(&cursor, currentCommand->data);
							free(currentCommand->data);
							free(currentCommand->type);
							currentCommand->isExecuted = 1;
						}
						if(strcmp(currentCommand->type, "::d") == 0){
							readDamage(&cursor);
							free(currentCommand->type);
							currentCommand->isExecuted = 1;
						}
						/* Multi read 
						 * Execut comanda de read, apoi incrementez adresa
						 * Daca se depaseste limita de adrese de pe linia curenta, se trece pe linia urmatoare
						 */
						if(strcmp(currentCommand->type, "::mr") == 0){
							readData(&cursor);
							currentCommand->no--;
							if(currentCommand->indexCL == pow(2, 4 + currentCommand->indexDL) - 1){
								currentCommand->indexDL++;
								currentCommand->indexCL = 0;
							}
							else
								currentCommand->indexCL++;
							if(currentCommand->no == 0)
								currentCommand->isExecuted = 1;
						}
						/* Multi write
						 * In campul currentCommand->data avem retinul sirul tuturor numelor de adrese ce vor fi scrise
						 * Selectam cate unul, folosind strtok
						 * Incrementam adresa si repetam acest procedeu pana la intalnirea lui '.'
						 */
						if(strcmp(currentCommand->type, "::mw") == 0){
							char *token;
							char *aux = strdup(currentCommand->data);
							token = strtok(aux, " \r\n\t");
							if(strcmp(token, ".") == 0)
								currentCommand->isExecuted = 1;
							else{
								writeData(&cursor, token);
								if(currentCommand->indexCL == pow(2, 4 + currentCommand->indexDL) - 1){
									currentCommand->indexDL++;
									currentCommand->indexCL = 0;
								}
								else
									currentCommand->indexCL++;
								free(currentCommand->data);
								currentCommand->data = strdup(token + strlen(token) + 1);
							}
							free(aux);
						}
					}
				}
				if(waitTimer > 0) waitTimer--;
				cursor->CL->damage++;
			}
			else{ 
				/* Comanda a fost executata
				 * Incarcam alta de pe coada
				 * Daca nu exista comenzi de executat, se asteapta pe pozitia curenta
				 */
				if(queue != NULL){
					currentCommand = popQueueCommand(&queue);
					if(strcmp(currentCommand->type, "::e") == 0)
							break;
				}
				else{ 
					if(waitTimer > 0) waitTimer--;
					cursor->CL->damage++;
				}
			}
		}
		/* Eliberare buffer */
		free(buffer);
	}
	/* Cazul stiva */
	if(type == 2){
		struct command *stack = NULL;
		struct command *currentCommand = malloc(sizeof(command));
		struct command *new;
		currentCommand->isExecuted = 1;
		int waitTimer = 0;
		char *buffer = malloc(64 * sizeof(char));
		while(1){
			/* Citim o comanda cand temporizatorul este 0
			 * Daca comanda precedenta nu a fost executata, este introdusa in stiva de executie
			 */
			while((waitTimer == 0) && (!feof(stdin))){
				if(currentCommand->isExecuted == 0)
					pushStackCommand(&stack, currentCommand);
				new = malloc(sizeof(command));
				fgets(buffer, 64, stdin);
				char *cuv = NULL;
				cuv = strtok(buffer, " \r\n\t");
				if(cuv == NULL){
					continue;
				}
				/* De tip read - "::r" / "::d" */
				if(strcmp(cuv, "::r") == 0 || strcmp(cuv, "::d") == 0){
					new->type = strdup(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexDL = atoi(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexCL = atoi(cuv);
					fgets(buffer, 64, stdin);
					new->waitTimer = atoi(buffer);
				/* De tip write - "::w" */
				} else if(strcmp(cuv, "::w") == 0){
					new->type = strdup(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexDL = atoi(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexCL = atoi(cuv);
					cuv = strtok(0, " \r\n\t");
					new->data = strdup(cuv);
					fgets(buffer, 64, stdin);
					new->waitTimer = atoi(buffer);
				}
				/* De tip end  - "::e" */
				else if(strcmp(cuv, "::e") == 0){
					new->type = strdup(cuv);
				}
				new->isExecuted = 0;
				waitTimer = new->waitTimer;
				currentCommand = new;
			}
			if(strcmp(currentCommand->type, "::e") == 0)
				break;
			/* Comanda a fost executata
			 * Preluam alta de pe stiva de executie
			 * In cazul in care nu exista comenzi in stiva de executie, se asteapta primirea unei comenzi
			 */
			if(currentCommand->isExecuted == 1){
				if(stack){
					free(currentCommand->type);
					free(currentCommand);
					currentCommand = popStackCommand(&stack);
				}
				else{
					if(waitTimer > 0) waitTimer--;
					cursor->CL->damage++;
				}
			}
			/* Comanda care trebuie executata */
			if(currentCommand->isExecuted == 0){
				/* Mutare cursor pe linia necesara */
				if(currentCommand->indexDL != cursor->DL->index){
					/* Mutare cursor la adresa 0, inainte de a ne muta pe linie */
					if(cursor->CL->index != 0){
						cursor->CL = moveOneAddress(cursor->CL);
					}
					else{
						cursor->DL = moveOneLine(cursor->DL, currentCommand->indexDL);
						cursor->CL = cursor->DL->circular;
					}
				}
				else{ /* Mutare cursor pe adresa necesara */
					if(cursor->CL->index != currentCommand->indexCL)
						cursor->CL = moveOneAddress(cursor->CL);
					else { /* Cursorul se afla pe pozitia buna => executa comanda */
						if(strcmp(currentCommand->type, "::r") == 0){
							readData(&cursor);
						}
						if(strcmp(currentCommand->type, "::w") == 0){
							writeData(&cursor, currentCommand->data);
							free(currentCommand->data);
						}
						if(strcmp(currentCommand->type, "::d") == 0)
							readDamage(&cursor);
						currentCommand->isExecuted = 1;
					}
				}
				if(waitTimer > 0) waitTimer--;
				cursor->CL->damage++;
			}
		}
		free(buffer);
		/* Eliberare stiva */
		command *temp;
		while(stack){
			temp = stack;
			stack = stack->next;
			free(temp);
		}
	}
	/* Damage pe sectoare */
	endCharts(hardDisk, numberOfLines);
	/* Eliberare memorie */
	free(cursor);
	struct line *tempLine;
	struct node *tempAddress, *tempNext, *tempInit;
	while(hardDisk != NULL){
		tempLine = hardDisk;
		tempInit = hardDisk->circular;
		tempAddress = tempInit->next;
		while(tempAddress != tempInit){
			tempNext = tempAddress->next;
			free(tempAddress->data);
			free(tempAddress);
			tempAddress = tempNext;
		}
		hardDisk = hardDisk->next;
		free(tempLine);
	}
	free(tempAddress);
	return 0;
}