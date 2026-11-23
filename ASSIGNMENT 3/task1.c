#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

Node *newNode(int key) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) exit(1);
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

Node *insert(Node *root, int key) {
    if (!root) return newNode(key);
    if (key < root->key) root->left = insert(root->left, key);
    else if (key > root->key) root->right = insert(root->right, key);
    return root;
}

int search(Node *root, int key, int *comparisons) {
    *comparisons = 0;
    while (root) {
        (*comparisons)++;
        if (key == root->key) return 1;
        root = key < root->key ? root->left : root->right;
    }
    return 0;
}

int height(Node *root) {
    if (!root) return -1;
    int l = height(root->left), r = height(root->right);
    return 1 + (l > r ? l : r);
}

Node *minimum(Node *root) {
    while (root && root->left) root = root->left;
    return root;
}

Node *deleteNode(Node *root, int key) {
    if (!root) return NULL;
    if (key < root->key) root->left = deleteNode(root->left, key);
    else if (key > root->key) root->right = deleteNode(root->right, key);
    else {
        if (!root->left) {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        if (!root->right) {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        Node *successor = minimum(root->right);
        root->key = successor->key;
        root->right = deleteNode(root->right, successor->key);
    }
    return root;
}

void preorder(Node *r) {
    if (r) { printf("%d ", r->key); preorder(r->left); preorder(r->right); }
}
void inorder(Node *r) {
    if (r) { inorder(r->left); printf("%d ", r->key); inorder(r->right); }
}
void postorder(Node *r) {
    if (r) { postorder(r->left); postorder(r->right); printf("%d ", r->key); }
}
void levelorder(Node *root) {
    if (!root) return;
    Node *q[1000]; int front = 0, rear = 0;
    q[rear++] = root;
    while (front < rear) {
        Node *n = q[front++];
        printf("%d ", n->key);
        if (n->left) q[rear++] = n->left;
        if (n->right) q[rear++] = n->right;
    }
}
void freeTree(Node *r) {
    if (r) { freeTree(r->left); freeTree(r->right); free(r); }
}

int main(void) {
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    Node *root = NULL;
    for (int i = 0; i < 7; i++) root = insert(root, keys[i]);

    printf("Inorder: "); inorder(root);
    printf("\nPreorder: "); preorder(root);
    printf("\nPostorder: "); postorder(root);
    printf("\nLevel order: "); levelorder(root);

    int comparisons, target = 60;
    printf("\nSearch %d: %s", target,
           search(root, target, &comparisons) ? "Found" : "Not found");
    printf("\nComparisons: %d\nHeight: %d\n", comparisons, height(root));

    root = deleteNode(root, 20);
    root = deleteNode(root, 30);
    root = deleteNode(root, 50);
    printf("After deletion: "); inorder(root); printf("\n");

    freeTree(root);
    return 0;
}
