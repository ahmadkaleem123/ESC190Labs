#if !defined(LINKED_LIST)
#define LINKED_LIST

struct node{
    void *p_data;
    struct node *next;
    int type; //0 = int, 1 = float, 2 = double
};

void insert(struct node *head, void *data, int pos, int data_type);

struct node* del(struct node *head, void *data);

void del2(struct node **head, void *data);

void free_list(struct node *head);

void print_list(struct node *head);

#endif