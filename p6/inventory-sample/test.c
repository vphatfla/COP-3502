// Eduardo Salcedo Fuentes
// inventory.c P6
// 11/29/2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// required constants
#define MAXLEN 19
#define TABLESIZE 300007

// item struct with name, quantity, and price
typedef struct item
{
    char name[MAXLEN + 1];
    int quantity;
    int saleprice;
} item;

// node struct with pointer to item and another node
typedef struct node
{
    item *iPtr;
    struct node *next;
} node;

// hash table struct with array of node pointers and size
typedef struct hashtable
{
    node **lists;
    int size;
} hashtable;

// testing functions
int test(hashtable *h, char *name);
void print(node *front);
void display_hashmap(hashtable *hashmap);
// given hash function
int hashfunc(char *word, int size);

// functions to find or create nodes
node *insertNode(node *front, char *name);
node *search(char *name, node *front, hashtable *h, int hash);
node *findNode(char *name, hashtable *h);

// item adjusting functions
void add(node *node, int amo);
void sub(node *node, int amo);
void changePrice(node *node, int newPrice);

// memory freeing functions based on Prof. Guha
void delTable(hashtable *h);
void delList(node *p);

// global vars for time complexity and total cash
int t = 0, totalCash = 100000;

int main(void)
{
    // initialize vars for input and calculations
    int n, amount, cost, newPrice, quantity;
    char input[13], item[MAXLEN + 1];

    // node pointer to item we're adjusting
    node *hold;

    // create table and allocate necessary memory
    hashtable *ht = malloc(sizeof(hashtable));
    ht->size = TABLESIZE;
    ht->lists = malloc(sizeof(node *) * TABLESIZE);
    // set each node pointer to NULL
    for (int i = 0; i < TABLESIZE; i++)
    {
        ht->lists[i] = NULL;
    }

    // take command # input
    scanf("%d", &n);

    // loop through amount of commands
    for (int i = 0; i < n; i++)
    {
        // command word
        scanf("%s", input);

        // buy command
        if (strcmp(input, "buy") == 0)
        {
            // take in remaining input
            scanf("%s %d %d", item, &amount, &cost);
            // store pointer to node that holds current item
            hold = findNode(item, ht);
            // add quantity to item count
            add(hold, amount);
            // adjust total cash
            totalCash -= cost;
            // print item name, quantity, and total cash
            printf("%s %d %d\n", item, hold->iPtr->quantity, totalCash);
        }

        // sell command
        if (strcmp(input, "sell") == 0)
        {
            // take in remaining input
            scanf("%s %d", item, &amount);
            // store pointer to node that holds current item
            hold = findNode(item, ht);
            // remove quantity of item and adjust cash
            sub(hold, amount);
            // print item name, quantity, and total cash
            printf("%s %d %d\n", item, hold->iPtr->quantity, totalCash);
        }

        // change_price command
        if (strcmp(input, "change_price") == 0)
        {
            // take in remaining input
            scanf("%s %d", item, &newPrice);
            // store pointer to node that holds current item
            hold = findNode(item, ht);
            // change the salesPrice of the item
            changePrice(hold, newPrice);
        }
    }

    // print total cash and time complexity
    printf("%d\n%d\n", totalCash, t);
    display_hashmap(ht);
    // free memory
    delTable(ht);
    return 0;
}

// function based on Guha, frees table
void delTable(hashtable *h)
{
    // delete nodes and their items
    for (int i = 0; i < TABLESIZE; i++)
    {
        delList(h->lists[i]);
    }
    // deletes array and table space
    free(h->lists);
    free(h);
}

// function based on Guha, frees linked list memory
void delList(node *p)
{
    node *temp;
    // recursively free each node and its item pointer in the list
    if (p != NULL)
    {
        temp = p->next;
        delList(temp);
        free(p->iPtr);
        free(p);
    }
}

// test function from Guha that prints linked list in order
void print(node *front)
{
    while (front != NULL)
    {
        printf("%s\n", front->iPtr->name);
        front = front->next;
    }
}

// test function that returns first-item-in-list's quantity
int test(hashtable *h, char *name)
{
    return h->lists[hashfunc(name, TABLESIZE)]->iPtr->quantity;
}

// hash function that determines hash values and index of item
int hashfunc(char *word, int size)
{
    int len = strlen(word);
    int res = 0;
    for (int i = 0; i < len; i++)
        res = (1151 * res + (word[i] - 'a')) % size;
    return res;
}

// used to find desired node, creates it otherwise and updates hashtable
node *findNode(char *name, hashtable *h)
{
    // finds hash val
    int hash = hashfunc(name, TABLESIZE);
    // returns node needed
    return search(name, h->lists[hash], h, hash);
}

// recursively looks for item, creates item node if not found
node *search(char *name, node *front, hashtable *h, int hash)
{
    // case to create node
    if (front == NULL)
    {
        // updates front of list to new created node
        h->lists[hash] = insertNode(h->lists[hash], name);
        // returns node
        return h->lists[hash];
    }
    // updates time complexity each time we traverse list
    t++;
    printf("\n t = %d\n", t);
    //  returns current node if item name matches
    if (!strcmp(name, front->iPtr->name))
        return front;
    // recursive traverses list
    return search(name, front->next, h, hash);
}

void display_hashmap(hashtable *hashmap)
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
// creates a new node
node *insertNode(node *front, char *name)
{
    // allocates node memory
    node *tmp = malloc(sizeof(node));
    // sets next to old front
    tmp->next = front;
    // allocates item memory and sets data to 0
    tmp->iPtr = malloc(sizeof(item));
    tmp->iPtr->quantity = 0;
    tmp->iPtr->saleprice = 0;
    // copies name
    strcpy(tmp->iPtr->name, name);
    // adds 1 to time complexity for making new node
    // t++;
    printf("\nt = %d\n", t);
    //  returns new front/node
    return tmp;
}

// adds amount to item quantity
void add(node *node, int amo)
{
    node->iPtr->quantity += amo;
}

// subtracts amount from item quantity and adjusts total cash
void sub(node *node, int amo)
{
    // sets quantity to 0 if there are not enough, adjusts cash only by how much is actually sold
    if (node->iPtr->quantity < amo)
    {
        totalCash += node->iPtr->quantity * node->iPtr->saleprice;
        node->iPtr->quantity = 0;
    }
    else
    {
        // otherwise, remove amount from quantity and adjusts cash by entire amount
        node->iPtr->quantity -= amo;
        totalCash += amo * node->iPtr->saleprice;
    }
}

// changes price to new price set
void changePrice(node *node, int newPrice)
{
    node->iPtr->saleprice = newPrice;
}