/*
Van Phat Phan
HW6 - Inventory.c
COP 3502 Seciton 4
Fall 2023
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 19
#define TABLESIZE 300007
#define INITAL_BUDGET 100000;

typedef struct item
{
    char name[MAXLEN + 1];
    int quantity;
    int saleprice;
} item;

typedef struct node
{
    item *iPtr;
    struct node *next;
} node;

typedef struct hashtable
{
    node **lists;
    int size;
} hashtable;

// global variable
int budget;
hashtable *hashmap;
int performance_count;

// to generate hash for hash table
int hashfunc(char *word, int size);

// command function
void buy_comand();
void change_price_command();
void sell_command();
void free_memory();
void free_memory_linkedList(node *front);
// utilities function
void insert_new_item(char name[], int quantity, int buy_price);
item *create_new_item(char name[], int quantity, int buy_price);
node *create_new_node(item *item_ptr);
void display_hashmap();
void print_item(node *crr_node);
void sell_item(node *node, int quantity);
void update_item_price(node *node, int new_price);
node *search_node(node *front, char name[]);

int main()
{
    // initialize variables
    budget = INITAL_BUDGET;
    hashmap = malloc(sizeof(hashtable));
    hashmap->lists = calloc(TABLESIZE, sizeof(node *));
    hashmap->size = TABLESIZE;
    performance_count = 0;

    int num_cmd;
    scanf("%d", &num_cmd);

    for (int i = 0; i < num_cmd; i += 1)
    {
        char command[15];
        scanf("%s", command);

        if (strcmp(command, "buy") == 0)
            buy_comand();
        else if (strcmp(command, "sell") == 0)
            sell_command();
        else if (strcmp(command, "change_price") == 0)
            change_price_command();
    }

    // display_hashmap();

    free_memory();

    printf("%d\n", budget);
    printf("%d\n", performance_count);
    return 0;
}

int hashfunc(char *word, int size)
{
    int len = strlen(word);
    int res = 0;
    for (int i = 0; i < len; i++)
        res = (1151 * res + (word[i] - 'a')) % size;
    return res;
}

void buy_comand()
{
    char name[MAXLEN];
    int quantity;
    int totalPrice;
    scanf("%s %d %d", name, &quantity, &totalPrice);

    insert_new_item(name, quantity, totalPrice);
}
void change_price_command()
{
    char name[MAXLEN];
    int new_price = 0;
    scanf("%s %d", name, &new_price);
    int hash_index = hashfunc(name, TABLESIZE);
    node *front = hashmap->lists[hash_index];
    node *node = search_node(front, name);
    update_item_price(node, new_price);
    return;
}
void sell_command()
{
    char name[19];
    int quantity = 0;
    scanf("%s %d", name, &quantity);
    int hash_index = hashfunc(name, TABLESIZE);
    node *front = hashmap->lists[hash_index];
    node *node = search_node(front, name);
    sell_item(front, quantity);
    return;
}
item *create_new_item(char name[], int quantity, int buy_price)
{
    item *new_item = malloc(sizeof(item));
    strcpy(new_item->name, name);
    new_item->quantity = quantity;
    new_item->saleprice = buy_price / quantity;
    return new_item;
}
node *create_new_node(item *item_ptr)
{
    node *new_node = malloc(sizeof(node));
    new_node->iPtr = item_ptr;
    new_node->next = NULL;
    return new_node;
}

node *search_node(node *front, char name[])
{
    performance_count += 1;
    if (front == NULL || strcmp(name, front->iPtr->name) == 0)
        return front;
    return search_node(front->next, name);
}
void insert_new_item(char name[], int quantity, int buy_price)
{
    int hash_index = hashfunc(name, TABLESIZE);

    node *prev = NULL;
    node *node = hashmap->lists[hash_index];
    while (node != NULL && strcmp(node->iPtr->name, name) != 0)
    {
        performance_count += 1;
        prev = node;
        node = node->next;
    }
    // if (node == NULL)
    // {
    //     item *new_item = create_new_item(name, quantity, buy_price);
    //     node = create_new_node(new_item);
    //     performance_count += 1;
    //     budget -= buy_price;
    //     hashmap->lists[hash_index] = node;
    //     return;
    // }
}

void sell_item(node *node, int quantity)
{
    if (node->iPtr->quantity < quantity)
        quantity = node->iPtr->quantity;
    budget += quantity * node->iPtr->saleprice;
    node->iPtr->quantity -= quantity;
    // performance_count += k;
    print_item(node);
    // printf("Performance %d \n", performance_count);
    printf("\nPerformance %d \n", performance_count);
}
void free_memory()
{
    for (int i = 0; i < hashmap->size; i += 1)
    {
        if (hashmap->lists[i] != NULL)
        {
            free_memory_linkedList(hashmap->lists[i]);
        }
    }
    free(hashmap->lists);
    free(hashmap);
}

void update_item_price(node *node, int new_price)
{
    node->iPtr->saleprice = new_price;
}
void free_memory_linkedList(node *front)
{
    while (front != NULL)
    {
        node *next = front->next;
        free(front->iPtr);
        free(front);
        front = next;
    }
}

void display_hashmap()
{
    for (int i = 0; i < hashmap->size; i += 1)
    {
        if (hashmap->lists[i] == NULL)
            continue;
        printf("at index %d\n", i);
        node *front = hashmap->lists[i];
        while (front != NULL)
        {
            printf("name = %s quantity = %d sale_price %d \n", front->iPtr->name, front->iPtr->quantity, front->iPtr->saleprice);
            front = front->next;
        }
    }
}

void print_item(node *crr_node)
{
    printf("%s %d %d\n", crr_node->iPtr->name, crr_node->iPtr->quantity, budget);
}