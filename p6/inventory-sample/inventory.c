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
void sell_item(char name[], int quantity);
void update_item_price(char name[], int new_price);

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
    char item_str[MAXLEN];
    int quantity;
    int totalPrice;
    scanf("%s %d %d", item_str, &quantity, &totalPrice);

    insert_new_item(item_str, quantity, totalPrice);
}
void change_price_command()
{
    char name[MAXLEN];
    int new_price = 0;
    scanf("%s %d", name, &new_price);

    update_item_price(name, new_price);
    return;
}
void sell_command()
{
    char name[19];
    int quantity = 0;
    scanf("%s %d", name, &quantity);
    sell_item(name, quantity);
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

void insert_new_item(char name[], int quantity, int buy_price)
{
    int hash_index = hashfunc(name, TABLESIZE);
    node *front = hashmap->lists[hash_index];
    node *prev = front;
    int k = 0;
    while (front != NULL)
    {
        k += 1;
        if (strcmp(front->iPtr->name, name) == 0)
        {
            // node exisited
            front->iPtr->quantity += quantity;
            budget -= buy_price;
            performance_count += k;
            print_item(front);
            return;
        }
        prev = front;
        front = front->next;
    }
    // at this point, we need to create new node,
    item *new_item = create_new_item(name, quantity, buy_price);
    front = create_new_node(new_item);
    front->next = NULL;
    if (prev == NULL)
        hashmap->lists[hash_index] = front;
    else
        prev->next = front;
    // update the budget
    budget -= buy_price;
    // update the performance
    performance_count += (k + 1);
    print_item(front);
}

void sell_item(char name[], int quantity)
{
    int hash_index = hashfunc(name, TABLESIZE);
    node *front = hashmap->lists[hash_index];

    int k = 0;
    while (front != NULL)
    {
        k += 1;
        if (strcmp(front->iPtr->name, name) == 0)
        {
            if (front->iPtr->quantity < quantity)
                quantity = front->iPtr->quantity;
            budget += quantity * front->iPtr->saleprice;
            front->iPtr->quantity -= quantity;
            performance_count += k;
            print_item(front);
            return;
        }
        front = front->next;
    }
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

void update_item_price(char name[], int new_price)
{
    int hash_index = hashfunc(name, TABLESIZE);
    node *front = hashmap->lists[hash_index];

    int k = 0;
    while (front != NULL)
    {
        k += 1;
        if (strcmp(front->iPtr->name, name) == 0)
        {
            front->iPtr->saleprice = new_price;
            performance_count += k;
            return;
        }
        front = front->next;
    }
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