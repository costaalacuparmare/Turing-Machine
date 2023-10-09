#include "turing.h"

int main() {
    /* declaration of all data structures */
    TTape T = NULL;
    TQueue Q = NULL;
    TStack Undo = NULL;
    TStack Redo = NULL;

    /* initializes data structures */
    Init(&T, &Q, &Undo, &Redo);

    /* runs the program */
    Run(&T, &Q, &Undo, &Redo);

    /* frees the heap memory used for data structures */
    Free(&T, &Q, &Undo, &Redo);

    return 0;
}
