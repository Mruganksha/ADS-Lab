#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node* left;
    struct Node* right;
};

struct Queue{
    struct Node** arr;
    int size;
    int capacity;
    int rear;
    int front;
};

struct Node* createNode(int data){
    struct Node* nn = (struct Node*)malloc(sizeof(struct Node));
    nn->data = data;
    nn->left = NULL;
    nn->right = NULL;
    return nn;
}

struct Queue* createQueue(int capacity){
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->capacity = capacity;
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    q->arr = (struct Node**)malloc(capacity * sizeof(struct Node*));
    return q;
}

void enqueue(struct Queue* q, struct Node* node){
    if(q->size == q->capacity){
        return;
    }
    q->rear = (q->rear + 1) % q->capacity;
    q->arr[q->rear] = node;
    q->size++;
}    

struct Node* dequeue(struct Queue* q){
    if(q->size == 0){
        return NULL;
    }  
    struct Node* node = q->arr[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return node;
}

struct Stack {
    struct Node** arr;
    int top;
    int capacity;
};

struct Stack* createStack(int capacity) {
    struct Stack* s = (struct Stack*)malloc(sizeof(struct Stack));
    s->capacity = capacity;
    s->top = -1;
    s->arr = (struct Node**)malloc(capacity * sizeof(struct Node*));
    return s;
}

int isEmpty(struct Stack* s) {
    return s->top == -1;
}

void push(struct Stack* s, struct Node* node) {
    if (s->top == s->capacity - 1) return; // Stack full
    s->arr[++s->top] = node;
}

struct Node* pop(struct Stack* s) {
    if (isEmpty(s)) return NULL;
    return s->arr[s->top--];
}

struct Node* peek(struct Stack* s) {
    if (isEmpty(s)) return NULL;
    return s->arr[s->top];
}

struct Node* insert(struct Node* root, int data){
    if(root == NULL){
        return createNode(data);
    }
    if(data < root->data){
        root->left = insert(root->left, data);
    } else if(data > root->data){
        root->right = insert(root->right, data);
    }
    return root;
}

void levelOrder(struct Node* root){
    if(root == NULL){
        return;
    }
    struct Queue* q = createQueue(100);
    enqueue(q, root);

    while(q->size > 0){
        struct Node* curr = dequeue(q);
        printf("%d ", curr->data);

        if(curr->left){
            enqueue(q, curr->left);
        }
        if(curr->right){
            enqueue(q, curr->right);
        }
    }
    printf("\n");
    free(q->arr);
    free(q);
}

// DFS
void inorder(struct Node* root){
    if(root == NULL){
        return;
    }
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);  
} 

void preorder(struct Node* root){
    if(root == NULL){
        return;
    }
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void postorder(struct Node* root){
    if(root == NULL){ 
        return;
    }
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);  
}      

void iterativeInorder(struct Node* root) {
    struct Stack* s = createStack(100);  
    struct Node* curr = root;

    while (curr != NULL || !isEmpty(s)) {
        while (curr != NULL) {
            push(s, curr);
            curr = curr->left;
        }
        curr = pop(s);
        printf("%d ", curr->data);
        curr = curr->right;
    }

    free(s->arr);
    free(s);
}

void iterativePreorder(struct Node* root) {
    if (root == NULL) return;

    struct Stack* s = createStack(100);
    push(s, root);

    while (!isEmpty(s)) {
        struct Node* curr = pop(s);
        printf("%d ", curr->data);

        if (curr->right) {
            push(s, curr->right); 
        }
        if (curr->left) {
            push(s, curr->left);
        }
    }

    free(s->arr);
    free(s);
}

void iterativePostorder(struct Node* root){
    if(root == NULL){
        return;
    }

    struct Stack* s1 = createStack(100);
    struct Stack* s2 = createStack(100);
    
    push(s1, root);

    while(!isEmpty(s1)){
        struct Node* n = pop(s1);
        push(s2, n);

        if(n->left){
            push(s1, n->left);
        }
        if(n->right){
            push(s1, n->right);
        }    
    }

    while(!isEmpty(s2)){
        struct Node* n = pop(s2);
        printf("%d ", n->data);
    }

    free(s1->arr);
    free(s1);
    free(s2->arr);
    free(s2);
}


int main() {
    struct Node* root = NULL;
    int n,val;
    printf("enter no. of nodes ");
    scanf("%d", &n);
    
    printf("Enter %d values: \n", n);
    for(int i = 0; i < n; i++){
      scanf("%d", &val);
      root = insert(root, val);
    }  
    /*
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);*/

    printf("Inorder Traversal: ");
    inorder(root);
    printf("\nIterative Inorder Traversal: ");
    iterativeInorder(root);
    
    printf("\nPreorder Traversal: ");
    preorder(root);
    printf("\nIterative Preorder Traversal: ");
    iterativePreorder(root);
    
    printf("\nPostorder Traversal: ");
    postorder(root);
    printf("\nIterative Postorder Traversal: ");
    iterativePostorder(root);
    
    printf("\nLevel Order Traversal: ");
    levelOrder(root);
    
    return 0;
}
