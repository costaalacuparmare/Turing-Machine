#include <stdio.h>
#include <string.h>

#define MAX 1000
#define PLUS '+'
#define MINUS '-'
#define ORI '*'
#define DIV '/'

void delnum ( int *num_count, float *num[MAX]) {
    for( int i = 1; i < *num_count; i++)
        *num[i] = *num[i+1];
    *num_count--;
}

int main()
{   char op[MAX];
    int num_count = 0;
    float num[MAX];
    scanf("%d",&num_count);
    for( int i = 0; i < num_count; i++)
        scanf("%f",&num[i]);
    scanf("%s",op);
    while( op[0] != NULL) {
    switch(op[0]) {
        case PLUS: {
        num[0] = num[0] + num[1];
        delnum( &num_count,&num);
        break;
        }
        case MINUS: {
        num[0] = num[0] - num[1];
        delnum( &num_count, &num);
        break;
        }
        case ORI: {
        num[0] = num[0] * num[1];
        delnum( &num_count, &num);
        break;
        }
        case DIV: {
        num[0] = num[0] - num[1];
        delnum( &num_count, &num);
        break;
        }
    break;
    }
    strcpy(op, op+1);
}
    return 0;
}
