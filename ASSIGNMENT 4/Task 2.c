/* TASK 2 - LIBRARY MEMBER LOOK-UP USING SEPARATE CHAINING */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETS 7

typedef struct Member {
    char id[12];
    char name[20];
    struct Member *next;
} Member;

Member *table[BUCKETS] = {NULL};

unsigned hash(const char *s) {
    unsigned h = 0;
    while (*s) h = h * 31u + (unsigned char)*s++;
    return h % BUCKETS;
}

void put(const char *id, const char *name) {
    unsigned index = hash(id);
    Member *node = malloc(sizeof(*node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    snprintf(node->id, sizeof(node->id), "%s", id);
    snprintf(node->name, sizeof(node->name), "%s", name);
    node->next = table[index];
    table[index] = node;
}

const char *get(const char *id, int *probes) {
    *probes = 0;
    for (Member *m = table[hash(id)]; m; m = m->next) {
        ++(*probes);
        if (strcmp(m->id, id) == 0) return m->name;
    }
    return NULL;
}

int remove_member(const char *id) {
    unsigned index = hash(id);
    Member **link = &table[index];
    while (*link) {
        if (strcmp((*link)->id, id) == 0) {
            Member *old = *link;
            *link = old->next;
            free(old);
            return 1;
        }
        link = &(*link)->next;
    }
    return 0;
}

int main(void) {
    const char *ids[] = {"CSE1043", "ECE2210", "MEC3307",
                         "CSE1044", "BME4120", "CIV5009"};
    const char *names[] = {"Kavya", "Arjun", "Fathima",
                           "Rahul", "Deepa", "Manoj"};

    for (int i = 0; i < 6; ++i) put(ids[i], names[i]);

    for (int b = 0; b < BUCKETS; ++b) {
        int length = 0;
        for (Member *m = table[b]; m; m = m->next) ++length;
        printf("%d", length);
    }

    int probes;
    const char *who = get("CSE1044", &probes);
    printf("\nCSE1044 -> %s (%d probes)\n", who ? who : "not found", probes);

    who = get("CSE9999", &probes);
    printf("CSE9999 -> %s (%d probes)\n", who ? who : "not found", probes);

    remove_member("ECE2210");

    for (int b = 0; b < BUCKETS; ++b) {
        while (table[b]) {
            Member *next = table[b]->next;
            free(table[b]);
            table[b] = next;
        }
    }
    return 0;
}
