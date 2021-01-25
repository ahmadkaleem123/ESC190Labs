#include <stdio.h>

/*void f(int *n){
    *n = 10;
}*/

void print_list(int l[], int size){
    for(int i = 0; i < size; i++){
       printf("%d\n", l[i]);
   }
}
void insertion_sort(int l[], int size){
  
    int i = 1;
    while(i < size){
        
        int j = i;
        while(j > 0 && l[j-1] > l[j]){
            int temp = l[j];
            l[j] = l[j-1];   
            l[j-1] = temp;
            j--;
        }
        i++; // same as i = i + 1
    }
}
int main(void){
    int x[7] ={3,2,7,3,2,-1,9};
    insertion_sort(x, sizeof(x)/sizeof(x[0]));
    print_list(x, sizeof(x)/sizeof(x[0]));
    // int a = 5;
    // f(&a);
    // printf("%d\n", a);
}