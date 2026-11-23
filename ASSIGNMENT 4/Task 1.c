/* TASK 1 - HASH FUNCTION DESIGN REVIEW */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100000
#define BUCKETS 100003
#define REG_LEN 12

typedef struct Node {
    char reg[REG_LEN];
    struct Node *next;
} Node;

static unsigned poor_hash(const char *s) {
    return ((unsigned char)s[0] + (unsigned char)s[1]) % BUCKETS;
}

static unsigned good_hash(const char *s) {
    unsigned long h = 0;
    while (*s) h = h * 31u + (unsigned char)*s++;
    return (unsigned)(h % BUCKETS);
}

static void insert(Node *table[], const char *reg, unsigned (*hash_fn)(const char *)) {
    unsigned index = hash_fn(reg);
    Node *node = malloc(sizeof(*node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(node->reg, reg);
    node->next = table[index];
    table[index] = node;
}

static void measure(Node *table[], unsigned (*hash_fn)(const char *), const char *label) {
    size_t longest = 0, nonempty = 0, total = 0;
    for (int i = 0; i < BUCKETS; ++i) {
        size_t length = 0;
        for (Node *p = table[i]; p; p = p->next) ++length;
        if (length) ++nonempty;
        if (length > longest) longest = length;
        total += length;
    }
    printf("%s\n", label);
    printf("Records: %zu\n", total);
    printf("Non-empty buckets: %zu\n", nonempty);
    printf("Longest chain: %zu\n", longest);
    printf("Average chain length (all buckets): %.4f\n",
           (double)total / BUCKETS);
    printf("Average chain length (non-empty buckets): %.4f\n\n",
           nonempty ? (double)total / nonempty : 0.0);
    (void)hash_fn;
}

static void free_table(Node *table[]) {
    for (int i = 0; i < BUCKETS; ++i) {
        while (table[i]) {
            Node *next = table[i]->next;
            free(table[i]);
            table[i] = next;
        }
    }
}

int main(void) {
    Node **poor = calloc(BUCKETS, sizeof(*poor));
    Node **good = calloc(BUCKETS, sizeof(*good));
    if (!poor || !good) {
        fprintf(stderr, "Memory allocation failed\n");
        free(poor); free(good);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < N; ++i) {
        char reg[REG_LEN];
        /* Same state prefix models a large number of registrations from one state. */
        snprintf(reg, sizeof(reg), "TN%02d%c%c%04d",
                 (i / 10000) % 100, 'A' + (i / 26) % 26,
                 'A' + i % 26, i % 10000);
        insert(poor, reg, poor_hash);
        insert(good, reg, good_hash);
    }

    measure(poor, poor_hash, "Poor hash function");
    measure(good, good_hash, "Polynomial rolling hash");

    free_table(poor);
    free_table(good);
    free(poor);
    free(good);
    return 0;
}
