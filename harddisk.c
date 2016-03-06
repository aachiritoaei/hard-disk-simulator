#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "harddisk.h"

/* Circular linked-list */

node* get_Circular_Tail(node *head){
	node *tail = head;
	while(tail->next != head)
		tail = tail->next;
	return tail;
}
/* Add element to the end of the circular linked-list */
void addCircular(struct node **head, int index){
	struct node *new = (node*)malloc(sizeof(node));
	new->index = index;
	new->damage = 0;
	new->data = strdup("0000");
	new->next = *head;
	struct node *temp = *head;
	if((*head) == NULL){
		*head = new;
		(*head)->next = (*head);
	} else {
		node *tail = get_Circular_Tail(temp);
		tail->next = new;
		new->next = temp;
	}
}

/* Create a circular list with numberOfNodes elements */
void createCircular(struct node **listCirc, int numberOfNodes){
	*listCirc = NULL;
	int i;
	for(i = 0; i < numberOfNodes; i++){
		addCircular(listCirc, i);
	}
}

/* Show linked list */
void showCirc(node *head){
	struct node *temp = head;
	do{
		printf("%d %d %s\n", temp->index, temp->damage, temp->data);
		temp = temp->next;
	} while (temp != head);
	printf("\n");
}

/* Double linked-list */

line* get_DoubleLinked_Tail(line *head){
	while(head->next != NULL)
		head = head->next;
	return head;
}

/* Add element to the end of the double linked-list */
void addDoubleLinked(struct line **lineHead, int index){
	struct line *new = (line*)malloc(sizeof(line));
	new->index = index;
	new->next = NULL;
	new->circular = NULL;
	createCircular(&new->circular, pow(2, 4 + index));
	struct line *temp = *lineHead;
	if((*lineHead) == NULL){
		*lineHead = new;
		(*lineHead)->next = NULL;
		(*lineHead)->previous = NULL;
	} else {
		line *tail = get_DoubleLinked_Tail(temp);
		tail->next = new;
		new->previous = tail;
	}
}

/* Create a double linked-list 
 * In this case, the hard disk itself
 */
void createHard(struct line **hard, int numberOfLines){
	*hard = NULL;
	int i;
	for(i = 0; i < numberOfLines; i++){
		addDoubleLinked(hard, i);
	}
}

/* Show double linked-list */
void showDL(line *lineHead){
	while(lineHead != NULL){
		printf("%d %p  %p %p\n", lineHead->index, lineHead->previous, lineHead, lineHead->next);
		lineHead = lineHead->next;
	}
}

/* Show hard disk */
void showHard(line *lineHead){
	while(lineHead != NULL){
		printf("Linia %d \n", lineHead->index);
		showCirc(lineHead->circular);
		lineHead = lineHead->next;
	}
}

/* Show damage average on all of the 4th sectors */
void endCharts(line *hardDisk, int numberOfLines){
	float sector1, sector2, sector3, sector4;
	struct node *head;
	int i, j, numitor = 0;
	sector1 = 0;
	sector2 = 0;
	sector3 = 0;
	sector4 = 0;
	for(i = 0; i < numberOfLines; i++){
		head = hardDisk->circular;
		for(j = 0; j <= pow(2, i + 4) / 4 - 1; j++){
			sector1 += head->damage;
			head = head->next;
		}
		// printf("%.1f ", pow(2, i+4) / 4);
		for(j = pow(2, i + 4) / 4; j <= pow(2, i + 4) / 2 - 1; j++){
			sector2 += head->damage;
			head = head->next;
		}
		// printf("%.1f ", pow(2, i+4) / 2);
		for(j = pow(2, i + 4) / 2; j <= 3 * pow(2, i + 4) / 4 - 1; j++){
			sector3 += head->damage;
			head = head->next;
		}
		// printf("%.1f ", 3 * pow(2, i + 4) / 4);
		for(j = 3 * pow(2, i + 4) / 4; j <= pow(2, i + 4) - 1; j++){
			sector4 += head->damage;
			head = head->next;
		}
		// printf("\n");
		hardDisk = hardDisk->next;
	}
	for(i = 0; i < numberOfLines; i++){
		numitor += pow(2, 2 + i);
	}
	sector1 /= numitor;
	sector2 /= numitor;
	sector3 /= numitor;
	sector4 /= numitor;
	sector1 = floorf(sector1 * 100) / 100;
	sector2 = floorf(sector2 * 100) / 100;
	sector3 = floorf(sector3 * 100) / 100;
	sector4 = floorf(sector4 * 100) / 100;
	printf("%.2f %.2f %.2f %.2f\n", sector1, sector2, sector3, sector4);
}