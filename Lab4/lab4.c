#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"


int main (void){
    struct node *a = (struct node*) malloc(sizeof(struct node));
    void *d = malloc(sizeof(int));

    *(int*)d = 8;
    a->p_data = d;
    a->next = NULL;
    a->type = 0;
    void *d2 = malloc(sizeof(double));

    *(double*)d2 = 8.91892133;


     void *d3 = malloc(sizeof(float));

    *(float*)d3 = -0.141;

      void *d4 = malloc(sizeof(float));

    *(float*)d4 = -1.112;
    insert(a, d2, 0,2);
    //print_list(a);
    insert(a, d3, 0,1);
    insert(a, d4, 1,1);
    //a = del(a,d);
    insert(a, d4, 0,1);
    del2(&a, d4);
    //free(a);
    print_list(a);
    //printf("%d\n", *(int *)(a->p_data));
    return 0;
}