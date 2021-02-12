#include <stdio.h>
#include "linkedlist.h"
#include <stdlib.h>


void insert(struct node *head, void *data, int pos, int data_type){

   // struct node *start = head;
    int p = 0;
    
    while(p != pos){
        head = head->next;
        p++;
        if(head == NULL){
            printf("Error: Index out of Range!\n");
            return;
        }

    }

   /* if (p==0){
        struct node *new_node = (struct node*)malloc(sizeof(struct node));
        new_node->p_data = data;
        new_node->next = head;
        new_node->type = data_type;
       // start = new_node;

    }*/
    if(head->next == NULL){
        struct node *new_node = (struct node*)malloc(sizeof(struct node));
        new_node->p_data = data;
        new_node->next = NULL;
        new_node->type = data_type;

        head->next = new_node; 

        
    }
    else{
        struct node *new_node = (struct node*)malloc(sizeof(struct node));
        new_node->p_data = data;
        new_node->next = head->next;
        new_node->type = data_type;

        head->next = new_node;
    }
    //return start;

    
}

struct node * del(struct node *head, void *data){
    int pos = 0;
    struct node *cur = head;
    struct node *prev = head;
    while(cur != NULL){
        if(cur->p_data == data){
            prev->next = cur->next;
            if(pos == 0){
                return prev->next;
            }
            free(cur);
            return head;
        }
        prev = cur;
        cur = cur->next;
        pos++;
    }
    return head;
}

void del2(struct node **head, void *data){
    struct node *cur = *head;
    struct node *prev = *head;
    int pos = 0;
    while (cur != NULL){
        if(cur->p_data == data){
            if(pos == 0){
                *head = (*head)->next;
                free(cur);
                return;
            }
            else{
                prev->next = cur->next;
                free(cur);
                return;
            }
        }
        prev = cur;
        cur = cur->next;
        pos++;
        
    }
}

void free_list(struct node *head){
    struct node *cur = head;
    while(head != NULL){
        cur = head->next;
        free(head);
        head = cur;
    }
}

void print_list(struct node *head){
    

    while(head != NULL){
        if(head->type == 0){
            printf("%d\n", *(int *)(head->p_data));
        }
        else if(head->type == 1){
            printf("%f\n", *(float *)(head->p_data));
        }
        else if(head->type == 2){
            printf("%lf\n", *(double *)(head->p_data));
        }
        head = head->next;
    }

}
/*int main(void){


    return 0;
}*/