#include <stdio.h>
#include <stdlib.h>

struct node{
    int data; 
    struct node *next; 
};

void print_list(struct node* head){
    struct node* cur = head;
    printf("%d, ", cur->data);
    cur = cur ->next;
    while(cur != NULL){
        printf("%d, ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}

void print_list_recur(struct node* head ){
    if (head != NULL){
        printf("%d, ", head->data);
        print_list_recur(head->next);
    }
    else{   
        printf("\n");
    }
    
    return;
}
struct node1{
    void *p_data; //a pointer to data (allocated with malloc)
    int type; // 0 if int, 1 if float, 2 if double
    struct node1 *next;
};
void append(struct node1* head, int type, void* value){
    struct node1 *cur = head;
    while (cur-> next != NULL){
        cur = cur->next;
    }
    if (type == 0){
        struct node1* nodea = (struct node1*) malloc(sizeof(struct node1));
        nodea -> p_data = value;
       
        nodea -> type = 0;
        nodea -> next = NULL;
        cur -> next = nodea;
    }
    else if (type == 1){
        
        struct node1* nodea = (struct node1*) malloc(sizeof(struct node1));
        nodea -> p_data = value;
        nodea -> type = 1;
        nodea -> next = NULL;
        cur -> next = nodea;
    }
    else{
        struct node1* nodea = (struct node1*) malloc(sizeof(struct node1));
        nodea -> p_data = value;
        nodea -> type = 2;
        nodea -> next = NULL;
        cur -> next = nodea;
    }
}
void print(struct node1* head){
    struct node1 *cur = head;
    while (cur != NULL){
        if(cur -> type == 0){ // integer
            printf("%d, ", *(int*)(cur->p_data));
            cur = cur->next;
        }
        else if(cur->type == 1){ //float
            printf("%f, ", *(float*)(cur->p_data));
            cur = cur->next;

        }
        else{   //double
            printf("%.15f, ", *(double*)(cur->p_data));
            cur = cur->next;
        }
    }
    printf("\n");

}

int main (void){
    /*struct node* node0 = (struct node*) malloc(sizeof(struct node));
    node0 -> data = 0;
    
    struct node* node1 = (struct node *)malloc(sizeof(struct node));
    node0->next = node1;
    node1->data = 30;
    struct node* node2 = (struct node *)malloc(sizeof(struct node));
    node1->next = node2;
    node2->data = 10;
    node2->next = NULL;
    print_list_recur(node0);
    printf("%d \n", node2->data);*/

    struct node1* nodea = (struct node1*) malloc(sizeof(struct node1));
    nodea -> p_data =   malloc(sizeof(int));
    //printf("%s",type(nodea -> p_data));
    *(int*)(nodea -> p_data) = 12;
    //printf("%d", *(int*)(nodea -> p_data) );
    nodea -> type = 0;
    struct node1* nodeb = (struct node1 *)malloc(sizeof(struct node1));
    nodea->next = nodeb;
    nodeb->p_data =   malloc(sizeof(float));
    *(float*)(nodeb -> p_data) = 2.38;
    nodeb -> type = 1;
    struct node1* nodec = (struct node1 *)malloc(sizeof(struct node1));
    nodeb->next = nodec;
    nodec->p_data =  malloc(sizeof(double));
    *(double*)(nodec -> p_data) = 3.14159;
    nodec->type = 2;
    nodec->next = NULL;
    print(nodea);
    void* new_node = malloc(sizeof(float));
    *(float*) new_node = 2.71;
    append(nodea, 1, new_node);
    print(nodea);
    void* x = malloc(sizeof(double));
    *(double*) x = 9.816753489052480987;
    append(nodea, 2, x);
    print(nodea);
}