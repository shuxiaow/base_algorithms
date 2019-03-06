#include <stdio.h>
#include "redBlackTree.h"

int main()
{
    RedBlackTree<int,int> rbtree;

    rbtree.put(1, 1);
    rbtree.put(2, 2);
    rbtree.put(3, 3);
    rbtree.put(4, 4);
    rbtree.put(5, 5);
    rbtree.put(6, 6);
    rbtree.put(7, 7);
    rbtree.put(8, 8);

    printf("min:%d\n", rbtree.min());
    rbtree.deleteMin();
    printf("min:%d\n", rbtree.min());
    rbtree.deleteMin();
    printf("min:%d\n", rbtree.min());
    rbtree.deleteMin();
    printf("min:%d\n", rbtree.min());
    rbtree.deleteMin();
    printf("min:%d\n", rbtree.min());

    return 0;
}
