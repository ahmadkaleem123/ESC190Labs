Lab 8
=====

In this lab, you will be working with code from avl_bag.c, to understand it more deeply.

1. In avl_bag.c, implement the function

   bool is_avl_tree(bag_t *bag)

   The function takes in a pointer to a BST, and returns True if the BST is an AVL tree.

2. To test your function from (1), you need to create AVL trees (easy) as well as BSTs
   that are not AVL trees. Make a function 

   bool bag_insert_norot(bag_t *bag, bag_elem_t elem)
 
   which inserts an element into a BST without performing any rotations. Using this function,
   create BSTs that are not AVL trees.

   Show your TA that your is_avl_tree function returns true for AVL trees and returns false
   for non-AVL trees.

3. Rewrite avl_remove from scratch.
