/* FILE avl_bag.c
 *    Implementation of the bag ADT using an AVL tree.
 * Author: Francois Pitt, March 2012.
 */

/******************************************************************************
 *  Types and Constants.                                                      *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "bag.h"

/* MACRO HEIGHT
 *    An expression for one more than the height of a node in an AVL tree
 *    (evaluates to 0 if node is NULL).
 * Side-effects:  evaluates its argument more than once
 */
#define HEIGHT(node) ((node) ? (node)->height : 0)
////////////////////////////////////////////////////////////////////////////////
//  Example of macros and side-effects interfering with one another:          //
//      #define MAX(x,y)  ((x) >= (y) ? (x) : (y))                            //
//  Consider this code:                                                       //
//      printf("%d", MAX(a++,b - 5));                                         //
//  This is replaced textually before compilation:                            //
//      printf("%d", ((a++) >= (b - 5) ? (a++) : (b - 5)));                   //
//  If a++ >= b - 5, then a++ is evaluated TWICE!                             //
////////////////////////////////////////////////////////////////////////////////

/* TYPE avl_note_t -- A node in an AVL tree. */
typedef struct avl_node {
    bag_elem_t elem;        /* the element stored in this node       */
    size_t height;          /* one more than the height of this node */
    struct avl_node *left;  /* pointer to this node's left child     */
    struct avl_node *right; /* pointer to this node's right child    */
} avl_node_t;

/* TYPE struct bag -- Definition of struct bag from the header. */
struct bag {
    avl_node_t *root; /* root of the AVL tree storing the elements */
    size_t size;      /* number of elements in this bag            */
    int (*cmp)(bag_elem_t, bag_elem_t); /* function to compare elements */
};

/******************************************************************************
 *  Declarations of helper functions -- including full documentation.         *
 ******************************************************************************/

/* FUNCTION avl_destroy
 *    Free the memory allocated for the binary tree rooted at a given node.
 * Parameters and preconditions:
 *    root: the root of the tree to free
 * Return value:  none
 * Side-effects:
 *    all the memory allocated for nodes in the subtree rooted at root has been
 *    freed
 */
static
void avl_destroy(avl_node_t *root);

/* FUNCTION avl_elems
 *    Fill an array with the elements in a BST, given its root.  Place the
 *    elements in sorted order, starting at the given index, and return the
 *    number of elements copied.
 * Parameters and preconditions:
 *    root: the root of the BST containing the values to be copied
 *    array: the array into which to copy the values
 *    index: the index of array where to copy the first value
 * Return value:
 *    the number of elements copies into array
 * Side-effects:
 *    elements are copied from the BST rooted at root into array
 */
static
size_t avl_elems(const avl_node_t *root, bag_elem_t *array, size_t index);

/* FUNCTION avl_traverse
 *    Call a function on every element in a BST, given its root.
 * Parameters and preconditions:
 *    root: the root of the BST to traverse
 *    fun != NULL: a pointer to a function to apply to each element in the tree
 * Return value:  none
 * Side-effect:
 *    function fun has been called on each element in the tree rooted at root,
 *    in order
 */
static
void avl_traverse(const avl_node_t *root, void (*fun)(bag_elem_t));

/* FUNCTION avl_contains
 *    Return whether or not a BST contains a certain element, given the root.
 * Parameters and preconditions:
 *    root: the root of the BST to search
 *    elem: the element to search for
 *    cmp != NULL: the comparison function to use for the search
 * Return value:
 *    true if the BST rooted at 'root' contains elem; false otherwise
 * Side-effects:  none
 */
static
bool avl_contains(const avl_node_t *root, bag_elem_t elem,
                  int (*cmp)(bag_elem_t, bag_elem_t));

/* FUNCTION avl_insert
 *    Add an element to a BST, given a pointer to its root.
 * Parameters and preconditions:
 *    root: a pointer to the root of the BST into which to insert
 *    elem: the element to insert
 *    cmp != NULL: the comparison function to use to find the insertion point
 * Return value:
 *    true if elem was inserted; false in case of error
 * Side-effects:
 *    memory has been allocated for the new element, and the tree structure has
 *    been adjusted accordingly
 */
static
bool avl_insert(avl_node_t **root, bag_elem_t elem,
                int (*cmp)(bag_elem_t, bag_elem_t));

/* FUNCTION avl_remove
 *    Remove an element from a BST, given a pointer to its root.
 * Parameters and preconditions:
 *    root: a pointer to the root of the BST into which to remove
 *    elem: the element to remove
 *    cmp != NULL: the comparison function to use to find the removal point
 * Return value:
 *    true if elem was removed; false if the element was not there
 * Side-effects:
 *    memory has been freed for the element removed, and the tree structure has
 *    been adjusted accordingly
 */
static
bool avl_remove(avl_node_t **root, bag_elem_t elem,
                int (*cmp)(bag_elem_t, bag_elem_t));

/* FUNCTION avl_remove_min
 *    Remove and return the smallest element in a BST, given a pointer to its
 *    root.
 * Parameters and preconditions:
 *    root: a pointer to the root of the BST
 * Return value:
 *    the smallest element in the BST rooted at 'root'
 * Side-effects:
 *    memory has been freed for the node containing the smallest element, and
 *    the tree structure has been adjusted accordingly
 */
static
bag_elem_t avl_remove_min(avl_node_t **root);

/* FUNCTION avl_remove_max
 *    Remove and return the largest element in a BST, given a pointer to its
 *    root.
 * Parameters and preconditions:
 *    root: a pointer to the root of the BST
 * Return value:
 *    the largest element in the BST rooted at 'root'
 * Side-effects:
 *    memory has been freed for the node containing the largest element, and
 *    the tree structure has been adjusted accordingly
 */
static
bag_elem_t avl_remove_max(avl_node_t **root);

/* FUNCTION avl_rebalance_to_the_left
 *    Rebalance the subtree rooted at *root, given that its right subtree is too
 *    tall -- this involves performing either a single or a double rotation.
 * Parameters and preconditions:
 *    root != NULL: a pointer to the root of the tree to rebalance
 *                  (*root != NULL and (*root)->right != NULL)
 * Return value:  none
 * Side-effects:
 *    the subtree rooted at *root has been rebalanced, and the heights of each
 *    node involved have been updated appropriately
 */
static
void avl_rebalance_to_the_left(avl_node_t **root);

/* FUNCTION avl_rebalance_to_the_right
 *    Rebalance the subtree rooted at *root, given that its left subtree is too
 *    tall -- this involves performing either a single or a double rotation.
 * Parameters and preconditions:
 *    root != NULL: a pointer to the root of the tree to rebalance
 *                  (*root != NULL and (*root)->left != NULL)
 * Return value:  none
 * Side-effects:
 *    the subtree rooted at *root has been rebalanced, and the heights of each
 *    node involved have been updated appropriately
 */
static
void avl_rebalance_to_the_right(avl_node_t **root);

/* FUNCTION avl_rotate_to_the_left
 *    Perform a single rotation of *parent to the left -- the tree structure
 *    goes from    *parent         to          child
 *                  /   \                      /   \
 *                 A     child           *parent    C
 *                       /   \            /   \
 *                      B     C          A     B
 * Parameters and precondition:
 *    parent != NULL: a pointer to the root of the tree to rotate
 *                    (*parent != NULL and (*parent)->right != NULL)
 * Return value:  none
 * Side-effects:
 *    the subtree rooted at *parent has been modified by rotating *parent with
 *    its right child
 */
static
void avl_rotate_to_the_left(avl_node_t **parent);

/* avl_rotate_to_the_right
 *    Perform a single rotation of *parent to the right -- the tree structure
 *    goes from         *parent    to     child
 *                       /   \            /   \
 *                  child     C          A    *parent
 *                  /   \                      /   \
 *                 A     B                    B     C
 * Parameters and precondition:
 *    parent != NULL: a pointer to the root of the tree to rotate
 *                    (*parent != NULL and (*parent)->left != NULL)
 * Return value:  none
 * Side-effects:
 *    the subtree rooted at *parent has been modified by rotating *parent with
 *    its left child
 */
static
void avl_rotate_to_the_right(avl_node_t **parent);

/* FUNCTION avl_update_height
 *    Update the height of a node (based on the heights of its children).
 * Parameters and preconditions:
 *    node != NULL: the node to update
 * Return value:  none
 * Side-effects:
 *    the height of node is updated
 */
static
void avl_update_height(avl_node_t *node);

/* FUNCTION avl_node_create
 *    Create a new avl_node.
 * Parameters and preconditions:
 *    elem: the element to store in the new node
 * Return value:
 *    pointer to a new node that stores elem and whose children are both NULL;
 *    NULL in case of error with memory allocation
 * Side-effects:
 *    memory has been allocated for the new node
 */
static
avl_node_t *avl_node_create(bag_elem_t elem);

/******************************************************************************
 *  Definitions of "public" functions -- see header file for documentation.   *
 ******************************************************************************/

bag_t *bag_create(int (*cmp)(bag_elem_t, bag_elem_t))
{
    bag_t *bag = malloc(sizeof(bag_t));
    if (bag) {
        bag->size = 0;
        bag->root = NULL;
        bag->cmp = cmp;
    }
    return bag;
}

void bag_destroy(bag_t *bag)
{
    avl_destroy(bag->root);
    free(bag);
}

size_t bag_size(const bag_t *bag)
{
    return bag->size;
}

size_t bag_elems(const bag_t *bag, bag_elem_t *array)
{
    return avl_elems(bag->root, array, 0);
}

void bag_traverse(const bag_t *bag, void (*fun)(bag_elem_t))
{
    avl_traverse(bag->root, fun);
}

bool bag_contains(const bag_t *bag, bag_elem_t elem)
{
    return avl_contains(bag->root, elem, bag->cmp);
}

bool bag_insert(bag_t *bag, bag_elem_t elem)
{
    if (avl_insert(&bag->root, elem, bag->cmp)) {
        bag->size++;
        return true;
    } else {
        return false;
    }
}

bool bag_remove(bag_t *bag, bag_elem_t elem)
{
    if (avl_remove(&bag->root, elem, bag->cmp)) {
        bag->size--;
        return true;
    } else {
        return false;
    }
}

/******************************************************************************
 *  Definitions of helper functions -- see above for documentation.           *
 ******************************************************************************/

void avl_destroy(avl_node_t *root)
{
    if (root) {
        avl_destroy(root->left);
        avl_destroy(root->right);
        free(root);
    }
}

size_t avl_elems(const avl_node_t *root, bag_elem_t *array, size_t index)
{
    size_t count = 0; /* number of elements copied so far */
    if (root) {
        count += avl_elems(root->left, array, index + count);
        array[index + count++] = root->elem;
        count += avl_elems(root->right, array, index + count);
    }
    return count;
}

void avl_traverse(const avl_node_t *root, void (*fun)(bag_elem_t))
{
    if (root) {
        avl_traverse(root->left, fun);
        (*fun)(root->elem);
        avl_traverse(root->right, fun);
    }
}

bool avl_contains(const avl_node_t *root, bag_elem_t elem,
                  int (*cmp)(bag_elem_t, bag_elem_t))
{
    if (! root)
        return false;
    else if ((*cmp)(elem, root->elem) < 0)
        return avl_contains(root->left, elem, cmp);
    else if ((*cmp)(elem, root->elem) > 0)
        return avl_contains(root->right, elem, cmp);
    else /* ((*cmp)(elem, root->elem) == 0) */
        return true;
}

bool avl_insert(avl_node_t **root, bag_elem_t elem,
                int (*cmp)(bag_elem_t, bag_elem_t))
{
    bool inserted;

    if (! *root) {
        inserted = (*root = avl_node_create(elem));
    } else if ((*cmp)(elem, (*root)->elem) < 0) {
        if ((inserted = avl_insert(&(*root)->left, elem, cmp))) {
            /* Check if the subtree needs rebalancing; update its height. */
            if (HEIGHT((*root)->left) > HEIGHT((*root)->right) + 1)
                avl_rebalance_to_the_right(root);
            else
                avl_update_height(*root);
        }
    } else if ((*cmp)(elem, (*root)->elem) > 0) {
        if ((inserted = avl_insert(&(*root)->right, elem, cmp))) {
            /* Check if the subtree needs rebalancing; update its height. */
            if (HEIGHT((*root)->right) > HEIGHT((*root)->left) + 1)
                avl_rebalance_to_the_left(root);
            else
                avl_update_height(*root);
        }
    } else { /* ((*cmp)(elem, (*root)->elem) == 0) */
        /* Insert into the subtree with smaller height. */
        if (HEIGHT((*root)->left) < HEIGHT((*root)->right))
            inserted = avl_insert(&(*root)->left, elem, cmp);
        else
            inserted = avl_insert(&(*root)->right, elem, cmp);
        /* No rebalancing necessary, but update height. */
        if (inserted)  avl_update_height(*root);
    }

    return inserted;
}

bool avl_remove(avl_node_t **root, bag_elem_t elem,
                int (*cmp)(bag_elem_t, bag_elem_t))
{
    bool removed;

    if (! *root) {
        removed = false;
    } else if ((*cmp)(elem, (*root)->elem) < 0) {
        if ((removed = avl_remove(&(*root)->left, elem, cmp))) {
            /* Check if the subtree needs rebalancing; update its height. */
            if (HEIGHT((*root)->left) + 1 < HEIGHT((*root)->right))
                avl_rebalance_to_the_left(root);
            else
                avl_update_height(*root);
        }
    } else if ((*cmp)(elem, (*root)->elem) > 0) {
        if ((removed = avl_remove(&(*root)->right, elem, cmp))) {
            /* Check if the subtree needs rebalancing; update its height. */
            if (HEIGHT((*root)->right) + 1 < HEIGHT((*root)->left))
                avl_rebalance_to_the_right(root);
            else
                avl_update_height(*root);
        }
    } else { /* ((*cmp)(elem, (*root)->elem) == 0) */
        if ((*root)->left && (*root)->right) {
            /* Remove from the subtree with larger height. */
            if (HEIGHT((*root)->left) > HEIGHT((*root)->right))
                (*root)->elem = avl_remove_max(&(*root)->left);
            else
                (*root)->elem = avl_remove_min(&(*root)->right);
            /* No rebalancing necessary, but update height. */
            avl_update_height(*root);
        } else {
            /* Remove *root. */
            avl_node_t *old = *root;
            *root = (*root)->left ? (*root)->left : (*root)->right;
            free(old);
        }
        removed = true;
    }

    return removed;
}

bag_elem_t avl_remove_min(avl_node_t **root)
{
    bag_elem_t min;

    if ((*root)->left) {
        /* *root is not the minimum, keep going and rebalance if necessary. */
        min = avl_remove_min(&(*root)->left);
        if (HEIGHT((*root)->left) + 1 < HEIGHT((*root)->right))
            avl_rebalance_to_the_left(root);
        else
            avl_update_height(*root);
    } else {
        /* Remove *root. */
        avl_node_t *old = *root;
        min = (*root)->elem;
        *root = (*root)->right;
        free(old);
    }

    return min;
}

bag_elem_t avl_remove_max(avl_node_t **root)
{
    bag_elem_t max;

    if ((*root)->right) {
        /* *root is not the maximum, keep going and rebalance if necessary. */
        max = avl_remove_max(&(*root)->right);
        if (HEIGHT((*root)->right) + 1 < HEIGHT((*root)->left))
            avl_rebalance_to_the_right(root);
        else
            avl_update_height(*root);
    } else {
        /* Remove *root. */
        avl_node_t *old = *root;
        max = (*root)->elem;
        *root = (*root)->left;
        free(old);
    }

    return max;
}

void avl_rebalance_to_the_left(avl_node_t **root)
{
    if (HEIGHT((*root)->right->left) > HEIGHT((*root)->right->right))
        avl_rotate_to_the_right(&(*root)->right);
    avl_rotate_to_the_left(root);
}

void avl_rebalance_to_the_right(avl_node_t **root)
{
    if (HEIGHT((*root)->left->right) > HEIGHT((*root)->left->left))
        avl_rotate_to_the_left(&(*root)->left);
    avl_rotate_to_the_right(root);
}

void avl_rotate_to_the_left(avl_node_t **parent)
{
    /* Rearrange pointers. */
    avl_node_t *child = (*parent)->right;
    (*parent)->right = child->left;
    child->left = *parent;
    *parent = child;

    /* Update heights. */
    avl_update_height(child->left);
    avl_update_height(child);
}

void avl_rotate_to_the_right(avl_node_t **parent)
{
    /* Rearrange pointers. */
    avl_node_t *child = (*parent)->left;
    (*parent)->left = child->right;
    child->right = *parent;
    *parent = child;

    /* Update heights. */
    avl_update_height(child->right);
    avl_update_height(child);
}

void avl_update_height(avl_node_t *node)
{
    node->height = 1 + ( HEIGHT(node->left) > HEIGHT(node->right) ?
                         HEIGHT(node->left) : HEIGHT(node->right) );
}

avl_node_t *avl_node_create(bag_elem_t elem)
{
    avl_node_t *node = malloc(sizeof(avl_node_t));
    if (node) {
        node->elem = elem;
        node->height = 1;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

/******************************************************************************
 *  Additional "hidden" functions, for debugging purposes.                    *
 ******************************************************************************/

/* FUNCTION avl_print
 *    Print every value in the subtree rooted at root to stdout, in a "sideways
 *    tree" layout with the root at the given depth.  Print each node's element
 *    and height.
 * Parameters and preconditions:
 *    root != NULL: the root of the subtree to print
 *    depth >= 0: the depth at which to print the root's value
 *    indent > 0: number of spaces to print for each level of depth
 *    print != NULL: the function to use to print each node's value
 * Return value:  none
 * Side-effects:
 *    every value in the subtree rooted at root is printed to stdout, using a
 *    "sideways tree" layout (with right subtrees above and left subtrees below,
 *    and indentation to indicate each value's depth in the tree)
 */
static
void avl_print(const avl_node_t *root, int depth, int indent,
               void (*print)(bag_elem_t))
{
    if (root) {
        avl_print(root->right, depth + 1, indent, print);

        /* Print each value followed by its depth, with INDENT spaces of
         * indentation for each level of depth in the tree. */
        printf("%*s", depth * indent, "");
        (*print)(root->elem);
        printf(" [%lu]\n", root->height);

        avl_print(root->left, depth + 1, indent, print);
    }
}

/* FUNCTION bag_print
 *    Print every value in a bag to stdout, in a "sideways tree" layout.
 * Parameters and preconditions:
 *    bag != NULL: the bag
 *    print != NULL: the function to use to print each value in the bag
 * Return value:  none
 * Side-effects:
 *    every value in the bag is printed to stdout, using a "sideways tree"
 *    layout (with right subtrees above and left subtrees below, and indentation
 *    to indicate each value's depth in the tree)
 */
void bag_print(const bag_t *bag, int indent, void (*print)(bag_elem_t))
{
    avl_print(bag->root, 1, indent, print);
}

//problem 1
bool is_avl(avl_node_t *node){
    if(node->left == NULL && node->right == NULL){
        return true;
    }
    // check if root is null as first if. Define heights as variables hleft, hright. check abs of the difference between heights.
    // can have return true at the end after all cases
    else if(node->left == NULL){
        if (node->right->height > 1){
            return false;
        }
        else{
            return true;
        }
    }
    else if(node->right == NULL){
        if (node->left->height > 1){
            return false;
        }
        else{
            return true;
        }
    }
   
    else{
        if(node->left->height > node->right->height + 1 || node->right->height > node->left->height + 1){
            return false;
        }
        else{
            return is_avl(node->left) && is_avl(node->right);
        }
        
    }

}

bool is_avl_tree(bag_t *bag){
    return is_avl(bag->root);
} 



/*bool is_avl_tree(bag_t *bag){
    printf("called\n");

    if(bag->root->left == NULL && bag->root->right == NULL){
      //  printf("in if 1\n");
        return true;
    }
    else if(bag->root->left == NULL ){
        if( bag->root->right->height > 1){
            return false;
        }
        else{
            return true;
        }

       // printf("in if 2\n");
       // return false;
    }
    else if(bag->root->right == NULL ){
        //printf("in if 3\n");
        if (bag->root->left->height > 1){
            return false;
        }
        else{
            return true;
        }
        //return false;
    }


    //if(bag->root->left != NULL && bag->root->right != NULL){
    else{
         //printf("in check \n");
        
        if(bag->root->left->height > bag->root->right->height + 1 || bag->root->right->height > bag->root->left->height + 1){
             //printf("in if 4\n");
            return false;
        }   
        else{
                //printf("in else\n");
                bag -> root = bag->root->left;
                bool x =  is_avl_tree(bag );
                bag -> root = bag->root->right;
                bool y =  is_avl_tree(bag );
                return x && y;

            
            }

    }
  
   


}*/



//Problem 2

bool bag_insert_norot(bag_t *bag, bag_elem_t elem){
    if(bag->root == NULL){
        bag->root = avl_node_create(elem);
        return true;
    }
    //printf("called\n");
    avl_node_t *cur = bag->root;
    avl_node_t *prev = bag->root;
    int dir = 0;
    //printf("done\n");

    while (cur != NULL){
        //printf("while\n");
        if (((bag->cmp)(elem, cur->elem) == 1 || (bag->cmp)(elem, cur->elem) == 0 )){
            cur->height += 1;
            prev = cur;
            cur = cur->right;
            dir = 1;
        }
        else{
            cur->height += 1;
            prev = cur;
            cur = cur->left;
            dir = 0;
        }
    }
    if(dir == 1){
        prev->right = avl_node_create(elem);
    }
    else{
        prev->left = avl_node_create(elem);
    }
    return true;
}
int cmp(bag_elem_t a, bag_elem_t b){
    if(*(int*)a > *(int*)b){
        return 1;
    }
    else if(*(int*)a < *(int*)b){
        return -1;
    }
    else{
        return 0;
    }
}

void print(bag_elem_t elem){
    printf("%d", *(int*)elem);
}
//bool avl_insert(avl_node_t **root, bag_elem_t elem,
  //              int (*cmp)(bag_elem_t, bag_elem_t))
  //avl_node_t *avl_node_create(bag_elem_t elem)



  //bool bag_insert(bag_t *bag, bag_elem_t elem)

//Problem 3

bool avl_remove2(avl_node_t **root, bag_elem_t elem,
                int (*cmp)(bag_elem_t, bag_elem_t))
{
  
    const int  height = HEIGHT(*root);
    avl_node_t** prevs = ( avl_node_t**) malloc(sizeof(avl_node_t*) * height); 
  

    avl_node_t *cur = *root;
  
    bool removed =false;
    int i = 0;
    int dir;
    while(cur != NULL){
      
        if( (*cmp)(elem,cur->elem) == 0){
            if ((cur)->left && (cur)->right) {
                if (HEIGHT((cur)->left) > HEIGHT((cur)->right))
                    (cur)->elem = avl_remove_max(&(cur)->left);
                else
                    (cur)->elem = avl_remove_min(&(cur)->right);
                
                avl_update_height(cur);
                prevs[i] = cur;
               
            } 
            else {
                if(i == 0){
                
                  
                    avl_node_t *old = cur;
                    if(cur->left != NULL){
                        cur = cur->left;
                    }
                    else if (cur->right != NULL){
                        cur = cur->right;
                    }
                    else{
                        cur = NULL;
                    }
   
                    
                    prevs[i] = cur;
                  
                    free(old);
                }
                else{
                    
                    
                    avl_node_t *old = cur;
                    if(cur->left != NULL){
                        if(dir ==1){
                            prevs[i-1] ->right = cur->left;
                            prevs[i] = cur->left;
                          
                        }
                        else{
                            prevs[i-1] ->left = cur->left;
                             prevs[i] = cur->left;
                            
                        }
                        
                    }
                    else if (cur->right != NULL){
                        if(dir ==1){
                            prevs[i-1] ->right = cur->right;
                             prevs[i] = cur->right;
                             
                        }
                        else{
                            prevs[i-1] ->left = cur->right;
                             prevs[i] = cur->right;
                            
                        }
                        
                    }
                    else{
                        if(dir ==1){
                            prevs[i-1] ->right = NULL;
                              prevs[i] = NULL;
                            
                        }
                        else{
                            prevs[i-1] ->left = NULL;
                            prevs[i] = NULL;
                          
                        }
                    }
     
                    free(old);

                }
            }
           
            removed = true;
            break;
        }
        else if((*cmp)(elem,cur->elem) == 1){
          
            prevs[i] = cur;
           
            cur = cur->right;
            dir = 1;
        }
        else{
            
            prevs[i] = cur;
      
            cur  = cur->left;
            dir = 0;
        }
        i++;
    }

   if (removed == true){
        
       if(prevs[i] == NULL){
            
            i--;
        }
        while(i >= 0){
           
            if(prevs[i]->right == NULL && prevs[i]->left == NULL){
               
                i--;
                continue;
            }
            else if(prevs[i]->right != NULL && prevs[i]->left == NULL && HEIGHT(prevs[i]->right) > 1 ){
                if(i==0){
                   avl_rebalance_to_the_left(root); 
                }
                else{
                  avl_rebalance_to_the_left(&prevs[i]);  
                }
                
            }
            else if(prevs[i]->left != NULL && prevs[i]->right == NULL && HEIGHT(prevs[i]->left) > 1 ){
             
                if(i==0){
                   avl_rebalance_to_the_right(root); 
                }
                else{
                  avl_rebalance_to_the_right(&prevs[i]);  
                }
            }

            else if(HEIGHT(prevs[i]->right) + 1 < HEIGHT(prevs[i]->left)){
                if(i==0){
                   avl_rebalance_to_the_right(root); 
                }
                else{
                  avl_rebalance_to_the_right(&prevs[i]);  
                }
            }

            else if(HEIGHT(prevs[i]->left) +1 < HEIGHT(prevs[i]->right)){
                if(i==0){
                   avl_rebalance_to_the_left(root); 
                }
                else{
                  avl_rebalance_to_the_left(&prevs[i]);  
                }
            }
            else{
                avl_update_height(prevs[i]);
            }


            i--;
        }
    }
    free(prevs);
    return removed;

}
    

int main(void){
        
    bag_elem_t a = malloc(sizeof(int));
    *(int*)a = 2;
    bag_t *tree1 = bag_create(&cmp);
    bag_insert(tree1, a);
    bag_elem_t b = malloc(sizeof(int));
    *(int*)b = 3;
    bag_insert(tree1, b);
    bag_elem_t c = malloc(sizeof(int));
    *(int*)c = 8;
    bag_insert(tree1, c);
    bag_elem_t d = malloc(sizeof(int));
    *(int*)d = 1;
    bag_insert(tree1, d);
    bag_elem_t e = malloc(sizeof(int));
    *(int*)e = 9;
    bag_insert(tree1, e);
    //bag_print(tree1, 5, print);

    //printf("%d\n", is_avl_tree(tree1));
    avl_remove2(&(tree1->root),c, cmp );
    bag_print(tree1, 5, print);
    printf("%d\n", is_avl_tree(tree1));

}

