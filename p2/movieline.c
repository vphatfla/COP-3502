/*
Van Phat Phan
COP 3502
09/19/23*/

#include <stdio.h>
#include <stdlib.h>

#define SIZEOFLISTQUEUE 12
#define EMPTY -1
#define DEFAULTPROCESSTIME 30
#define PROCESSTIMEPERTICKET 5

typedef struct Customer
{
    char *name;
    int numberOfTickets;
    int arrivalTIme;
    int outTime;
} Customer;

typedef struct Node
{
    Customer *customer;
    struct Node *next;
} Node;

typedef struct Queue
{
    Node *front;
    Node *end;
    int currSize;
} Queue;

// create new customer
Customer *createNewCustomer(char name[], int numberOfTicket, int arrivalTime);
// make empty queue
Queue *createEmptyQueue();
// add customer to queue
void addCustomerToQueue(Customer *thisCutomer, Queue **listQueue);
// free node
void freeNodePointer(Node *node);
// free customer
void freeCustomerPointer(Customer *customer);
// free queue
void freeQueuePointer(Queue *queue);
// return the list non-empty queue with least number of people
Queue *minQueue(Queue **listQueue);
// print all linkedlist
void printListQueue(Queue **listQueue);
// return number of non-zero queue
int numbersOfNonZeroQueue(Queue **listQueue);
// process the booth (queueAr)
void processBooth(Queue **listQueue, int queueAr[], int arSize);
// queue linkedlist implementation
int enqueue(struct Queue *queue, Customer *customer);
Node *dequeue(struct Queue *queue);
int empty(struct Queue *queue);
int front(struct Queue *queue);

int main()
{
    // init the list of queue
    Queue **listQueue = calloc(SIZEOFLISTQUEUE, sizeof(Queue *));
    for (int i = 0; i < 12; i += 1)
    {
        listQueue[i] = createEmptyQueue();
    }

    // declare the number of customer
    // number of booth
    int n, b;
    scanf("%i", &n);
    scanf("%i", &b);

    // init the booth
    int boothList[b];
    for (int i = 0; i < n; i += 1)
    {
        char *name = malloc(51 * sizeof(char));
        int nt;
        int arrivalTime;
        scanf("%s %i %i", name, &nt, &arrivalTime);
        // create new customer
        Customer *thisNewCustomer = createNewCustomer(name, nt, arrivalTime);
        // enqueue the customer
        addCustomerToQueue(thisNewCustomer, listQueue);
    }

    // print out the queue
    // printListQueue(listQueue);

    // number of nonezeroquee
    int nonZeroQueue = numbersOfNonZeroQueue(listQueue);
    // min qeueu per booth
    int minQueuePerBooth = nonZeroQueue / b;
    // number of booth to get extra
    int numberExtraBooth = nonZeroQueue % b;
    // process the customer from queue to booth
    int queueIndex = 0;
    // booth number
    int boothNumber = 1;
    while (queueIndex < SIZEOFLISTQUEUE)
    {
        if (listQueue[queueIndex]->currSize == 0)
        {
            queueIndex += 1;
            continue;
        }
        // int array contain the queue index in the current booth
        int arSize = minQueuePerBooth;
        if (numberExtraBooth > 0)
        {
            arSize += 1;
            numberExtraBooth -= 1;
        }
        int queueAr[arSize];
        // add the queue to the ar
        for (int i = 0; i < arSize; i += 1)
        {
            while (queueIndex < SIZEOFLISTQUEUE && listQueue[queueIndex]->currSize == 0)
                queueIndex += 1;
            queueAr[i] = queueIndex;
            queueIndex += 1;
        }

        printf("Booth %i\n", boothNumber);
        boothNumber += 1;
        // process the booth
        processBooth(listQueue, queueAr, arSize);
        printf("\n");
    }

    // free memmory
    for (int i = 0; i < SIZEOFLISTQUEUE; i += 1)
    {
        Node *tempNode = listQueue[i]->front;
        while (tempNode != NULL)
        {
            Node *nodeToFree = tempNode;
            tempNode = tempNode->next;

            // free the customer in the node
            freeCustomerPointer(nodeToFree->customer);
            // free the node
            freeNodePointer(nodeToFree);
        }
        // free the queue pointer
        freeQueuePointer(listQueue[i]);
    }
    // free the list of queue
    free(listQueue);
    return 0;
}

Queue *createEmptyQueue()
{
    Queue *newQueue = malloc(sizeof(Queue));
    newQueue->front = NULL;
    newQueue->end = NULL;
    newQueue->currSize = 0;
    return newQueue;
}

Customer *createNewCustomer(char *name, int numberOfTicket, int arrivalTime)
{
    Customer *newCustomer = malloc(sizeof(Customer));
    newCustomer->name = name;
    newCustomer->arrivalTIme = arrivalTime;
    newCustomer->numberOfTickets = numberOfTicket;
}

void addCustomerToQueue(Customer *thisCutomer, Queue **listQueue)
{
    int queueIndex = thisCutomer->name[0] % 13;

    // queueIndex-1 because listQueue index range from 0 <->11 not 12
    if (queueIndex != 0)
    {
        enqueue(listQueue[queueIndex - 1], thisCutomer);
    }
    else
        enqueue(minQueue(listQueue), thisCutomer);
}

Queue *minQueue(Queue **listQueue)
{
    Queue *tempQueue;
    int minSize = -1;
    for (int i = 0; i < 12; i += 1)
    {
        if (listQueue[i]->currSize == 0)
            continue;
        if (minSize == -1 || listQueue[i]->currSize < minSize)
        {
            minSize = listQueue[i]->currSize;
            tempQueue = listQueue[i];
        }
    }
    if (minSize == -1)
        return listQueue[0];

    return tempQueue;
}

void printListQueue(Queue **listQueue)
{
    for (int i = 0; i < SIZEOFLISTQUEUE; i += 1)
    {
        printf("\nQueue number %i\n", i + 1);
        Node *tempNode = listQueue[i]->front;
        while (tempNode != NULL)
        {
            printf("Name = %s      ", tempNode->customer->name);
            tempNode = tempNode->next;
        }
    }
}

int numbersOfNonZeroQueue(Queue **listQueue)
{
    int count = 0;
    for (int i = 0; i < SIZEOFLISTQUEUE; i += 1)
    {
        if (listQueue[i]->currSize > 0)
            count += 1;
    }

    return count;
}

void processBooth(Queue **listQueue, int queueAr[], int arSize)
{
    int currentTime = 0;
    int numberOfNoneEmptyQueue = arSize;
    while (numberOfNoneEmptyQueue != 0)
    {
        Queue *queueWithMinArrivalTime = NULL;
        int indexOfQueueWithMinArrivalTime = 0;
        for (int i = 0; i < arSize; i += 1)
        {
            if (listQueue[queueAr[i]]->currSize == 0)
                continue;
            if (queueWithMinArrivalTime == NULL)
            {
                queueWithMinArrivalTime = listQueue[queueAr[i]];
                indexOfQueueWithMinArrivalTime = queueAr[i];
            }
            else if (queueWithMinArrivalTime->front->customer->arrivalTIme > listQueue[queueAr[i]]->front->customer->arrivalTIme)
            {
                queueWithMinArrivalTime = listQueue[queueAr[i]];
                indexOfQueueWithMinArrivalTime = queueAr[i];
            }
        }

        Node *processingNode = dequeue(queueWithMinArrivalTime);
        int startTime = currentTime;
        // if customer came later than the start time, the start time = arrival time
        if (processingNode->customer->arrivalTIme > startTime)
            startTime = processingNode->customer->arrivalTIme;
        currentTime = (startTime + DEFAULTPROCESSTIME + processingNode->customer->numberOfTickets * PROCESSTIMEPERTICKET);

        // print the information of the customer
        printf("%s from line %i checks out at time %i.\n", processingNode->customer->name, indexOfQueueWithMinArrivalTime + 1, currentTime);
        // decrease number of nonEmtpy queue
        if (empty(queueWithMinArrivalTime))
            numberOfNoneEmptyQueue -= 1;

        // free the memory of the node and its customer
        freeCustomerPointer(processingNode->customer);
        freeNodePointer(processingNode);
    }
}
// free memory function
// free node
void freeNodePointer(Node *node)
{
    free(node);
}
// free customer
void freeCustomerPointer(Customer *customer)
{
    free(customer);
}
// free queue
void freeQueuePointer(Queue *queue)
{
    free(queue);
}
//-----------------------------------------------
// queue linkedlist implementation

// Pre-condition: qPtr points to a valid struct queue and val is the value to
//                enqueue into the queue pointed to by qPtr.
// Post-condition: If the operation is successful, 1 will be returned, otherwise
//                 no change will be made to the queue and 0 will be returned.
int enqueue(struct Queue *queue, Customer *customer)
{

    struct Node *tempNode;

    // Allocate space for a new node to add into the queue.
    tempNode = malloc(sizeof(Node));

    // This case checks to make sure our space got allocated.
    if (tempNode != NULL)
    {

        // Set up our node to enqueue into the back of the queue.
        tempNode->customer = customer;
        tempNode->next = NULL;
        // If the queue is NOT empty, we must set the old "last" node to point
        // to this newly created node.
        if (queue->end != NULL)
            queue->end->next = tempNode;

        // Now, we must reset the back of the queue to our newly created node.
        queue->end = tempNode;

        // If the queue was previously empty we must ALSO set the front of the
        // queue.
        if (queue->front == NULL)
            queue->front = tempNode;

        // update the size
        queue->currSize += 1;

        // Signifies a successful operation.
        return 1;
    }

    // No change to the queue was made because we couldn't find space for our
    // new enqueue.
    else
        return 0;
}

// remove the node from the list
// and return the node that contains the customer
Node *dequeue(struct Queue *queue)
{

    Node *tempNode;

    // Check the empty case.
    if (queue->front == NULL)
        return NULL;

    // Set up a temporary pointer to use return
    tempNode = queue->front;

    // Make front point to the next node in the queue.
    queue->front = queue->front->next;

    // If deleting this node makes the queue empty, we have to change the back
    // pointer also!
    if (queue->front == NULL)
        queue->end = NULL;

    // update the size
    queue->currSize -= 1;
    // Return the value that just got dequeued.
    return tempNode;
}

// Pre-condition: qPtr points to a valid struct queue.
// Post-condition: returns true iff the queue pointed to by pPtr is empty.
int empty(struct Queue *queue)
{
    return queue->front == NULL;
}
