#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
int my_strlen(char *s1){
    int len = 0;
        while(*s1 != '\0'){
            len++;
            s1++; //increases the s1 pointer to the next address 

        }
        return len;
}

char *my_strcat(char *dest, char *src){
    int x = my_strlen(dest);
    int y = my_strlen(src);
    char *dest2 = (char *) malloc((x+y+1) * sizeof(char)); //+1 to allocate space for the NULL pointer, pointer to first character in our new destination string
    char *new_dest = dest2;
    while(*dest != '\0'){
        *new_dest = *dest;
        dest++;
        new_dest++;
    }
    while(*src != '\0'){
        *new_dest = *src;
        src++;
        new_dest++;
    }
    *new_dest = '\0';
    //dest = dest2;
    //free(dest2);

    return dest2;
}


int my_strcmp_rec(char *str1, char *str2){
    if(*str1 == '\0' && *str2 == '\0' ){
       
        return 0;
    }
    else if (*str1 == '\0'){
        return 0 - *str2;
    }
    else if (*str2 == '\0'){
        return *str1 - 0;
    }
    else if (*str1 != *str2){
        return *str1 - *str2;
    }
    else{
        return  my_strcmp_rec(str1+1, str2+1);
    }
    /*while(*str1 != '\0' && *str2 != '\0'){
        if(*str1 != *str2){
            return *str1 - *str2;
        }
        str1++;
        str2++;

    }
    if(*str1 == '\0' && *str2 == '\0' ){
        return 0;
    }
    else if (*str1 == '\0'){
        return 0 - *str2;
    }
    else{
        return *str1 - 0;
    }
    ///we intepreted a "NULL" pointer as having value 0.
    */
}

int my_atoi(char *str){//note function signature was made my_atoi instead of atoi to avoid the error as atoi is already defined in stdlib.h
    int output = 0;
    
    bool is_neg = false;
    char *cur = str;
    int len = my_strlen(str);
    int pos = len-1;
    while(*cur != '\0'){
        if(cur == str){
            if(*cur == '-'){
                is_neg = true;
                pos--;
                cur++;
            }
         
        } 
        /*if(pos==len-1 && is_neg == true){
            pos--;
            cur++;
            continue;
        }*/
        if(isdigit(*cur) == 0){
            return 0;
        }
    
        else{
            output += (*cur - '0') * pow(10, pos);
        }
        pos--;
        cur++;
    }

    if(is_neg){
        return (-1)*output;
    }
    return output;   
}

struct node{
    void *data;
    struct node *next;
};
int tortoise_hare (struct node* head){
    struct node *hare = head;
    struct node *tortoise = head;
    hare = hare->next;
    if(hare == NULL){
        return 0;
    }
    hare = hare->next;
    if(hare == NULL){
        return 0;
    }
    tortoise = tortoise -> next;

    while (hare != tortoise){
        //printf("%d\n", 9000);
        hare = hare->next;
        if(hare == NULL){
            return 0;
        }
        hare = hare->next;
        if(hare == NULL){
            return 0;
        }
        tortoise = tortoise -> next;

    }
    return 1;
}

int main (void){
    char *s1 = "every";

    // printf("%s\n", s1);
    char *s2 = "everyone";
    s1 = my_strcat(s1, s2);
    //printf("%s\n",  s1);
    //printf("%s\n", s1);
    //printf("%d\n", my_strcmp_rec(s1, s2));
    //printf("%d\n", atoi(s1));
    //char *s3 = "0021";
    //int a = my_atoi(s3);
    
   // printf("%d\n", a);
    struct node *a = (struct node *)malloc(sizeof(struct node));

  //  struct node *b = (struct node *)malloc(sizeof(struct node));
    a->next = a;
/*
    struct node *c = (struct node *)malloc(sizeof(struct node));
    b->next = c;
    struct node *d = (struct node *)malloc(sizeof(struct node));
    c->next = d;
    d->next = a;8?
*/
    printf("%d\n",tortoise_hare(a));

  

    return 0;
}