#include <stdio.h>
#include <stdlib.h>

enum { RED, BLACK };

typedef struct Node {
    int key, color;
    struct Node *left, *right, *parent;
} Node;

Node *root = NULL;

Node *newNode(int key) {
    Node *n = (Node *)calloc(1, sizeof(Node));
    if (!n) exit(1);
    n->key = key; n->color = RED;
    return n;
}
int colorOf(Node *n) { return n ? n->color : BLACK; }

void rotateLeft(Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x; x->parent = y;
}
void rotateRight(Node *y) {
    Node *x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent) root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y; y->parent = x;
}

void fixInsert(Node *z) {
    while (z->parent && z->parent->color == RED) {
        Node *p = z->parent, *g = p->parent;
        if (p == g->left) {
            Node *u = g->right;
            if (colorOf(u) == RED) {
                p->color = BLACK; u->color = BLACK; g->color = RED; z = g;
            } else {
                if (z == p->right) { z = p; rotateLeft(z); p = z->parent; g = p->parent; }
                p->color = BLACK; g->color = RED; rotateRight(g);
            }
        } else {
            Node *u = g->left;
            if (colorOf(u) == RED) {
                p->color = BLACK; u->color = BLACK; g->color = RED; z = g;
            } else {
                if (z == p->left) { z = p; rotateRight(z); p = z->parent; g = p->parent; }
                p->color = BLACK; g->color = RED; rotateLeft(g);
            }
        }
    }
    root->color = BLACK;
}

void insert(int key) {
    Node *z = newNode(key), *y = NULL, *x = root;
    while (x) {
        y = x;
        if (key < x->key) x = x->left;
        else if (key > x->key) x = x->right;
        else { free(z); return; }
    }
    z->parent = y;
    if (!y) root = z;
    else if (key < y->key) y->left = z;
    else y->right = z;
    fixInsert(z);
}
Node *minimum(Node *n) {
    while (n && n->left) n = n->left;
    return n;
}
void inorder(Node *n) {
    if (n) {
        inorder(n->left);
        printf("%d(%s) ", n->key, n->color == RED ? "R" : "B");
        inorder(n->right);
    }
}
void freeTree(Node *n) {
    if (n) { freeTree(n->left); freeTree(n->right); free(n); }
}

int main(void) {
    int keys[] = {10, 20, 30, 15, 25, 5};
    for (int i = 0; i < 6; i++) insert(keys[i]);
    printf("Red-Black tree inorder: "); inorder(root);
    Node *m = minimum(root);
    if (m) printf("\nSmallest key: %d\n", m->key);
    freeTree(root);
    return 0;
}
