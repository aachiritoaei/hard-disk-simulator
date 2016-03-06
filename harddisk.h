#ifndef _HARDDISK_H_
#define _HARDDISK_H_

#include "structures.h"

/* Circular linked-list */
node* get_Circular_Tail(node *head);
void addCircular(struct node **head, int index);
void createCircular(struct node **listCirc, int numberOfNodes);
void showCirc(node *head);

/* Double linked-list */
line* get_DoubleLinked_Tail(line *head);
void addDoubleLinked(struct line **lineHead, int index);
void createHard(struct line **hard, int numberOfLines);
void showDL(line *lineHead);

/* Show */
void showHard(line *lineHead);
void endCharts(line *hardDisk, int numberOfLines);

#endif