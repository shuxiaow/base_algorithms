#pragma once

/*
 * This RedBlackTree is equivalent to 2-3 tree
 */

template <class Key, class Value>
class RedBlackTree
{
public:
    RedBlackTree(): m_root(nullptr) {}
    ~RedBlackTree(){}

    bool get(Key key, Value& val)
    {
        Node* node = get(m_root, key);
        if(node) {
            val = node->val;
            return true;
        }
        return false;
    }

    void put(Key key, Value val)
    {
        m_root = put(m_root, key, val);
        m_root->color = Node::BLACK;
    }

    Value min()
    {
        return min(m_root)->val;
    }

    void deleteMin()
    {
        if(!m_root) return ;

        if(!isRed(m_root->left) && !isRed(m_root->right))
            m_root->color = Node::RED;

        m_root = deleteMin(m_root);
        if(m_root)
            m_root->color = Node::BLACK;
    }

    unsigned int size()
    {
        return size(m_root);
    }

private:
    struct Node {
        enum Color {
            RED = 0,
            BLACK
        };
        Key key;
        Value val;
        unsigned int size;
        Color color;
        Node* left;
        Node* right;

        Node(Key key_, Value val_): key(key_), val(val_), size(1), 
            color(RED), left(nullptr), right(nullptr) 
        {}
    };

    Node* min(Node* node)
    {
        if(node->left)
            return min(node->left);
        return node;
    }

    // 将红色的右链接旋转成左链接
    Node* rotateLeft(Node* node)
    {
        if(node == nullptr || node->right == nullptr) 
            return node;

        Node* xright = node->right;
        xright->color = node->color;
        node->color = Node::RED;
        node->right = xright->left;
        xright->left = node;
        xright->size = node->size;
        node->size = 1 + size(node->left) + size(node->right);

        return xright;
    }

    Node* rotateRight(Node* node)
    {
        if(node == nullptr || node->left == nullptr)
            return node;
        Node* xleft = node->left;
        xleft->color = node->color;
        node->color = Node::RED;
        node->left = xleft->right;
        xleft->right = node;
        xleft->size = node->size;
        node->size = 1 + size(node->left) + size(node->right);

        return xleft;
    }

    Node* flipColor(Node* node)
    {
        if(node == nullptr || node->left == nullptr || node->right == nullptr) 
            return node;

        node->left->color = Node::BLACK;
        node->right->color = Node::BLACK;
        node->color = Node::RED;
        return node;
    }

    bool isRed(Node* node)
    {
        if(!node) return false;

        return node->color == Node::RED;
    }

    Node* get(Node* root, Key key)
    {
        if(!root) return nullptr;

        Node* node = nullptr;
        if(root->key > key)
            node = get(root->left, key);
        else if(root->key < key)
            node = get(root->right, key);
        else
            node = root;
        return node;
    }

    Node* put(Node* root, Key key, Value val)
    {
        if(root == nullptr)
            return new Node(key, val);

        if(root->key < key) {
            root->right = put(root->right, key, val);
        } else if(root->key > key) {
            root->left = put(root->left, key, val);
        } else {
            root->val = val;
        }

        if(isRed(root->right) && !isRed(root->left))
            root = rotateLeft(root);
        if(isRed(root->left) && isRed(root->left->left))
            root = rotateRight(root);
        if(isRed(root->left) && isRed(root->right))
            flipColor(root);

        root->size = 1 + size(root->left) + size(root->right);
        return root;
    }

    //  2-3-4(from Top to Bottom)
    //  自顶向下：在递归调用之前执行的代码
    //  自底向上：在递归调用之后执行的代码
    Node* put2(Node* node, Key key, Value val)
    {
        if(node == nullptr)
            return new Node(key, val);

        // 将 4- 节点转换成3个 2- 节点
        if(isRed(node->left) && isRed(node->right)) {
            flipColor(node);
        }

        if(node->key < key)
            node->right = put2(node->right, key, val);
        else if(node->key > key)
            node->left = put2(node->left, key, val);
        else
            node->val = val;

        if(!isRed(node->left) && isRed(node->right))
            node = rotateLeft(node);
        if(isRed(node->left) && isRed(node->left->left))
            node = rotateRight(node);

        node->size = 1 + size(node->left) + size(node->right);
        return node;
    }

    // node左子节点是一个2-节点
    Node* moveRedLeft(Node* node)
    {
        if(!node) return nullptr;
        node->color = Node::BLACK;
        if(node->left) node->left->color = Node::RED;
        if(node->right) node->right->color = Node::RED;

        if(isRed(node->right->left)) {
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
        }
        return node;
    }

    Node* deleteMin(Node* node)
    {
        if(!node || !node->left)
            return nullptr;
        if(!isRed(node->left) && !isRed(node->left->left))
            node = moveRedLeft(node);
        node->left = deleteMin(node->left);
        
        if(isRed(node->right))
            node = rotateLeft(node);
        if(isRed(node->right) && !isRed(node->left))
            node = rotateLeft(node);
        if(isRed(node->left) && isRed(node->left->left))
            node = rotateRight(node);
        if(isRed(node->left) && isRed(node->right))
            flipColor(node);

        node->size = 1 + size(node->left) + size(node->right);
        return node;
    }

    unsigned int size(Node* root)
    {
        if(!root)
            return 0;
        return root->size;
    }

    Node* m_root;
};
