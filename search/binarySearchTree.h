#pragma once

#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <vector>

template <class Key, class Value>
class BinarySearchTree
{
public:
    BinarySearchTree() : m_root(NULL) {}
    ~BinarySearchTree() {
        clear();
    }

    bool get(Key key, Value& val);
    bool put(Key key, Value val);
    bool remove(Key key);

    size_t size();
    void clear()
    {
        clear(m_root);
    }
    
    void inOrderTraversal(std::vector<Key>& keyvec)
    {
        inOrderTraversal(m_root, keyvec);
    }
    void preOrderTraversal(std::vector<Key>& keyvec)
    {
        preOrderTraversal(m_root, keyvec);
    }
    void postOrderTraversal(std::vector<Key>& keyvec)
    {
        postOrderTraversal(m_root, keyvec);
    }

private:
    struct Node {
        Key key;
        Value value;
        size_t size;
        Node* left;
        Node* right;

        Node() : left(NULL), right(NULL), size(1) {}

        Node(Key key_, Value val_) : key(key_), value(val_), left(NULL), right(NULL), size(1) {}
    };

    size_t size(Node* x)
    {
        return x == NULL ? 0 : x->size;
    }

    Node* get(Node* node, Key key)
    {
        if(node == NULL)
            return NULL;
        if(node->key == key)
            return node;
        if(node->key < key)
            return get(node->right, key);
        else
            return get(node->left, key);
    }

    Node* put(Node* node, Key key, Value val)
    {
        if(node == NULL)
            return new Node(key, val);
        if(node->key < key) {
            node->right = put(node->right, key, val);
        } else if(node->key > key) {
            node->left = put(node->left, key, val);
        } else {
            node->value = val;
        }
        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

    void clear(Node* node)
    {
        if(node == NULL)
            return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
    
    Node* min(Node* node)
    {
        if(node->left)
            return min(node->left);
        return node;
    }

    Node* removeMin(Node* node)
    {
        if(!node)
            return NULL;
        if(node->left) {
             node->left = removeMin(node->left);
             node->size = size(node->left) + size(node->right) + 1;
        } else {
            Node* right = node->right;
            delete node;
            node = right;
        }
        return node;
    }

    Node* remove(Node* node, Key key)
    {
        if(node == NULL)
            return NULL;

        if(node->key == key) {
            if(node->left == NULL) {
                Node* right = node->right;
                delete node;
                return right;
            }
            if(node->right == NULL) {
                Node* left = node->left;
                delete node;
                return left;
            }
                
            Node* minNode = min(node->right);
            node->value = minNode->value;
            node->right = removeMin(node->right);
        } else if(node->key < key) {
            node->right = remove(node->right, key);
        } else {
            node->left = remove(node->left, key);
        }
        node->size = size(node->left) + size(node->right) + 1;
        return node;
    }

    void preOrderTraversal(Node* node, std::vector<Key>& keyvec)
    {
        if(node == NULL)
            return;
        keyvec.push_back(node->key);
        preOrderTraversal(node->left, keyvec);
        preOrderTraversal(node->right, keyvec);
    }
    void inOrderTraversal(Node* node, std::vector<Key>& keyvec)
    {
        if(node == NULL)
            return;
        inOrderTraversal(node->left, keyvec);
        keyvec.push_back(node->key);
        inOrderTraversal(node->right, keyvec);
    }
    void postOrderTraversal(Node* node, std::vector<Key>& keyvec)
    {
        if(node == NULL)
            return;
        postOrderTraversal(node->left, keyvec);
        postOrderTraversal(node->right, keyvec);
        keyvec.push_back(node->key);
    }

    Node* m_root;
};

template <class Key, class Value>
bool BinarySearchTree<Key,Value>::get(Key key, Value& val)
{
    Node* pNode = get(m_root, key);
    if(pNode) {
        val = pNode->value;
        return true;
    }
    return false;
}

template <class Key, class Value>
bool BinarySearchTree<Key,Value>::put(Key key, Value val)
{
    m_root = put(m_root, key, val);
    return true;
}

template <class Key, class Value>
size_t BinarySearchTree<Key,Value>::size()
{
    return m_root->size;
}

template <class Key, class Value>
bool BinarySearchTree<Key,Value>::remove(Key key)
{
    m_root = remove(m_root, key);
    return true;
}
