#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "structures.h"
#include "harddisk.h"
#include "commands.h"


int main(int argc, char *argv[]){
	/* Command line paramters: input file, output file */
	freopen(argv[1], "r", stdin);
	freopen(argv[2], "w", stdout);
	/* Choose case: stack implementation/queue implementation
   	 * Get number of hard disk lines
	 */
	int type, numberOfLines;
	scanf("%d %d", &type, &numberOfLines);
	getchar();
	/* Initialize hard disk */
	line *hardDisk;
	createHard(&hardDisk, numberOfLines);
	/* Initialize cursor at position (0, 0) */
	struct cursor *cursor = malloc(sizeof(cursor));
	cursor->DL = hardDisk;
	cursor->CL = cursor->DL->circular;
	/* Queue case */
	if(type == 1){
		struct command *queue = NULL;
		struct command *currentCommand = malloc(sizeof(command));
		currentCommand->isExecuted = 1;
		int waitTimer = 0;
		char *buffer = malloc(4096 * sizeof(char));
		while(1){
			/* Wait timer is 0 -> add command in queue */
			while((waitTimer == 0) && (!feof(stdin))){
				struct command *new = malloc(sizeof(command));
				fgets(buffer, 4096, stdin);
				char *cuv = NULL;
				cuv = strtok(buffer, " \r\n\t");
				if (cuv == NULL) {
					continue;
				}
				/* Read command type  - "::r" / "::d" */
				if(strcmp(cuv, "::r") == 0 || strcmp(cuv, "::d") == 0){
					new->type = strdup(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexDL = atoi(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexCL = atoi(cuv);
					fgets(buffer, 64, stdin);
					new->waitTimer = atoi(buffer);
				/* Write command type - "::w" */
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
				/* End command type  - "::e" */
				} else if(strcmp(cuv, "::e") == 0){
					new->type = strdup(cuv);
				} else {
					continue;
				}
				new->isExecuted = 0;
				pushQueueCommand(&queue, new);
				waitTimer = new->waitTimer;
			}
			/* Command which is not executed */
			if(currentCommand->isExecuted == 0){
				/* Move cursor at required line */
				if(currentCommand->indexDL != cursor->DL->index){
					/* Move at 0 adress, in order to change line */
					if(cursor->CL->index != 0){
						cursor->CL = moveOneAddress(cursor->CL);
					}
					else{ 
						cursor->DL = moveOneLine(cursor->DL, currentCommand->indexDL);
						cursor->CL = cursor->DL->circular;
					}
				}
				else{ /* Move cursor at required address */
					if(cursor->CL->index != currentCommand->indexCL)
						cursor->CL = moveOneAddress(cursor->CL);
					else { /* Cursor is on required position -> execute command */
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
						/* Multi read */
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
						/* Multi write */
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
				/* Comanda was executed
				 * Add another one to queue
				 * If there is no command to be executed, wait on current position
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
		/* Free buffer */
		free(buffer);
	}
	/* Stack */
	if(type == 2){
		struct command *stack = NULL;
		struct command *currentCommand = malloc(sizeof(command));
		struct command *new;
		currentCommand->isExecuted = 1;
		int waitTimer = 0;
		char *buffer = malloc(64 * sizeof(char));
		while(1){
			/* Read command when timer is 0
			 * If the previous command hasn't been executed, add it to execution stack
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
				/* Type read - "::r" / "::d" */
				if(strcmp(cuv, "::r") == 0 || strcmp(cuv, "::d") == 0){
					new->type = strdup(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexDL = atoi(cuv);
					cuv = strtok(0, " \r\n\t");
					new->indexCL = atoi(cuv);
					fgets(buffer, 64, stdin);
					new->waitTimer = atoi(buffer);
				/* Type write - "::w" */
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
				/* Type end  - "::e" */
				else if(strcmp(cuv, "::e") == 0){
					new->type = strdup(cuv);
				}
				new->isExecuted = 0;
				waitTimer = new->waitTimer;
				currentCommand = new;
			}
			if(strcmp(currentCommand->type, "::e") == 0)
				break;
			/* Comanda was executed
			 * Pop another one from execution stack
			 * If stack is empty, wait until a command is given
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
			/* Command to be exeuted */
			if(currentCommand->isExecuted == 0){
				/* Move cursor at required line */
				if(currentCommand->indexDL != cursor->DL->index){
					/* Move cursor at 0 index, in order to move at a different line */
					if(cursor->CL->index != 0){
						cursor->CL = moveOneAddress(cursor->CL);
					}
					else{
						cursor->DL = moveOneLine(cursor->DL, currentCommand->indexDL);
						cursor->CL = cursor->DL->circular;
					}
				}
				else{ /* Move cursor on required address */
					if(cursor->CL->index != currentCommand->indexCL)
						cursor->CL = moveOneAddress(cursor->CL);
					else { /* Cursor is on required position => execute command */
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
		/* Free stack */
		command *temp;
		while(stack){
			temp = stack;
			stack = stack->next;
			free(temp);
		}
	}
	/* Sector damage */
	endCharts(hardDisk, numberOfLines);
	/* Free memory */
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