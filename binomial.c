#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Node {
    int key;
    int degree;
    struct Node* parent;
    struct Node* child;
    struct Node* sibling;
} Node;

typedef struct Heap {
    Node* head;
} Heap;

Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->degree = 0;
    newNode->parent = NULL;
    newNode->child = NULL;
    newNode->sibling = NULL;
    return newNode;
}

void linkTrees(Node* child, Node* parent) {
    child->parent = parent;
    child->sibling = parent->child;
    parent->child = child;
    parent->degree++;
}

Node* mergeRootLists(Node* first, Node* second) {
    if (!first) return second;
    if (!second) return first;

    Node* mergedHead = NULL;
    Node* mergedTail = NULL;

    if (first->degree <= second->degree) {
        mergedHead = first;
        first = first->sibling;
    } else {
        mergedHead = second;
        second = second->sibling;
    }
    mergedTail = mergedHead;

    while (first && second) {
        if (first->degree <= second->degree) {
            mergedTail->sibling = first;
            first = first->sibling;
        } else {
            mergedTail->sibling = second;
            second = second->sibling;
        }
        mergedTail = mergedTail->sibling;
    }

    if (first) mergedTail->sibling = first;
    else mergedTail->sibling = second;

    return mergedHead;
}

Heap* unionHeaps(Heap* heap1, Heap* heap2) {
    Heap* newHeap = (Heap*)malloc(sizeof(Heap));
    newHeap->head = mergeRootLists(heap1->head, heap2->head);

    if (!newHeap->head)
        return newHeap;

    Node* prev = NULL;
    Node* current = newHeap->head;
    Node* next = current->sibling;

    while (next) {
        if ((current->degree != next->degree) || 
            (next->sibling && next->sibling->degree == current->degree)) {
            prev = current;
            current = next;
        } else {
            if (current->key <= next->key) {
                current->sibling = next->sibling;
                linkTrees(next, current);
            } else {
                if (prev == NULL) {
                    newHeap->head = next;
                } else {
                    prev->sibling = next;
                }
                linkTrees(current, next);
                current = next;
            }
        }
        next = current->sibling;
    }
    return newHeap;
}

Heap* insertKey(Heap* heap, int key) {
    Heap* tempHeap = (Heap*)malloc(sizeof(Heap));
    tempHeap->head = createNode(key);
    Heap* newHeap = unionHeaps(heap, tempHeap);
    free(tempHeap);
    return newHeap;
}

Node* findMinNode(Heap* heap) {
    if (!heap->head) return NULL;

    Node* minNode = heap->head;
    Node* current = heap->head;
    int minKey = current->key;

    while (current) {
        if (current->key < minKey) {
            minKey = current->key;
            minNode = current;
        }
        current = current->sibling;
    }
    return minNode;
}

Node* reverseList(Node* node) {
    Node* prev = NULL;
    Node* next = NULL;

    while (node) {
        node->parent = NULL;
        next = node->sibling;
        node->sibling = prev;
        prev = node;
        node = next;
    }
    return prev;
}

Heap* extractMinKey(Heap* heap, int* minKey) {
    if (!heap->head) return heap;

    Node* prevMin = NULL;
    Node* minNode = heap->head;
    Node* prev = NULL;
    Node* current = heap->head;

    int min = current->key;

    while (current) {
        if (current->key < min) {
            min = current->key;
            prevMin = prev;
            minNode = current;
        }
        prev = current;
        current = current->sibling;
    }

    *minKey = min;

    if (prevMin) {
        prevMin->sibling = minNode->sibling;
    } else {
        heap->head = minNode->sibling;
    }

    Node* childList = reverseList(minNode->child);
    Heap* newHeap = (Heap*)malloc(sizeof(Heap));
    newHeap->head = childList;

    Heap* mergedHeap = unionHeaps(heap, newHeap);

    free(minNode);
    free(newHeap);

    return mergedHeap;
}

Node* findNode(Node* root, int key) {
    if (!root) return NULL;
    if (root->key == key) return root;

    Node* found = findNode(root->child, key);
    if (found) return found;

    return findNode(root->sibling, key);
}

void decreaseKey(Heap* heap, int oldKey, int newKey) {
    if (newKey > oldKey) {
        printf("Error: new key is greater than old key\n");
        return;
    }

    Node* node = findNode(heap->head, oldKey);
    if (!node) {
        printf("Error: key not found\n");
        return;
    }

    node->key = newKey;
    Node* parent = node->parent;

    while (parent && node->key < parent->key) {
        int temp = node->key;
        node->key = parent->key;
        parent->key = temp;

        node = parent;
        parent = node->parent;
    }
}

void printTree(Node* root, int indent) {
    while (root) {
        for (int i = 0; i < indent; i++);
        printf("%d\n", root->key);
        printTree(root->child, indent + 1);
        root = root->sibling;
    }
}

void printHeap(Heap* heap) {
    printf("Binomial Heap:\n");
    printTree(heap->head, 0);
}

int main() {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->head = NULL;

    heap = insertKey(heap, 10);
    heap = insertKey(heap, 20);
    heap = insertKey(heap, 15);
    heap = insertKey(heap, 30);
    heap = insertKey(heap, 5);

    printHeap(heap);

    Node* minNode = findMinNode(heap);
    if (minNode)
        printf("Minimum key: %d\n", minNode->key);

    int minValue;
    heap = extractMinKey(heap, &minValue);
    printf("Extracted minimum key: %d\n", minValue);
    printHeap(heap);

    printf("Decrease key 20 to 2\n");
    decreaseKey(heap, 20, 2);
    printHeap(heap);

    Heap* heap2 = (Heap*)malloc(sizeof(Heap));
    heap2->head = NULL;
    heap2 = insertKey(heap2, 25);
    heap2 = insertKey(heap2, 7);

    printf("Second heap:\n");
    printHeap(heap2);

    heap = unionHeaps(heap, heap2);
    printf("After union:\n");
    printHeap(heap);

    free(heap);
    free(heap2);

    return 0;
}
