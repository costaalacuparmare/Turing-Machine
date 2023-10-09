#include "turing.h"

/* auxiliary functions for data structures */

/* queue functions */

/* initializes the queue */
TQueue InitQ() {
    TQueue Q = (TQueue) malloc(sizeof(struct queue));
    if (!Q) return NULL;
    Q->front = Q->rear = NULL;
    return Q;
}

/* adds a command to the queue */
void PushQ(TQueue *Q, TCmd x) {
    TListQ aux = (TListQ) malloc(sizeof(TCellQ));
    if (!aux) return;
    aux->info = (TCmd) malloc(sizeof(TCmd));
    if (!aux->info) return;
    aux->info->code = x->code;
    aux->info->param = x->param;
    aux->next = NULL;
    if ((*Q)->front != NULL)
        (*Q)->rear->next = aux;
    else
        (*Q)->front = aux;
    (*Q)->rear = aux;
}

/* removes a command from the queue */
void PopQ(TQueue *Q) {
    TListQ aux = NULL;
    if ((*Q)->front == NULL)
        return;
    aux = (*Q)->front;
    (*Q)->front = (*Q)->front->next;
    free(aux->info);
    free(aux);
}

/* frees the heap memory space used for the queue */
void FreeQ(TQueue *Q) {
    TListQ p = NULL;
    TListQ aux = NULL;
    p = (*Q)->front;
    while (p) {
        aux = p;
        p = p->next;
        free(aux->info);
        free(aux);
    }
    free(*Q);
    *Q = NULL;
}

/* stack functions */

/* initializes the stack */
TStack InitS() {
    TStack s = (TStack) malloc(sizeof(struct stack));
    if (!s) return NULL;
    s->top = NULL;
    return s;
}

/* adds a tape pointer to the stack */
void PushS(TStack *S, TListT x) {
    TListS aux = (TListS) malloc(sizeof(TCellS));
    if (!aux) return;
    aux->info = x;
    if ((*S)->top != NULL)
        aux->next = (*S)->top;
    else
        aux->next = NULL;
    (*S)->top = aux;
}

/* removes a tape pointer from the stack */
void PopS(TStack *S) {
    TListS aux = NULL;
    if ((*S)->top == NULL) return;
    aux = (*S)->top;
    (*S)->top = (*S)->top->next;
    free(aux);
}

/* frees the heap memory space used for the stack */
void FreeS(TStack *S) {
    TListS p = NULL;
    TListS aux = NULL;
    p = (*S)->top;
    while (p) {
        aux = p;
        p = p->next;
        free(aux);
    }
    free(*S);
    *S = NULL;
}

/* tape functions */

/* initializes the tape */
TTape InitT() {
    /* creates the tape returning the head and inserting
     * the first element '#' from where current starts */
    TTape aux = (TTape) malloc(sizeof(struct tape));
    if (!aux) return NULL;
    aux->head = (TListT) malloc(sizeof(TCellT));
    if (!aux->head) {
        free(aux);
        return NULL;
    }
    TListT aux2 = (TListT) malloc(sizeof(TCellT));
    if (!aux2) return NULL;
    aux->head->prev = NULL;
    aux->head->next = aux2;
    aux2->info = '#';
    aux2->prev = aux->head;
    aux2->next = NULL;
    aux->current = aux2;
    return aux;
}

/* adds a character to the tape */
TListT PushT(char x) {
    TListT aux = (TListT) malloc(sizeof(TCellT));
    if (!aux) return NULL;
    aux->info = x;
    aux->prev = aux->next = NULL;
    return aux;
}

/* frees the heap memory space used for the tape */

void FreeT(TTape *T) {
    TListT p = NULL;
    TListT aux = NULL;
    p = (*T)->head;
    while (p) {
        aux = p;
        p = p->next;
        free(aux);
    }
    free(*T);
    *T = NULL;
}

/* command functions */

/* codes the commands and extracts
 * the parameter from them using 'strcmp' */
int Code(char *cmd, char *param) {
    if (strcmp(cmd, "EXECUTE\n") == 0 || strcmp(cmd, "EXECUTE") == 0)
        return EXECUTE_CODE;
    if (strcmp(cmd, "MOVE_LEFT\n") == 0 || strcmp(cmd, "MOVE_LEFT") == 0)
        return MOVE_LEFT_CODE;
    if (strcmp(cmd, "MOVE_RIGHT\n") == 0 || strcmp(cmd, "MOVE_RIGHT") == 0)
        return MOVE_RIGHT_CODE;
    if (C_IN_MOVE <= strlen(cmd)) {
        if (cmd[C_IN_MOVE] == 'C' || cmd[C_IN_MOVE+1] == 'C') {
            *param = cmd[strlen(cmd) - PARAM_POS];
            if (cmd[L_IN_MOVE] == 'L') {
                return MOVE_LEFT_CHAR_CODE;
            } else {
                return MOVE_RIGHT_CHAR_CODE;
            }
        }
    }
    if (cmd[0] == 'W') {
        *param = cmd[strlen(cmd) - PARAM_POS];
        return WRITE_CODE;
    }
    if (cmd[0] == 'I') {
        *param = cmd[strlen(cmd) - PARAM_POS];
        if (cmd[L_IN_INSERT] == 'L') {
            return INSERT_LEFT_CODE;
        } else {
            return INSERT_RIGHT_CODE;
        }
    }
    if (strcmp(cmd, "SHOW_CURRENT\n") == 0 || strcmp(cmd, "SHOW_CURRENT") == 0)
        return SHOW_CURRENT_CODE;
    if (strcmp(cmd, "SHOW\n") == 0 || strcmp(cmd, "SHOW") == 0)
        return SHOW_CODE;
    if (strcmp(cmd, "UNDO\n") == 0 || strcmp(cmd, "UNDO") == 0)
        return UNDO_CODE;
    if (strcmp(cmd, "REDO\n") == 0 || strcmp(cmd, "REDO") == 0)
        return REDO_CODE;
}

/* commands */

/* executes UPDATE commands */
void EXECUTE(TTape *T, TQueue *Q, TStack *Undo, FILE *output) {
    char param = (*Q)->front->info->param;
    switch ((*Q)->front->info->code) {
        case MOVE_LEFT_CODE: {
            MOVE_LEFT(T, Undo);
            break;
        }
        case MOVE_RIGHT_CODE: {
            MOVE_RIGHT(T, Undo);
            break;
        }
        case MOVE_LEFT_CHAR_CODE: {
            MOVE_LEFT_CHAR(T, param, output);
            break;
        }
        case MOVE_RIGHT_CHAR_CODE: {
            MOVE_RIGHT_CHAR(T, param);
            break;
        }
        case WRITE_CODE: {
            WRITE(T, param);
            break;
        }
        case INSERT_LEFT_CODE: {
            INSERT_LEFT(T, param, output);
            break;
        }
        case INSERT_RIGHT_CODE: {
            INSERT_RIGHT(T, param);
            break;
        }
    }
    PopQ(Q);
}

/* UPDATE type commands */

/* moves the current to the left */
void MOVE_LEFT(TTape *T, TStack *Undo) {
    if ((*T)->current->prev->prev == NULL) return;
    (*T)->current = (*T)->current->prev;
    PushS(Undo, (*T)->current->next);
}

/* moves the current to the right and, if
 * there is no character, creates a new one,
 * represented by # and moves there */
void MOVE_RIGHT(TTape *T, TStack *Undo) {
    TListT aux = NULL;
    if ((*T)->current->next == NULL) {
        aux = PushT('#');
        if (!aux) return;
        aux->next = NULL;
        aux->prev = (*T)->current;
        (*T)->current->next = aux;
    }
    (*T)->current = (*T)->current->next;
    PushS(Undo, (*T)->current->prev);
}

/* searches for a certain character to the left
 * and, if found, moves there;
 * if the current character is equal to the searched
 * character, it keeps its position */
void MOVE_LEFT_CHAR(TTape *T, char param, FILE *output) {
    TListT p = NULL;
    int check = 0;
    if ((*T)->current->info == param)
        return;
    for (p = (*T)->current; p != (*T)->head; p = p->prev)
        if (p->info == param) {
            (*T)->current = p;
            check = 1;
            break;
        }
    if (!check)
        fprintf(output, "ERROR\n");
}

/* searches for a certain character to the right
 * and, if found, moves there;
 * if there is no character found, creates
 * a new one, represented by # and moves there;
 * if the current character is equal to the searched
 * character, it keeps its position */
void MOVE_RIGHT_CHAR(TTape *T, char param) {
    TListT p = NULL;
    int check = 0;
    if ((*T)->current->info == param)
        return;
    for (p = (*T)->current; p != NULL; p = p->next)
        if (p->info == param) {
            (*T)->current = p;
            check = 1;
            break;
        }
    if (!check) {
        TListT aux = PushT('#');
        if (!aux) {
            return;
        }
        aux->next = NULL;
        for (; (*T)->current->next != NULL; (*T)->current = (*T)->current->next)
            continue;
        aux->prev = (*T)->current;
        (*T)->current->next = aux;
        (*T)->current = (*T)->current->next;
    }
}

/* overrides the current character */
void WRITE(TTape *T, char param) {
    (*T)->current->info = param;
}

/* inserts a character to the left, if possible. and
 * moves the current on the inserted character */
void INSERT_LEFT(TTape *T, char param, FILE *output) {
    if ((*T)->current->prev->prev == NULL) {
        fprintf(output, "ERROR\n");
        return;
    }
    TListT aux = PushT(param);
    if (!aux) return;
    aux->next = (*T)->current;
    aux->prev = (*T)->current->prev;
    (*T)->current->prev->next = aux;
    (*T)->current->prev = aux;
    (*T)->current = aux;
}

/* inserts a character to the right and moves
 * the current on the inserted character */
void INSERT_RIGHT(TTape *T, char param) {
    TListT aux = PushT(param);
    if ((*T)->current->next == NULL) {
        (*T)->current->next = aux;
        aux->prev = (*T)->current;
        (*T)->current = aux;
        return;
    }
    aux->next = (*T)->current->next;
    aux->prev = (*T)->current;
    (*T)->current->next->prev = aux;
    (*T)->current->next = aux;
    (*T)->current = aux;
}

/* QUERY type commands */

/* prints the current position on the tape */
void SHOW_CURRENT(TTape *T, FILE *output) {
    fprintf(output, "%c\n", (*T)->current->info);
}

/* prints the tape in the current state */
void SHOW(TTape *T, FILE *output) {
    TListT p = NULL;
    for (p = (*T)->head->next; p != NULL; p = p->next)
        if (p == (*T)->current)
            fprintf(output, "|%c|", p->info);
        else
            fprintf(output, "%c", p->info);
    fprintf(output, "\n");
}

/* UNDO/ REDO type commands */

/* reverses MOVE_LEFT/ MOVE_RIGHT commands */
void UNDO(TTape *T, TStack *Undo, TStack *Redo) {
    PushS(Redo, (*T)->current);
    (*T)->current = (*Undo)->top->info;
    PopS(Undo);
}

/* reverses UNDO commands */
void REDO(TTape *T, TStack *Undo, TStack *Redo) {
    if (!(*Redo)->top->info->next)
        PushS(Undo, (*Redo)->top->info->prev);
    else
        PushS(Undo, (*Redo)->top->info->next);
    (*T)->current = (*Redo)->top->info;
    PopS(Redo);
}

/* implementation functions */

/* initializes all the data structures used */
void Init(TTape *T, TQueue *Q, TStack *Undo, TStack *Redo) {
    *T = InitT();
    *Q = InitQ();
    *Undo = InitS();
    *Redo = InitS();
}

/* reads and codes the commands from the input file */
TCmd Read(FILE *input) {
    char cmd[MAX] = {};
    TCmd CMD = (TCmd) malloc(sizeof(struct command));
    fgets(cmd, MAX, input);
    CMD->param = '\0';
    CMD->code = Code(cmd, &CMD->param);
    return CMD;
}

/* takes each command and runs it accordingly */
void Execute(TTape *T, TQueue *Q, TStack *Undo, TStack *Redo, TCmd cmd, FILE *output) {
    if (cmd->code >= MOVE_LEFT_CODE && cmd->code <= INSERT_RIGHT_CODE)
        PushQ(Q, cmd);
    switch (cmd->code) {
        case EXECUTE_CODE: {
            EXECUTE(T, Q, Undo, output);
            break;
        }
        case SHOW_CURRENT_CODE: {
            SHOW_CURRENT(T, output);
            break;
        }
        case SHOW_CODE: {
            SHOW(T, output);
            break;
        }
        case UNDO_CODE: {
            UNDO(T, Undo, Redo);
            break;
        }
        case REDO_CODE: {
            REDO(T, Undo, Redo);
            break;
        }
    }
    free(cmd);
}

/* runs the program in main */
void Run(TTape *T, TQueue *Q, TStack *Undo, TStack *Redo) {
    FILE *input = fopen("turing.in", "rt");
    FILE *output = fopen("turing.out", "w+");
    int cmd_nr = 0;
    fscanf(input, "%d", &cmd_nr);
    TCmd cmd = NULL;
    char s[MAX];
    fgets(s, MAX, input);
    int i = 0;
    for (; i < cmd_nr; i++) {
        cmd = Read(input);
        Execute(T, Q, Undo, Redo, cmd, output);
    }
    fclose(input);
    fclose(output);
}

/* frees the heap memory used for data structures */
void Free(TTape *T, TQueue *Q, TStack *Undo, TStack *Redo) {
    FreeT(T);
    FreeQ(Q);
    FreeS(Undo);
    FreeS(Redo);
}
