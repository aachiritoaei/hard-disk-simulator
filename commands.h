#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "structures.h"

/* Commands */

/* ::r */
void readData(struct cursor **cursor);
/* ::d */
void readDamage(struct cursor **cursor);
/* ::w */
void writeData(struct cursor **cursor, char *string);
/* Move cursor one line */
line *moveOneLine(line *cursor, int indexDL);
/* Move cursor one address*/
node* moveOneAddress(node *cursor);

/* Case 1 - queue implementation */
void pushQueueCommand(command **head, command *new);
command *popQueueCommand(command **head);

/* Case 2 - stack implementation */
void pushStackCommand(command **head, command *new);
command *popStackCommand(command **head);

#endif