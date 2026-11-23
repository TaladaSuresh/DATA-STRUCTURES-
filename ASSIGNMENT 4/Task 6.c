/* TASK 6 - FOOD DELIVERY ORDER PROCESSING
   Hash table: order ID lookup; max-heap: most overdue order;
   stable report ordering: restaurant, then amount, then insertion order. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 100003
#define MAX_ORDERS 100000

typedef struct Order {
    int id;
    char restaurant[32];
    int amount;
    int overdue_minutes;
    int active;
    unsigned long sequence;
    struct Order *hash_next;
} Order;

typedef struct {
    Order *items[MAX_ORDERS];
    int size;
} Heap;

static Order *hash_table[HASH_SIZE];
static unsigned long next_sequence = 0;

unsigned hash_id(int id) {
    return (unsigned)id % HASH_SIZE;
}

Order *find_order(int id) {
    for (Order *p = hash_table[hash_id(id)]; p; p = p->hash_next)
        if (p->id == id) return p;
    return NULL;
}

void insert_hash(Order *order) {
    unsigned index = hash_id(order->id);
    order->hash_next = hash_table[index];
    hash_table[index] = order;
}

int remove_hash(int id) {
    unsigned index = hash_id(id);
    Order **link = &hash_table[index];
    while (*link) {
        if ((*link)->id == id) {
            *link = (*link)->hash_next;
            return 1;
        }
        link = &(*link)->hash_next;
    }
    return 0;
}

int higher(Order *a, Order *b) {
    if (a->overdue_minutes != b->overdue_minutes)
        return a->overdue_minutes > b->overdue_minutes;
    return a->sequence < b->sequence;
}

void heap_swap(Order **a, Order **b) {
    Order *t = *a; *a = *b; *b = t;
}

void heap_push(Heap *h, Order *order) {
    if (h->size >= MAX_ORDERS) return;
    int i = h->size++;
    h->items[i] = order;

    while (i > 0) {
        int p = (i - 1) / 2;
        if (!higher(h->items[i], h->items[p])) break;
        heap_swap(&h->items[i], &h->items[p]);
        i = p;
    }
}

void heap_down(Heap *h, int i) {
    while (1) {
        int l = 2 * i + 1, r = 2 * i + 2, best = i;
        if (l < h->size && higher(h->items[l], h->items[best])) best = l;
        if (r < h->size && higher(h->items[r], h->items[best])) best = r;
        if (best == i) break;
        heap_swap(&h->items[i], &h->items[best]);
        i = best;
    }
}

Order *heap_pop(Heap *h) {
    while (h->size > 0) {
        Order *top = h->items[0];
        h->items[0] = h->items[--h->size];
        if (h->size) heap_down(h, 0);
        if (top->active) return top; /* skips cancelled/stale entries */
    }
    return NULL;
}

int place_order(Heap *h, int id, const char *restaurant,
                int amount, int overdue) {
    if (find_order(id) || h->size >= MAX_ORDERS) return 0;

    Order *order = calloc(1, sizeof(*order));
    if (!order) return 0;
    order->id = id;
    snprintf(order->restaurant, sizeof(order->restaurant), "%s", restaurant);
    order->amount = amount;
    order->overdue_minutes = overdue;
    order->active = 1;
    order->sequence = next_sequence++;

    insert_hash(order);
    heap_push(h, order);
    return 1;
}

int cancel_order(int id) {
    Order *order = find_order(id);
    if (!order) return 0;
    order->active = 0;
    remove_hash(id);
    return 1;
}

Order *assign_next_order(Heap *h) {
    Order *order = heap_pop(h);
    if (order) {
        order->active = 0;
        remove_hash(order->id);
    }
    return order;
}

int compare_report(const void *x, const void *y) {
    const Order *a = *(const Order * const *)x;
    const Order *b = *(const Order * const *)y;
    int c = strcmp(a->restaurant, b->restaurant);
    if (c) return c;
    if (a->amount != b->amount) return (a->amount > b->amount) - (a->amount < b->amount);
    return (a->sequence > b->sequence) - (a->sequence < b->sequence);
}

void report_orders(void) {
    Order *list[MAX_ORDERS];
    int count = 0;

    for (int i = 0; i < HASH_SIZE; ++i)
        for (Order *p = hash_table[i]; p; p = p->hash_next)
            if (p->active) list[count++] = p;

    qsort(list, (size_t)count, sizeof(list[0]), compare_report);

    puts("Hourly order report:");
    for (int i = 0; i < count; ++i)
        printf("ID=%d Restaurant=%s Amount=%d Overdue=%d\n",
               list[i]->id, list[i]->restaurant,
               list[i]->amount, list[i]->overdue_minutes);
}

int main(void) {
    Heap heap = {{NULL}, 0};

    place_order(&heap, 1001, "FoodHub", 250, 18);
    place_order(&heap, 1002, "SpiceHouse", 180, 35);
    place_order(&heap, 1003, "FoodHub", 300, 12);
    place_order(&heap, 1004, "SpiceHouse", 220, 35);

    Order *found = find_order(1003);
    printf("Search 1003: %s\n", found ? "Found" : "Not found");

    printf("Cancel 1001: %s\n", cancel_order(1001) ? "Success" : "Failed");

    Order *next = assign_next_order(&heap);
    if (next) printf("Assigned order: %d (overdue %d minutes)\n",
                     next->id, next->overdue_minutes);

    report_orders();

    for (int i = 0; i < HASH_SIZE; ++i) {
        Order *p = hash_table[i];
        while (p) {
            Order *next_order = p->hash_next;
            free(p);
            p = next_order;
        }
    }
    return 0;
}
