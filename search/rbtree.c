#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// internal functions
rbt_node* __rbtree_put(rbt_node* root, int key, char* value);
rbt_node* __rbtree_del(rbt_node* root, int key);
rbt_node* __rbtree_new_node(int key, char* value);
int __rbtree_is_red(rbt_node* node);
int __rbtree_size(rbt_node* node);
void __rbtree_flip_colors(rbt_node* node);
rbt_node* __rbtree_rotate_left(rbt_node* node);
rbt_node* __rbtree_rotate_right(rbt_node* node);
rbt_node* __rbtree_put(rbt_node* node, int key, char* value);
rbt_node* __rbtree_min(rbt_node* root);
rbt_node* __rbtree_del_min(rbt_node* root);
int __rbtree_inorder_traversal(rbt_node* node, char* buf, int size);

rbtree_t* rbtree_new()
{
    rbtree_t* rbt = (rbtree_t*)malloc(sizeof(rbtree_t));
    rbt->root = NULL;
    return rbt;
}

// return value of key, return NULL if not found
char* rbtree_get(rbtree_t* rbt, int key)
{
    char* val = NULL;
    rbt_node* node = rbt->root;
    while(node) {
        if(node->key < key) {
            node = node->right;
        } else if(node->key > key) {
            node = node->left;
        } else {
            val = node->value;
            break;
        }
    }
    return val;
}

// put key:value into rbtree, return 0 on success, -1 on failure
int rbtree_put(rbtree_t* rbt, int key, char* value)
{
    rbt->root = __rbtree_put(rbt->root, key, value);
    rbt->root->color = RBT_BLACK;
    return 0;
}

// remove key:value from rbtree, return 0 on success
int rbtree_del(rbtree_t* rbt, int key)
{
    if(rbt == NULL)
        return -1;
    rbt->root = __rbtree_del(rbt->root, key);
    if(rbt->root && rbt->root->color == RBT_DOUBLE_BLACK) {
        if(rbt->root->removed) {
            free(rbt->root);
            rbt->root = NULL;
        } else {
            rbt->root->color = RBT_BLACK;
            if(rbt->root->left) {
                if(rbt->root->left->removed)
                    rbt->root->left = NULL;
                else 
                    rbt->root->left->color = RBT_RED;
            }
            if(rbt->root->right) {
                if(rbt->root->right->removed)
                    rbt->root->right = NULL;
                else 
                    rbt->root->right->color = RBT_RED;
            }
        }
    }

    return 0;
}

// return size of rbtree
int rbtree_size(rbtree_t* rbt)
{
    if(rbt == NULL)
        return 0;
    return __rbtree_size(rbt->root);
}

void rbtree_inorder_traversal(rbtree_t* rbt)
{
    char buf[1024];
    int ret = __rbtree_inorder_traversal(rbt->root, buf, 1024);
    if(ret > 0) {
        buf[ret] = '\0';
        printf("%s\n", buf);
    }
}

// implements of internal functions
//
//
//
//
rbt_node* __rbtree_new_node(int key, char* value)
{
    rbt_node* node = (rbt_node*)malloc(sizeof(rbt_node));
    node->parent = NULL;
    node->color = RBT_RED;
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->size = 1;
    node->removed = 0;
    return node;
}

int __rbtree_is_red(rbt_node* node)
{
    if(node && node->color == RBT_RED)
        return 1;
    return 0;
}

int __rbtree_size(rbt_node* node)
{
    return node == NULL ? 0 : node->size;
}

// before: node is black, and node's children are all red
// after: node is red, and node's children are all black
void __rbtree_flip_colors(rbt_node* node)
{
    node->color = RBT_RED;
    node->left->color = RBT_BLACK;
    node->right->color = RBT_BLACK;
}

// node and node->right
rbt_node* __rbtree_rotate_left(rbt_node* node)
{
    rbt_node* right = node->right;
    node->right = right->left;
    right->left = node;
    right->color = node->color;
    right->parent = node->parent;
    right->size = node->size;
    node->color = RBT_RED;
    node->parent = right;
    node->size = __rbtree_size(node->left) + __rbtree_size(node->right) + 1;
    return right;
}

rbt_node* __rbtree_rotate_right(rbt_node* node)
{
    rbt_node* left = node->left;
    node->left = left->right;
    left->right = node;
    left->color = node->color;
    left->parent = node->parent;
    left->size = node->size;
    node->color = left->color;
    node->parent = left;
    node->size = __rbtree_size(node->left) + __rbtree_size(node->right) + 1;
    return left;
}

rbt_node* __rbtree_put(rbt_node* node, int key, char* value)
{
    if(node == NULL) {
        return __rbtree_new_node(key, value);
    }

    if(node->key == key) {
        node->value = value;
        return node;
    }

    if(node->key > key) {
        node->left = __rbtree_put(node->left, key, value);
        if(node->left->parent == NULL)
            node->left->parent = node;
    } else {
        node->right = __rbtree_put(node->right, key, value);
        if(node->right->parent == NULL)
            node->right->parent = node;
    }

    if(__rbtree_is_red(node->left) && __rbtree_is_red(node->left->right)) {
        if(__rbtree_is_red(node->right)) {
            __rbtree_flip_colors(node);
        } else {
            node->left = __rbtree_rotate_left(node->left);
        }
    }

    if(__rbtree_is_red(node->left) && __rbtree_is_red(node->left->left)) {
        if(__rbtree_is_red(node->right)) {
            __rbtree_flip_colors(node);
        } else {
            node = __rbtree_rotate_right(node);
        }
    }
    if(__rbtree_is_red(node->right) && __rbtree_is_red(node->right->left)) {
        if(__rbtree_is_red(node->left)) {
            __rbtree_flip_colors(node);
        } else {
            node->right = __rbtree_rotate_right(node->right);
        }
    }
    if(__rbtree_is_red(node->right) && __rbtree_is_red(node->right->right)) {
        if(__rbtree_is_red(node->left)) {
            __rbtree_flip_colors(node);
        } else {
            node = __rbtree_rotate_left(node);
        }
    }

    node->size = __rbtree_size(node->left) + __rbtree_size(node->right) + 1;

    return node;
}

rbt_node* __rbtree_min(rbt_node* root)
{
    rbt_node* min = root;
    while(min) {
        if(min->left == NULL)
            break;
        min = min->left;
    }

    return min;
}

// 自底向上
rbt_node* __rbtree_del(rbt_node* node, int key)
{
    if(node == NULL)
        return NULL;

    if(node->key < key) {
        node->right = __rbtree_del(node->right, key);
    } else if(node->key > key) {
        node->left = __rbtree_del(node->left, key);
    } else {
        if(node->right != NULL) {
            rbt_node* succNode = __rbtree_min(node->right); // successor node in inorder traversal
            // copy successor's key&value to this node
            node->key = succNode->key;
            node->value = succNode->value;
            node->right = __rbtree_del(node->right, succNode->key);
        } else { // node 是叶子节点或只有一个左二子
            if(__rbtree_is_red(node)) { // 如果是红色，那就肯定是叶子节点
                free(node);
                return NULL;
            } else { // 如果是黑色，那可能是叶子节点，或者有一个红色左儿子
                if(__rbtree_is_red(node->left)) {
                    node->key = node->left->key;
                    node->value = node->left->value;
                    free(node->left);
                    node->left = NULL;
                    return node;
                } else {
                    // 节点是2-节点，标记下，然后交给父节点处理
                    // 这里标记的是待删除的叶子节点，后面需要执行删除操作
                    node->color = RBT_DOUBLE_BLACK;
                    node->removed = 1;
                    return node;
                }
            }
        }
    }

    // 处理 double black节点
    if(node->left && node->left->color == RBT_DOUBLE_BLACK) {
        if(!__rbtree_is_red(node->right)) {
            if(__rbtree_is_red(node->right->left) && !__rbtree_is_red(node->right->right)) {
                node->right = __rbtree_rotate_right(node->right);
            }
            if(__rbtree_is_red(node->right->right)) { // noed->right是3-节点或4-节点
                if(node->left->removed) {
                    free(node->left);
                    node->left = NULL;
                } else {
                    node->left->color = RBT_BLACK;
                }
                node = __rbtree_rotate_left(node);
            } else { // node->right是2-节点
                node->left->color = RBT_BLACK;
                node->right->color = RBT_RED;
                if(__rbtree_is_red(node)) {
                    node->color = RBT_BLACK;
                    if(node->left->removed) {
                        free(node->left);
                        node->left = NULL;
                    }
                } else {
                    node->color = RBT_DOUBLE_BLACK;
                }
            }
        } else { // node->right 是红色
            node = __rbtree_rotate_left(node);
            node->left->color = RBT_BLACK;
            node->left->left->color = RBT_BLACK;
            node->left->right->color = RBT_RED;
            if(node->left->left->removed) {
                free(node->left->left);
                node->left->left = NULL;
                node->left->size--;
            }
        }
    } else if(node->right && node->right->color == RBT_DOUBLE_BLACK) {
        if(!__rbtree_is_red(node->left)) {
            if(__rbtree_is_red(node->left->right) && !__rbtree_is_red(node->left->left)) {
                node->left = __rbtree_rotate_left(node->left);
            }
            if(__rbtree_is_red(node->left->left)) { // noed->left是3-节点或4-节点
                if(node->right->removed) {
                    free(node->right);
                    node->right = NULL;
                } else {
                    node->right->color = RBT_BLACK;
                }
                node = __rbtree_rotate_right(node);
            } else { // node->left是2-节点
                node->right->color = RBT_BLACK;
                node->left->color = RBT_RED;
                if(__rbtree_is_red(node)) {
                    node->color = RBT_BLACK;
                    if(node->right->removed) {
                        free(node->right);
                        node->right = NULL;
                    }
                } else {
                    node->color = RBT_DOUBLE_BLACK;
                }
            }
        } else { // node->left 是红色
            node = __rbtree_rotate_right(node);
            node->right->color = RBT_BLACK;
            node->right->right->color = RBT_BLACK;
            node->right->left->color = RBT_RED;
            if(node->right->right->removed) {
                free(node->right->right);
                node->right->right = NULL;
                node->right->size--;
            }
        }
    }
    node->size = __rbtree_size(node->left) + __rbtree_size(node->right);

    return node;
}

int __rbtree_inorder_traversal(rbt_node* node, char* buf, int size)
{
    if(node == NULL)
        return 0;
    int total = 0;
    int ret = 0;
    if(node->left) {
        ret = __rbtree_inorder_traversal(node->left, buf, size);
        if(ret < 0)
            return ret;
        buf += ret;
        size -= ret;
        total += ret;
    }
    ret = snprintf(buf, size, "%d ", node->key);
    if(ret < 0)
        return ret;
    buf += ret;
    size -= ret;
    total += ret;
    if(node->right) {
        ret = __rbtree_inorder_traversal(node->right, buf, size);
        if(ret < 0)
            return ret;
        buf += ret;
        size -= ret;
        total += ret;
    }

    return total;
}
