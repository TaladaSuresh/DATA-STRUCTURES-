#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 1000
#define WORD_LEN 64

typedef struct TrieNode {
    struct TrieNode *child[26];
    int isEnd;
} TrieNode;

TrieNode *newTrieNode(void) {
    TrieNode *n = (TrieNode *)calloc(1, sizeof(TrieNode));
    if (!n) exit(1);
    return n;
}
void trieInsert(TrieNode *root, const char *word) {
    TrieNode *cur = root;
    for (int i = 0; word[i]; i++) {
        if (word[i] < 'a' || word[i] > 'z') continue;
        int k = word[i] - 'a';
        if (!cur->child[k]) cur->child[k] = newTrieNode();
        cur = cur->child[k];
    }
    cur->isEnd = 1;
}
int trieSearch(TrieNode *root, const char *word) {
    TrieNode *cur = root;
    for (int i = 0; word[i]; i++) {
        if (word[i] < 'a' || word[i] > 'z') return 0;
        int k = word[i] - 'a';
        if (!cur->child[k]) return 0;
        cur = cur->child[k];
    }
    return cur->isEnd;
}
void freeTrie(TrieNode *r) {
    if (!r) return;
    for (int i = 0; i < 26; i++) freeTrie(r->child[i]);
    free(r);
}
int compareStrings(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}
void listPrefix(char words[][WORD_LEN], int count, const char *prefix) {
    int found = 0, len = (int)strlen(prefix);
    for (int i = 0; i < count; i++) {
        if (strncmp(words[i], prefix, len) == 0) {
            printf("%s\n", words[i]);
            found = 1;
        }
    }
    if (!found) printf("No matching words\n");
}
void rangeList(char words[][WORD_LEN], int count, const char *first, const char *last) {
    for (int i = 0; i < count; i++)
        if (strcmp(words[i], first) >= 0 && strcmp(words[i], last) <= 0)
            printf("%s\n", words[i]);
}
int main(void) {
    char words[][WORD_LEN] = {
        "apple", "apply", "banana", "band", "cat",
        "cater", "dog", "door", "dove", "zebra"
    };
    int count = 10;
    TrieNode *root = newTrieNode();
    for (int i = 0; i < count; i++) trieInsert(root, words[i]);
    qsort(words, count, WORD_LEN, compareStrings);

    printf("Exact search 'banana': %s\n",
           trieSearch(root, "banana") ? "Found" : "Not found");
    printf("Autocomplete for prefix 'ca':\n");
    listPrefix(words, count, "ca");
    printf("Alphabetical range from 'band' to 'dog':\n");
    rangeList(words, count, "band", "dog");

    freeTrie(root);
    return 0;
}
