/*
Van Phat Phan
COP3502 - section 4
Thursday 08/31/2023
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INITSIZE 10
#define MAXLEN 50
#define MAXROWS 100000

// max lenght of an input line LOOKUP (6) + 6*24 for each number = 30
#define MAXSIZEOFINPUTLINE 30

typedef struct order
{
    int s_seat;
    int e_seat;
    char *name;
} order;

typedef struct theaterrow
{
    order **list_orders;
    int max_size;
    int cur_size;
} theaterrow;

order *make_order(int start, int end, char *this_name);
theaterrow *make_empty_row();
int conflict(order *order1, order *order2);
int can_add_order(theaterrow *this_row, order *this_order);
int add_order(theaterrow *this_row, order *this_order);
char *get_owner(theaterrow **theater, int row, int seat_num);
char *get_row_owner(theaterrow *this_row, int seat_num);
int contains(order *myorder, int seat_no);
void free_order(order *this_order);
void free_row(theaterrow *this_row);

int main()
{
    theaterrow **mainTheater = calloc(MAXROWS + 1, sizeof(theaterrow *));
    // input
    char quitStr[] = "QUIT";
    while (1)
    {
        // init variable
        char command[7];
        // get command input in
        scanf("%s", command);

        if (strcmp(command, "QUIT") == 0)
            break;

        if (strcmp(command, "BUY") == 0)
        {
            // init name and row
            char name[50];
            int rowNumber = 0, startNumber = 0, endNumber = 0;
            // get input in
            scanf("%d %d %d %s", &rowNumber, &startNumber, &endNumber, name);

            // init the row
            if (mainTheater[rowNumber] == NULL)
            {
                mainTheater[rowNumber] = make_empty_row();
            }
            order *newOrder = make_order(startNumber, endNumber, name);

            if (add_order(mainTheater[rowNumber], newOrder) == 1)
                printf("SUCCESS\n");
            else
                printf("FAILURE\n");
        }
        else if (strcmp(command, "LOOKUP") == 0)
        {
            int rowNumber, seatNumber;
            // input
            scanf("%d %d", &rowNumber, &seatNumber);
            // input
            if (mainTheater[rowNumber] == NULL)
            {
                printf("No one\n");
                continue;
            }
            char *name = get_row_owner(mainTheater[rowNumber], seatNumber);
            if (name == NULL)
                printf("No one\n");
            else
                printf("%s\n", name);
        }
    }

    // DONE
    // free the memory
    for (int i = 1; i <= MAXROWS; i += 1)
    {
        if (mainTheater[i] != NULL)
            free_row(mainTheater[i]);
    }

    free(mainTheater);

    return 0;
}

order *make_order(int start, int end, char *this_name)
{
    // printf("CALLED INSIDE FUNCTION");
    //  allocate memory or this
    order *newOrder = malloc(sizeof(order));
    // set the field
    newOrder->s_seat = start;
    newOrder->e_seat = end;
    // memory allocating and copy string of name
    newOrder->name = calloc(MAXLEN, sizeof(char));
    strcpy(newOrder->name, this_name);
    return newOrder;
}

theaterrow *make_empty_row()
{
    // init the row
    theaterrow *newRow = malloc(sizeof(theaterrow));
    // set the field
    newRow->cur_size = 0;
    newRow->max_size = INITSIZE;
    // init pointer to pointer, max_size = maximum number of orders
    newRow->list_orders = calloc(newRow->max_size, sizeof(order *));
    return newRow;
}

int conflict(order *order1, order *order2)
{
    int left1 = order1->s_seat;
    int right1 = order2->e_seat;

    int left2 = order2->s_seat;
    int right2 = order2->e_seat;

    // only 2 cases where they can be no conlfict, assert that left <= right
    if (right1 < left2 || right2 < left1)
        return 0;

    return 1;
}

int can_add_order(theaterrow *this_row, order *this_order)
{
    for (int i = 0; i < this_row->cur_size; i += 1)
    {
        if (conflict(this_row->list_orders[i], this_order) == 1)
            return 0;
    }

    return 1;
}

int add_order(theaterrow *this_row, order *this_order)
{
    // there is conflic, failed
    if (can_add_order(this_row, this_order) == 0)
        return 0;

    if (this_row->cur_size == this_row->max_size)
    {
        // double maxsize
        this_row->max_size *= 2;
        // realloc the list of orders
        this_row->list_orders = realloc(this_row->list_orders, this_row->max_size * sizeof(order *));
    }
    // add the order to the list
    this_row->list_orders[this_row->cur_size] = this_order;
    // increment the current size
    this_row->cur_size += 1;
    // return success
    return 1;
}

char *get_owner(theaterrow **theater, int row, int seat_num)
{
    return get_row_owner(theater[row], seat_num);
}

char *get_row_owner(theaterrow *this_row, int seat_num)
{
    order **currListOfOrder = this_row->list_orders;
    int currSize = this_row->cur_size;

    // iterate
    for (int i = 0; i < currSize; i += 1)
    {
        // is contained in the currOrder
        if (contains(currListOfOrder[i], seat_num) == 1)
            return currListOfOrder[i]->name;
    }

    return NULL;
}

int contains(order *myorder, int seat_no)
{
    int currLeft = myorder->s_seat;
    int currRight = myorder->e_seat;
    if (currLeft <= seat_no && seat_no <= currRight)
        return 1;
    return 0;
}

// Frees all memory associated with this_order.
void free_order(order *this_order)
{
    // printf("%s\n", this_order->name);
    //  free the name
    free(this_order->name);
    // free the whole order
    free(this_order);
}

// Frees all memory associated with this_row.
void free_row(theaterrow *this_row)
{
    for (int i = 0; i < this_row->cur_size; i += 1)
    {
        // free the array of order
        // free this order
        // printf("%d\n", i);
        free_order(this_row->list_orders[i]);
    }

    // free the array of order
    free(this_row->list_orders);
    // free the row
    free(this_row);
}