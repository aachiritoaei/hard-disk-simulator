#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

/* Circular list element */
typedef struct node{
	int index;
	int damage;
	char *data;
	struct node *next;
}node, nod;

/* Double linked list element */
typedef struct line{
	int index;
	struct node* circular;
	struct line *next;
	struct line *previous;
}line;
/* Cursor */

typedef struct cursor{
	struct node* CL;
	struct line* DL;
}cursor;

/* Command structure */
typedef struct command{
	char *type;
	char *data;
	int indexDL; //index double linked list
	int indexCL; //index circular list
	int waitTimer;
	int isExecuted;
	int no;
	struct command *next;
}command;

#endif