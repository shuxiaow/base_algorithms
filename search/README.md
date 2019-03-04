# 基础搜索

## 1 二叉搜索树

基础API：

```cpp
bool get(Key key, Value& val); // 查找key对应的value
bool put(Key key, Value val); // 插入或更新key对应的value
bool remove(Key key); // 删除指定key
```

### 查找

二叉搜索树本身是维护了有序性：一个节点的值总是大于左子树所有节点的值，小于右子树所有节点的值。

此外，二叉搜索树本身包含了递归的思想：任何一棵树，都是由 根节点 + 左子树 + 右子树 构成。

```cpp
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
```

### 删除

二叉树的删除操作：

- 如果删除的节点是叶子节点，直接删除即可；
- 如果删除的节点只有一个子树（左或者右），直接将子树提上来即可；
- 如果删除的节点有两个子树，那么将右子树的最小节点提上来，然后将其从右子树删除即可。

注：将一个节点A提到节点B的位置，只需要将A的value赋值到B即可，然后将节点A删除即可。如果是要将节点B的父节点指向节点A来替换的话，那必须修改节点B的父节点的信息，比较麻烦。

如何删除一个树最小的节点：

```cpp
// 删除node为根节点的树中最小的节点，并返回新的根节点
Node* deleteMin(Node* node)
{
    if(!node)
        return NULL;

    if(node->left) {
        node->left = deleteMin(node->left);
    } else {
        Node* right = node->right;
        delete node;
        node = node->right;
    }
    return node;
}
```
