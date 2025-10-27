#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256  

struct node {
    char ch;             
    int freq;             
    struct node *left;    
    struct node *right;  
};

struct heap {
    int size;
    struct node *arr[MAX];
};

struct node* newNode(char ch, int freq) {
    struct node* n = (struct node*)malloc(sizeof(struct node));
    n->ch = ch;
    n->freq = freq;
    n->left = n->right = NULL;
    return n;
}

void swap(struct node** a, struct node** b) {
    struct node* t = *a;
    *a = *b;
    *b = t;
}

void heapify(struct heap* h, int i) {
    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < h->size && h->arr[l]->freq < h->arr[smallest]->freq)
        smallest = l;

    if (r < h->size && h->arr[r]->freq < h->arr[smallest]->freq)
        smallest = r;

    if (smallest != i) {
        swap(&h->arr[i], &h->arr[smallest]);
        heapify(h, smallest);
    }
}

struct node* extractMin(struct heap* h) {
    struct node* temp = h->arr[0];
    h->arr[0] = h->arr[h->size - 1];
    h->size--;
    heapify(h, 0);
    return temp;
}

void insertHeap(struct heap* h, struct node* n) {
    h->size++;
    int i = h->size - 1;
    h->arr[i] = n;

    while (i && h->arr[i]->freq < h->arr[(i - 1) / 2]->freq) {
        swap(&h->arr[i], &h->arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

struct heap* buildHeap(char data[], int freq[], int n) {
    struct heap* h = (struct heap*)malloc(sizeof(struct heap));
    h->size = n;

    for (int i = 0; i < n; i++)
        h->arr[i] = newNode(data[i], freq[i]);

    for (int i = (n - 2) / 2; i >= 0; i--)
        heapify(h, i);

    return h;
}

struct node* buildHuffman(char data[], int freq[], int n) {
    struct heap* h = buildHeap(data, freq, n);

    while (h->size > 1) {
        struct node* left = extractMin(h);
        struct node* right = extractMin(h);

        struct node* top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertHeap(h, top);
    }

    return extractMin(h);
}

void printCodes(struct node* root, int arr[], int top, char codeTable[MAX][MAX]) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1, codeTable);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1, codeTable);
    }

    if (!root->left && !root->right && root->ch != '$') {
        printf("'%c': ", root->ch);
        for (int i = 0; i < top; i++)
            printf("%d", arr[i]);
        printf("\n");

        for (int i = 0; i < top; i++)
            codeTable[(unsigned char)root->ch][i] = arr[i] + '0';
        codeTable[(unsigned char)root->ch][top] = '\0';
    }
}

// Encode text
void encodeText(char text[], char codeTable[MAX][MAX], char encoded[]) {
    encoded[0] = '\0';
    for (int i = 0; text[i]; i++)
        strcat(encoded, codeTable[(unsigned char)text[i]]);
}

// Decode text
void decodeText(struct node* root, char encoded[]) {
    struct node* cur = root;
    printf("\nDecoded Text: ");
    for (int i = 0; encoded[i]; i++) {
        if (encoded[i] == '0')
            cur = cur->left;
        else
            cur = cur->right;

        if (!cur->left && !cur->right) {
            printf("%c", cur->ch);
            cur = root;
        }
    }
    printf("\n");
}

int main() {
    char text[100];
    printf("Enter text: ");
    scanf("%[^\n]", text);

    int freq[MAX] = {0};
    char data[MAX];
    int n = 0;

    for (int i = 0; text[i]; i++)
        freq[(unsigned char)text[i]]++;

    for (int i = 0; i < MAX; i++) {
        if (freq[i] > 0) {
            data[n] = i;
            freq[n] = freq[i];
            n++;
        }
    }

    struct node* root = buildHuffman(data, freq, n);

    int arr[MAX], top = 0;
    char codeTable[MAX][MAX] = {{0}};

    printf("\nCharacter Codes:\n");
    printCodes(root, arr, top, codeTable);

    char encoded[1000];
    encodeText(text, codeTable, encoded);

    printf("\nEncoded Message: %s\n", encoded);

    decodeText(root, encoded);

    return 0;
}
