#include<stdio.h>
#include<stdlib.h>
#define MAX_LEVEL 6
#define P 0.5

typedef struct node {
    int key; 
    struct node** forward;
} node;

typedef struct skiplist {
    int level;
    struct node *header;
} skiplist;

node* create_node(int level, int key) {
    struct node *nn = (node*)malloc(sizeof(struct node));
    nn->key = key;
    nn->forward = (node**)malloc(sizeof(struct node*) * (level + 1));
    for (int i = 0; i <= level; i++) {
        nn->forward[i] = NULL;
    }
    return nn;
}

skiplist* create_skiplist() {
    skiplist *list = (skiplist*)malloc(sizeof(skiplist));
    list->level = 0;
    list->header = create_node(MAX_LEVEL, -1);
    return list;
}

int random_level() {
    int level = 0;
    while (((double)rand() / RAND_MAX) < P && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

void insert(skiplist *list, int key) {
    node *update[MAX_LEVEL + 1];
    node *x = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] != NULL && x->forward[i]->key < key) {
            x = x->forward[i];
        }
        update[i] = x;
    }

    x = x->forward[0];

    if (x == NULL || x->key != key) {
        int lvl = random_level();
        if (lvl > list->level) {
            for (int i = list->level + 1; i <= lvl; i++) {
                update[i] = list->header;
            }
            list->level = lvl;
        }

        x = create_node(lvl, key);
        for (int i = 0; i <= lvl; i++) {
            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
    }
}

void delete(skiplist *list, int key) {
    node *update[MAX_LEVEL + 1];
    node *x = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] != NULL && x->forward[i]->key < key) {
            x = x->forward[i];
        }
        update[i] = x;
    }

    x = x->forward[0];

    if (x != NULL && x->key == key) {
        for (int i = 0; i <= list->level; i++) {
            if (update[i]->forward[i] != x)
                break;
            update[i]->forward[i] = x->forward[i];
        }

        free(x->forward);
        free(x);

        while (list->level > 0 && list->header->forward[list->level] == NULL) {
            list->level--;
        }

        printf("Deleted key %d\n", key);
    } else {
        printf("Key %d not found, cannot delete.\n", key);
    }
}

void print(skiplist *list) {
    printf("\nSkip List Levels:\n");
    for (int i = list->level; i >= 0; i--) {
        node *x = list->header->forward[i];
        printf("Level %d: ", i);
        while (x != NULL) {
            printf("%d ", x->key);
            x = x->forward[i];
        }
        printf("\n");
    }
    printf("\n");
}

node* search(skiplist *list, int key) {
    node *x = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] != NULL && x->forward[i]->key < key) {
            x = x->forward[i];
        }
    }
    x = x->forward[0];
    if (x != NULL && x->key == key)
        return x;
    else
        return NULL;
}

int main() {
    skiplist *list = create_skiplist();

    insert(list, 3);
    insert(list, 6);
    insert(list, 7);
    insert(list, 9);
    insert(list, 12);
    insert(list, 19);
    insert(list, 17);
    insert(list, 26);
    insert(list, 21);
    insert(list, 25);

    print(list);

    int x = 19;
    node *found = search(list, x);
    if (found != NULL)
        printf("Found key %d\n", x);
    else
        printf("Key %d not found\n", x);

    delete(list, 19);
    delete(list, 7);
    delete(list, 50); 

    print(list);

    return 0;
}
