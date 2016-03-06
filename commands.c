#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"


/* Commands */

/* ::r */
void readData(struct cursor **cursor){
	printf("%s\n", (*cursor)->CL->data);
	(*cursor)->CL->damage += 5;
}

/* ::d */
void readDamage(struct cursor **cursor){
	printf("%d\n", (*cursor)->CL->damage);
	(*cursor)->CL->damage += 2;
}

/* ::w */
void writeData(struct cursor **cursor, char *string){
	(*cursor)->CL->data = strdup(string);
	(*cursor)->CL->damage += 30;
}

/* Move cursor one line */
line *moveOneLine(line *cursor, int indexDL){
	if(cursor->index > indexDL)
		cursor = cursor->previous;
	else
		cursor = cursor->next;
	return cursor;
}

/* Move curson one address */
node* moveOneAddress(node *cursor){
	cursor = cursor->next;
	return cursor;
}

/* Case 1 - queue implementation */
void pushQueueCommand(command **head, command *new){
	struct command *temp = *head;
	if(temp == NULL)
		*head = new;
	else{
		while(temp ->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
}

command *popQueueCommand(command **head){
	struct command *temp = *head;
	if(temp == NULL)
		return NULL;
	else if(temp->next == NULL){
		*head = NULL;
		return temp;
	} else {
		temp = *head;
		*head = (*head)->next;
		return temp;
	}
}
/* Case 2 - stack implementation */
void pushStackCommand(command **head, command *new){
	struct command *temp = *head;
	if(temp == NULL)
		*head = new;
	else{
		while(temp ->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
}

command *popStackCommand(command **head){
	struct command *temp = *head;
	struct command *previous;
	if(temp == NULL)
		return NULL;
	else if(temp->next == NULL){
		*head = NULL;
		return temp;
	} else {
		while(temp->next != NULL){
			previous = temp;
			temp = temp->next;
		}
		previous->next = NULL;
		return temp;
	}
}