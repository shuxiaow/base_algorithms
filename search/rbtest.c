#include <stdio.h>
#include "rbtree.h"

int main()
{
    rbtree_t* rbt = rbtree_new();
    rbtree_put(rbt, 1, (char*)1);
    rbtree_put(rbt, 2, (char*)2);
    rbtree_put(rbt, 3, (char*)3);
    rbtree_put(rbt, 4, (char*)4);
    rbtree_put(rbt, 5, (char*)5);
    rbtree_put(rbt, 6, (char*)5);
    rbtree_put(rbt, 7, (char*)5);
    rbtree_inorder_traversal(rbt);

    rbtree_del(rbt, 1);
    rbtree_inorder_traversal(rbt);
    rbtree_del(rbt, 2);
    rbtree_inorder_traversal(rbt);
    rbtree_del(rbt, 3);
    rbtree_inorder_traversal(rbt);
    rbtree_del(rbt, 4);
    rbtree_inorder_traversal(rbt);
    rbtree_del(rbt, 5);
    rbtree_inorder_traversal(rbt);
    rbtree_del(rbt, 6);
    rbtree_inorder_traversal(rbt);
    rbtree_del(rbt, 7);
    rbtree_inorder_traversal(rbt);

    return 0;
}
