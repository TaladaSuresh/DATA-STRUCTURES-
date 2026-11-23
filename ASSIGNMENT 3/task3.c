#include <stdio.h>
#include <stdlib.h>

#define T 2
#define MAX_KEYS (2 * T - 1)
#define MAX_CHILD (2 * T)

typedef struct BTreeNode {
    int n, leaf;
    int keys[MAX_KEYS];
    struct BTreeNode *child[MAX_CHILD];
} BTreeNode;

BTreeNode *createNode(int leaf) {
    BTreeNode *x = (BTreeNode *)calloc(1, sizeof(BTreeNode));
    if (!x) exit(1);
    x->leaf = leaf;
    return x;
}

BTreeNode *search(BTreeNode *root, int key) {
    if (!root) return NULL;
    int i = 0;
    while (i < root->n && key > root->keys[i]) i++;
    if (i < root->n && key == root->keys[i]) return root;
    if (root->leaf) return NULL;
    return search(root->child[i], key);
}

void splitChild(BTreeNode *parent, int i) {
    BTreeNode *y = parent->child[i];
    BTreeNode *z = createNode(y->leaf);
    z->n = T - 1;
    for (int j = 0; j < T - 1; j++) z->keys[j] = y->keys[j + T];
    if (!y->leaf)
        for (int j = 0; j < T; j++) z->child[j] = y->child[j + T];
    y->n = T - 1;

    for (int j = parent->n; j >= i + 1; j--)
        parent->child[j + 1] = parent->child[j];
    parent->child[i + 1] = z;

    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];
    parent->keys[i] = y->keys[T - 1];
    parent->n++;
}

void insertNonFull(BTreeNode *x, int key) {
    int i = x->n - 1;
    if (x->leaf) {
        while (i >= 0 && key < x->keys[i]) {
            x->keys[i + 1] = x->keys[i]; i--;
        }
        if (i >= 0 && x->keys[i] == key) return;
        x->keys[i + 1] = key; x->n++;
    } else {
        while (i >= 0 && key < x->keys[i]) i--;
        i++;
        if (i < x->n && x->keys[i] == key) return;
        if (x->child[i]->n == MAX_KEYS) {
            splitChild(x, i);
            if (key > x->keys[i]) i++;
            else if (key == x->keys[i]) return;
        }
        insertNonFull(x->child[i], key);
    }
}

BTreeNode *insert(BTreeNode *root, int key) {
    if (!root) {
        root = createNode(1);
        root->keys[0] = key; root->n = 1;
        return root;
    }
    if (search(root, key)) return root;
    if (root->n == MAX_KEYS) {
        BTreeNode *s = createNode(0);
        s->child[0] = root;
        splitChild(s, 0);
        int i = key > s->keys[0] ? 1 : 0;
        insertNonFull(s->child[i], key);
        return s;
    }
    insertNonFull(root, key);
    return root;
}

void traverse(BTreeNode *root) {
    if (!root) return;
    int i;
    for (i = 0; i < root->n; i++) {
        if (!root->leaf) traverse(root->child[i]);
        printf("%d ", root->keys[i]);
    }
    if (!root->leaf) traverse(root->child[i]);
}
void freeTree(BTreeNode *r) {
    if (!r) return;
    if (!r->leaf)
        for (int i = 0; i <= r->n; i++) freeTree(r->child[i]);
    free(r);
}

int main(void) {
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    BTreeNode *root = NULL;
    for (int i = 0; i < 8; i++) root = insert(root, keys[i]);
    printf("B-tree keys in sorted order: ");
    traverse(root);
    printf("\nSearch 12: %s\n", search(root, 12) ? "Found" : "Not found");
    printf("Search 99: %s\n", search(root, 99) ? "Found" : "Not found");
    freeTree(root);
    return 0;
}
