#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHA 26 

struct Trie {
    struct Trie *child[ALPHA];
    int end;  
};

struct Trie* newNode() {
    struct Trie *n = (struct Trie*)malloc(sizeof(struct Trie));
    n->end = 0;
    for (int i = 0; i < ALPHA; i++)
        n->child[i] = NULL;
    return n;
}

void insert(struct Trie *root, char *word) {
    struct Trie *cur = root;
    for (int i = 0; word[i]; i++) {
        int idx = tolower(word[i]) - 'a';
        if (idx < 0 || idx >= ALPHA)
            continue; 
        if (cur->child[idx] == NULL)
            cur->child[idx] = newNode();
        cur = cur->child[idx];
    }
    cur->end = 1;
}

int search(struct Trie *root, char *word) {
    struct Trie *cur = root;
    for (int i = 0; word[i]; i++) {
        int idx = tolower(word[i]) - 'a';
        if (idx < 0 || idx >= ALPHA)
            return 0;  
        if (cur->child[idx] == NULL)
            return 0;
        cur = cur->child[idx];
    }
    return cur->end;
}

void suggestHelper(struct Trie *root, char *word, int level) {
    if (root->end) {
        word[level] = '\0';
        printf("%s\n", word);
    }
    for (int i = 0; i < ALPHA; i++) {
        if (root->child[i]) {
            word[level] = i + 'a';
            suggestHelper(root->child[i], word, level + 1);
        }
    }
}

void autoSuggest(struct Trie *root, char *prefix) {
    struct Trie *cur = root;
    for (int i = 0; prefix[i]; i++) {
        int idx = tolower(prefix[i]) - 'a';
        if (idx < 0 || idx >= ALPHA || cur->child[idx] == NULL) {
            printf("No suggestions found for '%s'\n", prefix);
            return;
        }
        cur = cur->child[idx];
    }

    char word[100];
    strcpy(word, prefix);
    suggestHelper(cur, word, strlen(prefix));
}

void loadDictionary(struct Trie *root) {
    char *words[] = {
        "apple","banana","carrot","dog","elephant","frog","grape","house","ice","jungle",
        "kite","lemon","mountain","night","orange","penguin","queen","river","sun","tiger",
        "umbrella","violet","water","xylophone","yellow","zebra","anchor","brave","circle","dance",
        "engine","forest","giant","heart","idea","jacket","knife","lamp","mirror","nose",
        "ocean","pillow","quiet","road","stone","train","under","voice","window","yard",
        "zero","ant","bread","cloud","dream","earth","feather","gold","hill","iron",
        "jewel","king","lion","moon","nest","owl","pear","quiz","rain","sand",
        "tree","unit","village","whale","xenon","year","zone","arrow","berry","chair",
        "dragon","energy","flame","gate","hammer","island","joy","key","leaf","music",
        "north","open","plate","queenly","rope","sugar","table","unique","vivid","wind"
    };

    int n = sizeof(words) / sizeof(words[0]);
    for (int i = 0; i < n; i++)
        insert(root, words[i]);
}

int main() {
    struct Trie *root = newNode();
    loadDictionary(root);

    int choice;
    char word[50], prefix[50];

    while (1) {
        printf("\n1. Search a word\n2. Auto-suggest words\n3. Exit\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter word to search: ");
            scanf("%s", word);
            for (int i = 0; word[i]; i++)
                word[i] = tolower(word[i]);  

            if (search(root, word))
                printf("'%s' found in dictionary.\n", word);
            else
                printf("'%s' NOT found in dictionary.\n", word);
            break;

        case 2:
            printf("Enter prefix: ");
            scanf("%s", prefix);
            for (int i = 0; prefix[i]; i++)
                prefix[i] = tolower(prefix[i]); 

            printf("Suggestions for '%s':\n", prefix);
            autoSuggest(root, prefix);
            break;

        case 3:
            printf("Exiting program.\n");
            return 0;

        default:
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
