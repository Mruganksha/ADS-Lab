#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } nodeColor;

typedef struct Node {
    int data;
    nodeColor color;
    struct Node *left, *right, *parent;
} Node;

Node* NIL;  
Node* root; 

void initialize() {
    NIL = (Node*)malloc(sizeof(Node));
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NULL;
    root = NIL;
}

Node* createNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->color = RED;
    node->left = node->right = node->parent = NIL;
    return node;
}

void leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NIL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rightRotate(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != NIL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NIL)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void fixInsert(Node* k) {
    Node* u;

    while (k->parent->color == RED) {
        if (k->parent == k->parent->parent->left) {
            u = k->parent->parent->right; 
            if (u->color == RED) {
                k->parent->color = BLACK;
                u->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(k->parent->parent);
            }
        } else {
            u = k->parent->parent->left; 
            if (u->color == RED) {
                k->parent->color = BLACK;
                u->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(k);
                }
                
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(k->parent->parent);
            }
        }
        if (k == root)
            break;
    }
    root->color = BLACK;
}

void insert(int data) {
    Node* node = createNode(data);
    Node* y = NIL;
    Node* x = root;

    while (x != NIL) {
        y = x;
        if (node->data < x->data)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;
    if (y == NIL)
        root = node;
    else if (node->data < y->data)
        y->left = node;
    else
        y->right = node;

    fixInsert(node);
}

Node* search(Node* node, int key) {
    if (node == NIL || key == node->data)
        return node;

    if (key < node->data)
        return search(node->left, key);

    return search(node->right, key);
}

void inorder(Node* node) {
    if (node != NIL) {
        inorder(node->left);
        printf("%d ", node->data);
        inorder(node->right);
    }
}

int main() {
    initialize();

    int values[] = {20, 15, 25, 10, 5, 1, 30, 22, 27};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++) {
        insert(values[i]);
    }

    printf("Inorder traversal: ");
    inorder(root);
    printf("\n");

    int val = 22;
    Node* found = search(root, val);
    if (found != NIL)
        printf("Found %d in the tree.\n", val);
    else
        printf("%d not found in the tree.\n", val);

    return 0;
}
