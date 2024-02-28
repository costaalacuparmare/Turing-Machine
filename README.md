# Turing Machine

>This file contains the all around subject of the project and file contents

## Constantinescu Vlad

1. Subject:

Turing Machine:
    
    - I implemented a turing machine using data structures and input commands
    - The user of this code can use certain commands to manipulate the tape
    - The commands with explanations are listed below:

        UPDATE COMMANDS:

        - MOVE_LEFT moves the current position to the left w/ one cell
   
        - MOVE_RIGHT moves the current position to the right w/ one cell
        if there is nothing to the right, inserts '#' char and moves on it

        - MOVE_LEFT_CHAR <param> moves the current position to the left at
        the specified char. If it isn't found, or the current char is the one
        searched for, it doesn't change the current position

        - MOVE_RIGHT_CHAR <param> moves the current position to the right at
        the specified char. If it isn't found, inserts '#' char and moves on it.
        If the current char is the one searched for, it doesn't change the current
        position

        - WRITE <param> writes the desired char over the current one, without
        modifying current position

        - INSERT_LEFT <param> inserts the specified char to the left of the current
        position. it it's the first cell, prints ERROR

        - INSERT_RIGHT <param> inserts the specified char to the right of the current
        position

        QUERY COMMANDS:

        - SHOW_CURRENT show the current position

        - SHOW show the whole tape

        UNDO/REDO:

        - UNDO moves the current position to the previous position

        - REDO moves the current position to the position before the UNDO command

        EXECUTE COMMANDS:

        - EXECUTE used to execute one UPDATE command at the time, therefore is not
        available before such a commands is used

    -Restrictions:

        - The UPDATE commands are stored in a queue, where they are executed one by one
        by EXECUTE commands
        - The QUERY commands are executed at anytime
        - UNDO only works with MOVE_LEFT and MOVE_RIGHT, therefore it must
        be used after one of these two commands has been implemented
        - REDO can be used only after an UNDO has been used

2. File contents:

turing_functions.h:

    - macros for each command code and verifications
    - structures for defining the commands, the tape,
    command queues and UNDO/REDO stacks
    - the header of the functions for manipulating the structures
    of related data, as well as the requested orders and auxiliary functions

main.c:

    - declaration of all data structures
    - Init (data structure initialization)
    - Run (Running the program)
    - Free (release of data structures from memory)

turing_functions.c - declarations of functions:

    - auxiliary functions:
        - tape operation: InitB, PushB, FreeB
        - queue operation: InitC, PushC, PopC
        - stack operation: InitS, PushS, PopS, FreeS
    - Code: coding of commands in integer values using the detection of
    certain characters from the command string and comparison by
    the 'strcmp' function. The parameters of parameterized processing
    functions are modified secondarily by the function
    - Read: reading the command from the file and its coding
    - Execute: a switch case for command execution, if there are
    UPDATE commands, add them to the queue
    - Run: reads a number of commands from the file, encodes each one
    and runs the execution function for each encoded command (Execute)
    - Free: frees the data structures declared in main.c
    - command functions: the functions that the user can access
