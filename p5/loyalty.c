/*
Van Phat Phan
COP-3502 Section 4
11/03/2023*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 19

// global variables, N = number of commands, num_customer = total number of customers;
int N, num_customer;

typedef struct customer {
 char name[MAXLEN+1];
 int points;
} customer;

typedef struct treenode {
 customer* cPtr;
 int size;  // store total number of nodes in the subtree rooted at that node, including itself
 struct treenode* left;
 struct treenode* right;
} treenode;

// new customer
customer* create_new_customer(char name[], int points);

// node implementation
treenode* create_new_node(customer* customer);

// Traversals.
void preorder(struct treenode *current_ptr);
void inorder(struct treenode *current_ptr);

// Search and insert.
treenode* findNode(struct treenode *current_ptr, char name[]);
treenode* insert(treenode *root, treenode *element);

// Structural functions to aid delete.
int isLeaf(struct treenode *node);
int hasOnlyLeftChild(struct treenode *node);
int hasOnlyRightChild(struct treenode *node);


// Other functions to aid delete.
struct treenode* parent(struct treenode *root, struct treenode *node);
struct treenode* minVal(struct treenode *root);
struct treenode* maxVal(struct treenode *root);

// Delete functions.
struct treenode* delete(struct treenode* root, char name[]);

// ultilities functions
int compare_string(char name1[], char name2[]);
void free_memory_node(treenode *node);
void free_memory_BST(treenode* root);

// command functions 
treenode* add_command(treenode* root);
treenode* sub_command(treenode* root);
void search_command(treenode* root);
void del_command(treenode* root);
void count_smaller_command(treenode* root);

int main() {
    treenode* root = NULL;
    
    // init num_customer
    num_customer = 0;
    scanf("%d", &N);
    for (int i = 0; i< N; i+=1) {
        char command[15]; // command max length is 15, count_smaller isn't even 15, safety upper bound
        scanf("%s", command);
        
        if (strcmp(command, "add") == 0) {
            printf("Add command \n");
            root = add_command(root);
        }
        else if (strcmp(command, "sub") == 0) {
            printf("Sub command \n");
            root = sub_command(root);
        }
        else if (strcmp(command, "del")) {
            printf("del command \n");
        }
        else if (strcmp(command, "search") == 0) {
            printf("search command \n");
        }
        else if (strcmp(command, "count_smaller") == 0) {
            printf("count_smaller command \n");
        }
    }


    preorder(root);


    // memory free
    free_memory_BST(root);
    root = NULL;
    if (root == NULL) 
        printf("Free memory successfully\n");
    return 0;
}

customer* create_new_customer(char name[], int points) {
    customer* new_customer = malloc(sizeof(customer));
    strcpy(new_customer->name, name);
    new_customer->points = points;
    return new_customer;
}

treenode* create_new_node(customer* customer) {
    treenode* new_treenode = malloc(sizeof(treenode));
    new_treenode->cPtr = customer;
    new_treenode->size = 0;
    new_treenode->left = NULL;
    new_treenode->right = NULL;
    return new_treenode;
}

int compare_string(char name1[], char name2[]) {
    return strcmp(name1, name2);
}

void free_memory_node(treenode *node) {
    free(node->cPtr);
    free(node);
}

void free_memory_BST(treenode* root) {
    if (root == NULL) 
        return;
    
    if (root->left != NULL)
        free_memory_BST(root->left);
    if (root->right != NULL)
        free_memory_BST(root->right);
    
    root->left = NULL;
    root->right = NULL;
    free_memory_node(root);
    root = NULL;
}
// Inserts a the node pointed to by element in the tree rooted by
// root and returns a pointer to the resulting tree.
struct treenode* insert(treenode* root,
                         treenode* element) {

    // Inserting into an empty tree.
    if (root == NULL)
        {
            return element; 
        }


    // Regular case.
    else {
        int compare_result = compare_string(element->cPtr->name, root->cPtr->name);
        
        // customer existed, update the points
        // no size update
        if (compare_result == 0) {
            root->cPtr->points += element->cPtr->points;
            free_memory_node(element);
        }
        // element name is greater than root
        // element should be inserted to the right.
        else if (compare_result > 0) {
            // increase the size of root first
            root->size += 1;
            // There is a right subtree to insert the node.
            if (root->right != NULL)
                root->right = insert(root->right, element);

            // Place the node directly to the right of root.
            else
                root->right = element;
        }

        // element should be inserted to the left.
        else {
            // increase the size of root first
            root->size += 1;
            // There is a left subtree to insert the node.
            if (root->left != NULL)
                root->left = insert(root->left, element);

            // Place the node directly to the left of root.
            else
                root->left = element;
        }

        // Return the root pointer of the updated tree.
        return root;
    }
}

// Returns a pointer to a node that stores value in it in the subtree
// pointed to by current_ptr. NULL is returned if no such node is found.
struct treenode* findNode(struct treenode *current_ptr, char name[]) {

    // Check if there are nodes in the tree.
    if (current_ptr != NULL) {
        int compare_result = compare_string(name, current_ptr->cPtr->name);
        // Found the value at the root.
        if (compare_result == 0)
            return current_ptr;

        // Search to the left.
        if (compare_result < 0)
            return findNode(current_ptr->left, name);

        // Or...search to the right.
        else
            return findNode(current_ptr->right, name);
    }

    // Null case.
    else
        return NULL;
}

// Structural functions to aid delete.
int isLeaf(struct treenode *node) {
    return (node->left == NULL && node->right == NULL);
}
int hasOnlyLeftChild(struct treenode *node) {
    return (node->left != NULL && node->right == NULL);
}
int hasOnlyRightChild(struct treenode *node) {
    return (node->left == NULL && node->right != NULL);
}

// returns the parent of the node pointed to by node in the tree
//rooted at root, if the node is the root of the tree, 
// or node doesn't exist in the tree, null will be returned
struct treenode* parent(struct treenode *root, struct treenode *node) {
    
    if (root ==NULL || root == node) 
        return NULL;
    
    // the root is the direct parent of node
    if (root->left == node || root->right == node)
        return root;
    

    
    int compare_result = compare_string(node->cPtr->name, root->cPtr->name);
    // look for node's parent in the right side of the tree
    if (compare_result > 0) 
        return parent(root->right, node);
    else if (compare_result < 0)
        return parent(root->left, node);
    
    return NULL;
}

struct treenode* minVal(struct treenode *root) {
    if (root->left == NULL)
        return root;
    
    else return minVal(root->left);
}
struct treenode* maxVal(struct treenode *root)
{
    if (root->right == NULL)
        return root;
    
    else return maxVal(root->right);
}

// Delete functions.
struct treenode* delete(struct treenode* root, char name[]) {

    treenode *delNode, *new_del_node, *save_node;
    treenode *parentNode;
    
    delNode = findNode(root, name);
    parentNode = parent(root, delNode);

    if (isLeaf(delNode)) {
        // delete the only node in the tree
        if (parentNode == NULL) {
            free_memory_node(root);
            return NULL;
        }

        // delete if it is a left child
        if (parentNode->left == delNode) {
            free_memory_node(delNode);
            parentNode->left = NULL;
        }
        else {
            free_memory_node(delNode);
            parentNode->right = NULL;
        }

        return root;
    }

    // only has left
    if (hasOnlyLeftChild(delNode)) {

        if (parentNode == NULL) {  // delnode is the root
            save_node = delNode->left;
            free_memory_node(delNode);
            return save_node;  // new root
        }

        // delete a node if it's a left child
        if (delNode == parentNode->left) {
            save_node = parentNode->left;
            parentNode->left = parentNode->left->left; // remove the left from the BST
            free_memory_node(delNode);
        }
        else { // delete node if it's a right child
            save_node = parentNode->right;
            parentNode->right = parentNode->right->right; // remove the right from the BST
            free_memory_node(delNode);
        }
        return root;    
    }

    // only has right child
    if (hasOnlyRightChild(delNode)) {
        if (parentNode == NULL) { // delnode is a root
            save_node = delNode->right;
            free_memory_node(delNode);
            return save_node;
        }
        
        // if delnode is a left child of parentnode
        if (parentNode->left == delNode) {
            parentNode->left = parentNode->left->right;
            free_memory_node(delNode);
        }
        else { // delnot is a right child of parentnode
            parentNode->right = parentNode->right->right;
            free_memory_node(delNode);
        }
        return root;
    }

    // if none of these aboves are the case, then "switch the place of delnode customer with the maxVal"
    // find the greatest on the left
    new_del_node = maxVal(delNode->left);
    customer* new_customer_pointer = new_del_node->cPtr;
    
    delete(root, new_del_node->cPtr->name); // recursively delete the node with the given name, kinda sketchy at this point

    // restore the save data
    delNode->cPtr = new_customer_pointer;

    return root;
}


// command functions 
treenode* add_command(treenode* root) {
    char name[MAXLEN];
    int points;
    scanf("%s", name);
    scanf("%d",  &points);
    // check if existed
    treenode* existed_node = findNode(root, name);
    
    if (existed_node != NULL) {
        existed_node->cPtr->points +=points;
        printf("%s %d\n", existed_node->cPtr->name, existed_node->cPtr->points);
        return root;
    }
    customer* new_customer = create_new_customer(name, points);
    treenode* new_treenode = create_new_node(new_customer);

    new_treenode->size = 1;

    printf("%s %d\n", new_treenode->cPtr->name, new_treenode->cPtr->points);
    return insert(root, new_treenode);
}
treenode* sub_command(treenode* root) {
    char name[MAXLEN];
    int points;
    scanf("%s", name);
    scanf("%d", &points);

    treenode* existed_node = findNode(root, name);

    if (existed_node != NULL) {
        existed_node->cPtr->points -= points;
        if (existed_node->cPtr->points <0)
            existed_node->cPtr->points = 0;
        printf("%s %d\n", existed_node->cPtr->name, existed_node->cPtr->points);
    }
    
    printf("%s not found\n", existed_node->cPtr->name);
    return root;
}

void search_command(treenode* root) {
    char name[MAXLEN];
    scanf("%s", name);

    treenode* existed_node = findNode(root, name);

    if (existed_node != NULL) {
        printf("%s %d %d\n", existed_node->cPtr->name, existed_node->cPtr->points, existed_node->size);
    }
}
void del_command(treenode* root) {

}
void count_smaller_command(treenode* root) {

}


// Traversals.
void preorder(struct treenode *current_ptr) {
    if (current_ptr != NULL)
    {
        printf("%s %d w = %d -- \n", current_ptr->cPtr->name, current_ptr->cPtr->points, current_ptr->size);
        preorder(current_ptr->left);
        preorder(current_ptr->right);
    }
    else {
        printf("NULL --\n");
    }

}
void inorder(struct treenode *current_ptr) {
    if (current_ptr != NULL) {
        inorder(current_ptr->left);
        printf("%s %d --", current_ptr->cPtr->name, current_ptr->cPtr->points);
        inorder(current_ptr->right);
    }
    else 
        printf("NULL -- \n");
}   