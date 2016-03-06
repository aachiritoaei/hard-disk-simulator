HARD DISK SIMULATOR
===================

About
-----

This hard disk simulator was one of my homeworks during my first year of study in Computer Science.

Implementation
--------------

The simulator is implemented using lists, stacks and queues.

Description
-----------

The hard disk is composed of a disk on which information is written and a cursor. 

The disk is composed of multiple circular lists, each with double the capacity of the previous.

For example, if the first circular list has 16 positions, the next one has 32 and so on.

The cursor is able to move from a circular list to another only from its starting index, 0.


Execution will be done using 'a time division'. On each 'time divison', the cursor can either
execute a command, move to another address or 'do nothing'.

There are multiple COMMANDS that can be given to the disk:

READ DATA - ::r [line] [index]

WRITE DATA - ::w [line] [index] [data]

READ DAMAGE - ::d [line] [index]

END - ::e

MULTIPLE READ DATA - ::mr [line] [index] [numberOfReads]

MULTIPLE WRITE DATA - ::mw [line] [index] [data] [data] ... [data]


Each command aplies damage to the disk, as referenced :

READ DATA - 5 Damage

WRITE DATA - 30 Damage

READ DAMAGE - 2 Damage

'time division' - 1 Damage(each time division spent on an address adds 1 damage to it)


INPUT AND OUTPUT EXAMPLES
-------------------------

1)
- input

1 5
::r
20
::w
1
::r
100
::d
1
::d
1
::e

- ouput
0000
AA2A
58
61
0.20 0.19 0.19 0.75