#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/* macros for command array and verifications */
#define MAX 20
#define PARAM_POS 2
#define C_IN_MOVE 10
#define L_IN_MOVE 5
#define L_IN_INSERT 7

/* macros for command coding */
#define EXECUTE_CODE 0
#define MOVE_LEFT_CODE 1
#define MOVE_RIGHT_CODE 2
#define MOVE_LEFT_CHAR_CODE 3
#define MOVE_RIGHT_CHAR_CODE 4
#define WRITE_CODE 5
#define INSERT_LEFT_CODE 6
#define INSERT_RIGHT_CODE 7
#define SHOW_CURRENT_CODE 8
#define SHOW_CODE 9
#define UNDO_CODE 10
#define REDO_CODE 11

/* declaring data structures */

/* memory tape: which is implemented
 * using a doubly linked list, in which
 * each cell has a character */

typedef struct cellT {
    char info;
    struct cellT *prev, *next;
} TCellT, *TListT;

/* the tape consists of the head of
 * the doubly linked list and current,
 * the position on the tape that is currently
 * occupied by the user */

typedef struct tape {
    TListT head;
    TListT current;
} *TTape;

/* the UNDO/ REDO command are
 * implemented using stacks that memorise
 * a pointer to the current position before
 * executing a command such as UNDO or REDO */

typedef struct cellS {
    TListT info;
    struct cellS* next;
} TCellS, *TListS;

typedef struct stack {
    TListS top;
} *TStack;

/* the commands are coded for a simplified
 * implementation using a structure that
 * memorises the code of the command, as well
 * as the parameter for commands that modify
 * the tape in any way*/

typedef struct command {
    int code;
    char param;
} *TCmd;

/* the UPDATE type commands are memorised in a queue,
 * waiting to be executed one by one when the EXECUTE
 * commands are read from the input file */

typedef struct cellQ {
    TCmd info;
    struct cellQ* next;
} TCellQ, *TListQ;

typedef struct queue {
    TListQ front, rear;
} *TQueue;

/* auxiliary functions for data structures */

/* queue functions */

/* initializes the queue */
TQueue InitQ();

/* adds a command to the queue */
void PushQ(TQueue *Q, TCmd x);

/* removes a command from the queue */
void PopQ(TQueue *Q);

/* frees the heap memory space used for the queue */
void FreeQ(TQueue *Q);

/* stack functions */

/* initializes the stack */
TStack InitS();

/* adds a tape pointer to the stack */
void PushS(TStack *S, TListT x);

/* removes a tape pointer from the stack */
void PopS(TStack *S);

/* frees the heap memory space used for the stack */
void FreeS(TStack *S);

/* tape functions */

/* initializes the tape */
TTape InitT();

/* adds a character to the tape */
TListT PushT(char x);

/* frees the heap memory space used for the tape */
void FreeT(TTape *T);

/* command functions */

/* codes the commands and extracts
 * the parameter from them */
int Code(char *cmd, char *param);

/* commands */

/* executes UPDATE commands */
void EXECUTE(TTape *T, TQueue *Q, TStack *Undo, FILE *output);

/* UPDATE type commands */

/* moves the current to the left */
void MOVE_LEFT(TTape *T, TStack *Undo);

/* moves the current to the right and, if
 * there is no character, creates a new one,
 * represented by # and moves there */
void MOVE_RIGHT(TTape *T, TStack *Undo);

/* searches for a certain character to the left
 * and, if found, moves there;
 * if the current character is equal to the searched
 * character, it keeps its position */
void MOVE_LEFT_CHAR(TTape *T, char param, FILE *output);

/* searches for a certain character to the right
 * and, if found, moves there;
 * if there is no character found, creates
 * a new one, represented by # and moves there;
 * if the current character is equal to the searched
 * character, it keeps its position */
void MOVE_RIGHT_CHAR(TTape *T, char param);

/* overrides the current character */
void WRITE(TTape *T, char param);

/* inserts a character to the left, if possible. and
 * moves the current on the inserted character */
void INSERT_LEFT(TTape *T, char param, FILE *output);

/* inserts a character to the right and moves
 * the current on the inserted character */
void INSERT_RIGHT(TTape *T, char param);

/* QUERY type commands */

/* prints the current position on the tape */
void SHOW_CURRENT(TTape *T, FILE *output);

/* prints the tape in the current state */
void SHOW(TTape *T, FILE *output);

/* UNDO/ REDO type commands */

/* reverses MOVE_LEFT/ MOVE_RIGHT commands */
void UNDO(TTape *T, TStack *Undo, TStack *Redo);

/* reverses UNDO commands */
void REDO(TTape *T, TStack *Undo, TStack *Redo);

/* implementation functions */

/* initializes all the data structures used */
void Init(TTape *T, TQueue *Q, TStack *Undo, TStack *Redo);

/* reads and codes the commands from the input file */
TCmd Read(FILE *input);

/* takes each command and runs it accordingly */
void Execute(TTape *T, TQueue *Q, TStack *Undo, TStack *Redo,
             TCmd cmd, FILE *output);

/* runs the program in main */
void Run(TTape *T, TQueue *Q, TStack *Undo, TStack *Redo);

/* frees the heap memory used for data structures */
void Free(TTape *T, TQueue *Q, TStack *Undo, TStack *Redo);

