#ifndef __RBTREE_IMPL_H__
#define __RBTREE_IMPL_H__

typedef enum rbt_color {
    RBT_RED = 0,
    RBT_BLACK = 1,
    RBT_DOUBLE_BLACK // temp state
} rbt_color;

typedef struct rbt_node {
    rbt_color color;
    struct rbt_node* left;
    struct rbt_node* right;
    int key;
    char* value;
    int size;
    int removed;
} rbt_node;
    
typedef struct rbtree_t {
    rbt_node* root;
} rbtree_t;

rbtree_t* rbtree_new();
void rbtree_destroy(rbtree_t* rbt);

// return value of key, return NULL if not found
char* rbtree_get(rbtree_t* rbt, int key);
// put key:value into rbtree, return 0 on success, -1 on failure
int rbtree_put(rbtree_t* rbt, int key, char* value);
// remove key:value from rbtree, return 0 on success
int rbtree_del(rbtree_t* rbt, int key);
// return size of rbtree
int rbtree_size(rbtree_t* rbt);
void rbtree_inorder_traversal(rbtree_t* rbt);

#endif
