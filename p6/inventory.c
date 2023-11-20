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

// global variable
int budget;
hashtable *hashmap;
int performance_count;

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

// to generate hash for hash table
int hashfunc(char *word, int size);

// command function
void buy_comand();
void change_price_command();
void sell_command();

// utilities function
void insert_new_item(char name[], int quantity, int buy_price);
item *create_new_item(char name[], int quantity, int buy_price);
node *create_new_node(item *item_ptr);

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
        else if (strcmp(command, "sell" == 0))
            sell_command;
        else if (strcmp(command, "change_price") == 0)
            change_price_command();
    }
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

    int index = hashfunc(item_str, TABLESIZE);
}

item *create_new_item(char name[], int quantity, int buy_price)
{
    item *new_item = malloc(sizeof(item));
    strcpy(new_item->name, name);
    new_item->quantity = quantity;
    new_item->saleprice = 0;
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
    node *front = hashmap->list[hash_index];
    int k = 0;
    while (front != NULL)
    {
        int curr_name = front->iPtr->name;
        if (strcmp(curr_name, name) == 0)
        {
            // node exisited
            front->iPtr->quantity += quantity;
            return;
        }
        k += 1;
        front = front->next;
    }
    // at this point, we need to create new node,
    item *new_item = create_new_item(name, quantity, buy_price);
    front = create_new_node(new_item);
    front->next = NULL;

    // update the budget
    budget -= buy_price;
}