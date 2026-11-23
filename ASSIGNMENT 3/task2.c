#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key, height;
    struct Node *left, *right;
} Node;

int max(int a, int b) { return a > b ? a : b; }
int height(Node *n) { return n ? n->height : 0; }

Node *newNode(int key) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) exit(1);
    n->key = key; n->height = 1;
    n->left = n->right = NULL;
    return n;
}

void updateHeight(Node *n) {
    n->height = 1 + max(height(n->left), height(n->right));
}
int balance(Node *n) {
    return n ? height(n->left) - height(n->right) : 0;
}

Node *rotateRight(Node *y) {
    Node *x = y->left, *t = x->right;
    x->right = y; y->left = t;
    updateHeight(y); updateHeight(x);
    return x;
}
Node *rotateLeft(Node *x) {
    Node *y = x->right, *t = y->left;
    y->left = x; x->right = t;
    updateHeight(x); updateHeight(y);
    return y;
}

Node *insert(Node *root, int key) {
    if (!root) return newNode(key);
    if (key < root->key) root->left = insert(root->left, key);
    else if (key > root->key) root->right = insert(root->right, key);
    else return root;

    updateHeight(root);
    int b = balance(root);

    if (b > 1 && key < root->left->key) return rotateRight(root);
    if (b < -1 && key > root->right->key) return rotateLeft(root);
    if (b > 1 && key > root->left->key) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (b < -1 && key < root->right->key) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

void preorder(Node *r) {
    if (r) {
        printf("%d ", r->key);
        preorder(r->left); preorder(r->right);
    }
}
void freeTree(Node *r) {
    if (r) { freeTree(r->left); freeTree(r->right); free(r); }
}

int main(void) {
    int keys[] = {10, 20, 30, 40, 50, 25};
    Node *root = NULL;
    for (int i = 0; i < 6; i++) {
        root = insert(root, keys[i]);
        printf("After inserting %d: ", keys[i]);
        preorder(root); printf("\n");
    }
    printf("Final height: %d\n", root->height);
    freeTree(root);
    return 0;
}
