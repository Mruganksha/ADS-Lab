#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char mpnth[20];
    struct Node* left;
    struct Node* right;
    int height;
};

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(struct Node* n) {
    return (n == NULL) ? 0 : n->height;
}

struct Node* newNode(const char* month) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    strcpy(n->mpnth, month);
    n->left = n->right = NULL;
    n->height = 1;
    return n;
}

struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(struct Node* n) {
    return (n == NULL) ? 0 : height(n->left) - height(n->right);
}

int compareMonths(const char* m1, const char* m2) {
    return strcmp(m1, m2);
}

struct Node* insert(struct Node* node, const char* month) {
    if (node == NULL)
        return newNode(month);

    if (compareMonths(month, node->mpnth) < 0)
        node->left = insert(node->left, month);
    else if (compareMonths(month, node->mpnth) > 0)
        node->right = insert(node->right, month);
    else 
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && compareMonths(month, node->left->mpnth) < 0)
        return rightRotate(node);

    if (balance < -1 && compareMonths(month, node->right->mpnth) > 0)
        return leftRotate(node);

    if (balance > 1 && compareMonths(month, node->left->mpnth) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && compareMonths(month, node->right->mpnth) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void inorder(struct Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%s ", root->mpnth);
        inorder(root->right);
    }
}

int main() {
    struct Node* root = NULL;

    char* months[] = {"december", "jan", "april", "march", "july", "aug", "october", "feb", "november", "may", "june"};
    int n = sizeof(months) / sizeof(months[0]);

    for (int i = 0; i < n; i++) {
        root = insert(root, months[i]);
    }

    printf("Inorder traversal of AVL tree:\n");
    inorder(root);
    printf("\n");

    return 0;
}
