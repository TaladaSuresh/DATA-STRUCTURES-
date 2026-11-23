#include <stdio.h>
#include <stdlib.h>

typedef struct Trie {
    struct Trie *child[26];
    int isEnd;
    int wordCount;
} Trie;

Trie *newNode(void) {
    Trie *n = (Trie *)calloc(1, sizeof(Trie));
    if (!n) exit(1);
    return n;
}
void insert(Trie *root, const char *word) {
    Trie *cur = root;
    for (int i = 0; word[i]; i++) {
        if (word[i] < 'a' || word[i] > 'z') continue;
        int index = word[i] - 'a';
        if (!cur->child[index]) cur->child[index] = newNode();
        cur = cur->child[index];
        cur->wordCount++;
    }
    cur->isEnd = 1;
}
Trie *prefixNode(Trie *root, const char *prefix) {
    Trie *cur = root;
    for (int i = 0; prefix[i]; i++) {
        if (prefix[i] < 'a' || prefix[i] > 'z') return NULL;
        int index = prefix[i] - 'a';
        if (!cur->child[index]) return NULL;
        cur = cur->child[index];
    }
    return cur;
}
int countPrefix(Trie *root, const char *prefix) {
    Trie *n = prefixNode(root, prefix);
    return n ? n->wordCount : 0;
}
int search(Trie *root, const char *word) {
    Trie *n = prefixNode(root, word);
    return n && n->isEnd;
}
void printWords(Trie *node, char buffer[], int depth) {
    if (!node) return;
    if (node->isEnd) { buffer[depth] = '\0'; printf("%s\n", buffer); }
    for (int i = 0; i < 26; i++) {
        if (node->child[i]) {
            buffer[depth] = (char)('a' + i);
            printWords(node->child[i], buffer, depth + 1);
        }
    }
}
void autocomplete(Trie *root, const char *prefix) {
    Trie *n = prefixNode(root, prefix);
    if (!n) { printf("No matches\n"); return; }
    char buffer[256];
    int len = 0;
    while (prefix[len] && len < 254) { buffer[len] = prefix[len]; len++; }
    printWords(n, buffer, len);
}
void freeTrie(Trie *root) {
    if (!root) return;
    for (int i = 0; i < 26; i++) freeTrie(root->child[i]);
    free(root);
}
int main(void) {
    const char *names[] = {"anand", "anitha", "anu", "arun", "bala", "anusuya"};
    Trie *root = newNode();
    for (int i = 0; i < 6; i++) insert(root, names[i]);
    printf("Prefix counts:\n");
    printf("a:%d an:%d anu:%d b:%d z:%d\n",
           countPrefix(root, "a"), countPrefix(root, "an"),
           countPrefix(root, "anu"), countPrefix(root, "b"),
           countPrefix(root, "z"));
    printf("Search anand: %s\n", search(root, "anand") ? "Found" : "Not found");
    printf("Autocomplete for 'an':\n");
    autocomplete(root, "an");
    freeTrie(root);
    return 0;
}
