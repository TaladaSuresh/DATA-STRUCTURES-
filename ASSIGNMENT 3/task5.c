#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

Node *newNode(int key) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) exit(1);
    n->key = key; n->left = n->right = NULL;
    return n;
}
Node *rotateRight(Node *x) {
    Node *y = x->left;
    x->left = y->right; y->right = x;
    return y;
}
Node *rotateLeft(Node *x) {
    Node *y = x->right;
    x->right = y->left; y->left = x;
    return y;
}
Node *splay(Node *root, int key) {
    if (!root || root->key == key) return root;
    if (key < root->key) {
        if (!root->left) return root;
        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = rotateRight(root);
        } else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right) root->left = rotateLeft(root->left);
        }
        return root->left ? rotateRight(root) : root;
    } else {
        if (!root->right) return root;
        if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = rotateLeft(root);
        } else if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left) root->right = rotateRight(root->right);
        }
        return root->right ? rotateLeft(root) : root;
    }
}
Node *insert(Node *root, int key) {
    if (!root) return newNode(key);
    root = splay(root, key);
    if (root->key == key) return root;
    Node *n = newNode(key);
    if (key < root->key) {
        n->right = root; n->left = root->left; root->left = NULL;
    } else {
        n->left = root; n->right = root->right; root->right = NULL;
    }
    return n;
}
void inorder(Node *r) {
    if (r) { inorder(r->left); printf("%d ", r->key); inorder(r->right); }
}
void freeTree(Node *r) {
    if (r) { freeTree(r->left); freeTree(r->right); free(r); }
}
int main(void) {
    int keys[] = {5, 3, 7, 1, 9, 6};
    Node *root = NULL;
    for (int i = 0; i < 6; i++) root = insert(root, keys[i]);
    printf("Inorder: "); inorder(root);
    root = splay(root, 3);
    printf("\nRoot after accessing 3: %d", root->key);
    root = splay(root, 9);
    printf("\nRoot after accessing 9: %d\n", root->key);
    freeTree(root);
    return 0;
}
