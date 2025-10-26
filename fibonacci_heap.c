#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

typedef struct Node {
    int key;
    int degree;
    struct Node *parent;
    struct Node *child;
    struct Node *left;
    struct Node *right;
    int mark;
} Node;

typedef struct FibHeap {
    Node *min;
    int n;
} FibHeap;

Node* newNode(int key) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->key = key;
    n->degree = 0;
    n->parent = n->child = NULL;
    n->left = n->right = n;
    n->mark = 0;
    return n;
}

FibHeap* createHeap() {
    FibHeap* h = (FibHeap*)malloc(sizeof(FibHeap));
    h->min = NULL;
    h->n = 0;
    return h;
}

void insert(FibHeap* h, int key) {
    Node* n = newNode(key);
    if (h->min == NULL)
        h->min = n;
    else {
        n->left = h->min;
        n->right = h->min->right;
        h->min->right->left = n;
        h->min->right = n;
        if (n->key < h->min->key)
            h->min = n;
    }
    h->n++;
}

Node* findMin(FibHeap* h) {
    return h->min;
}

void link(Node* y, Node* x) {
    y->left->right = y->right;
    y->right->left = y->left;
    y->parent = x;
    if (x->child == NULL) {
        x->child = y;
        y->left = y->right = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = 0;
}

void consolidate(FibHeap* h) {
    int D = (int)(log2(h->n)) + 1;
    Node* A[D];
    for (int i = 0; i < D; i++) A[i] = NULL;

    Node* start = h->min;
    Node* w = start;
    do {
        Node* x = w;
        int d = x->degree;
        w = w->right;
        while (A[d] != NULL) {
            Node* y = A[d];
            if (x->key > y->key) {
                Node* temp = x;
                x = y;
                y = temp;
            }
            link(y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
    } while (w != start);

    h->min = NULL;
    for (int i = 0; i < D; i++) {
        if (A[i] != NULL) {
            if (h->min == NULL)
                h->min = A[i]->left = A[i]->right = A[i];
            else {
                A[i]->left = h->min;
                A[i]->right = h->min->right;
                h->min->right->left = A[i];
                h->min->right = A[i];
                if (A[i]->key < h->min->key)
                    h->min = A[i];
            }
        }
    }
}

Node* extractMin(FibHeap* h) {
    Node* z = h->min;
    if (z != NULL) {
        if (z->child != NULL) {
            Node* c = z->child;
            do {
                Node* next = c->right;
                c->parent = NULL;
                c->left = h->min;
                c->right = h->min->right;
                h->min->right->left = c;
                h->min->right = c;
                c = next;
            } while (c != z->child);
        }

        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right)
            h->min = NULL;
        else {
            h->min = z->right;
            consolidate(h);
        }
        h->n--;
    }
    return z;
}

FibHeap* unite(FibHeap* h1, FibHeap* h2) {
    if (h1->min == NULL) return h2;
    if (h2->min == NULL) return h1;

    Node* h1Right = h1->min->right;
    Node* h2Left = h2->min->left;

    h1->min->right = h2->min;
    h2->min->left = h1->min;
    h1Right->left = h2Left;
    h2Left->right = h1Right;

    if (h2->min->key < h1->min->key)
        h1->min = h2->min;
    h1->n += h2->n;
    return h1;
}

void cut(FibHeap* h, Node* x, Node* y) {
    if (x->right == x)
        y->child = NULL;
    else {
        x->left->right = x->right;
        x->right->left = x->left;
        if (y->child == x)
            y->child = x->right;
    }
    y->degree--;

    x->left = h->min;
    x->right = h->min->right;
    h->min->right->left = x;
    h->min->right = x;
    x->parent = NULL;
    x->mark = 0;
}

void cascadingCut(FibHeap* h, Node* y) {
    Node* z = y->parent;
    if (z != NULL) {
        if (y->mark == 0)
            y->mark = 1;
        else {
            cut(h, y, z);
            cascadingCut(h, z);
        }
    }
}

void decreaseKey(FibHeap* h, Node* x, int newKey) {
    if (newKey > x->key) {
        printf("New key is greater than current key!\n");
        return;
    }
    x->key = newKey;
    Node* y = x->parent;
    if (y != NULL && x->key < y->key) {
        cut(h, x, y);
        cascadingCut(h, y);
    }
    if (x->key < h->min->key)
        h->min = x;
}

void display(FibHeap* h) {
    if (h->min == NULL) {
        printf("Heap is empty\n");
        return;
    }
    printf("Root list: ");
    Node* p = h->min;
    do {
        printf("%d ", p->key);
        p = p->right;
    } while (p != h->min);
    printf("\n");
}

int main() {
    FibHeap* h1 = createHeap();
    FibHeap* h2 = createHeap();

    insert(h1, 10);
    insert(h1, 3);
    insert(h1, 15);
    insert(h1, 6);

    printf("Heap 1 after inserts:\n");
    display(h1);
    printf("Min key: %d\n", findMin(h1)->key);

    insert(h2, 5);
    insert(h2, 8);

    printf("\nHeap 2:\n");
    display(h2);

    h1 = unite(h1, h2);
    printf("\nAfter uniting Heap 1 and Heap 2:\n");
    display(h1);

    Node* minNode = extractMin(h1);
    printf("\nExtracted min: %d\n", minNode->key);
    display(h1);

    Node* node = h1->min->right; 
    printf("\nDecreasing key %d to 1...\n", node->key);
    decreaseKey(h1, node, 1);
    display(h1);
    printf("Min key now: %d\n", findMin(h1)->key);

    return 0;
}
