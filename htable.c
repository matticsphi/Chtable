/*
 * htable.c
 *
 *  Created on: Apr 20, 2011
 *      Author: Mattics Phi
 *
 *      http://geeksforgeeks.org/?p=7740 (Code for mergesort linked list)
 *      http://cslibrary.stanford.edu/110/BinaryTrees.html
 */

#include "header.h"

#define TABLESIZE 255

FILE *openFile(int argc, char *argv[]);
void addToTable(char c);
struct node *createNode(char c);
void otherMethods();
void sortTable();
struct node *makeLinkedList();
struct node *buildTree(struct node *head);
struct node *super(struct node *head);
void reBuildTable();
void printTable(struct node *head);
struct node *searchNode(char c, struct node *head);
void printPath(struct node *nodeFound);

struct node {
    int count, path;
    char letter;
    struct node *left, *right, *parent, *next;
};

static struct node **table;

/*
 * Main
 */
int main(int argc, char *argv[])
{
    FILE *fp;
    int i;
    char c;
   
    /* Initializing the table to NULL */
    table = malloc(sizeof(*table) * (TABLESIZE + 1));
    for (i = 0; i < TABLESIZE; i++) {
        table[i] = NULL;
    }

    /* Open the file */
    fp = openFile(argc, argv);
   
    /* Scan in characters until end of file */
    while ((c = fgetc(fp)) != EOF) {
        addToTable(c);
    }
   
    /* Other methods */
    otherMethods();

    return 0;
}

/*
 * Opening file to read
 */
FILE *openFile(int argc, char *argv[])
{
    FILE *fp;
   
    if (argc == 2) {
        fp = fopen(argv[1], "r");
    } else if (argc != 2) {
        fprintf(stderr, "Input one file.\n");
        return 0;
    }
   
    if (fp == NULL) {
        perror(argv[1]);
        exit(0);
    }
   
    return fp;
}

/*
 * Add character to table
 */
void addToTable(char c)
{
    if ((table[(int)c]) == NULL ) {
        table[(int)c] = createNode(c);
    } else {
        table[(int)c]->count++;
    }
}

/*
 * Creates a new node to add to table
 */
struct node *createNode(char c)
{
    struct node *newNode = malloc(sizeof(struct node *));

    newNode->count = 1;
    newNode->letter = c;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->next = NULL;

    return newNode;
}

/*
 * Other main function
 */
void otherMethods()
{
    struct node *head;
   
    sortTable();
    head = makeLinkedList();
    head = buildTree(head);
    reBuildTable();
    printTable(head);
}

/*
 * Sorts the table based on number of occurances
 */
void sortTable()
{
    int counter = 0, i, j;
    struct node *temp;

    for (i = 0; i < TABLESIZE; i++) {
        if (table[i] != NULL) {
            table[counter] = table[i];
            table[i] = NULL;
            counter++;
        }
    }

    for (i = 0; i < TABLESIZE && table[i] != NULL; i++) {
        for (j = 0; j < TABLESIZE - 1 && table[j+1] != NULL; j++) {
            if (table[j]->count > table[j+1]->count) {
                temp = table[j+1];
                table[j+1] = table[j];
                table[j] = temp;
            }
        }
    }
}

/*
 * Makes a linked list from the table
 */
struct node *makeLinkedList()
{
    int i = 0;

    while (table[i] != NULL) {
        if (table[i + 1] != NULL) {
            table[i]->next = table[i + 1];
        }
        i++;
    }

    return table[0];
}

/*
 * Builds a tree from the linked list
 */
struct node *buildTree(struct node *head)
{
    while (head->next != NULL) {
        head = super(head);
    }
   
    return head;
}

/*
 * Combines the first two nodes and puts it back in the list
 */
struct node *super(struct node *head)
{
    struct node *left, *right, *temp, *superNode, *newHead;
   
    left = head;
    right = left->next;
   
    if (right == NULL) {
        return left;
    } else {
        temp = right->next;
        superNode = malloc(sizeof(* superNode));
        superNode->parent = NULL;
        superNode->letter = -1;
        superNode->left = left;
        superNode->right = right;
        superNode->count = left->count + right->count;
      
        left->parent = superNode;
        left->next = NULL;
        left->path = 0;
        right->parent = superNode;
        right->next = NULL;
        right->path = 1;
      
        if (temp == NULL) {
            return superNode;
        }
      
        /* Put the supernode in front */
        if (superNode->count <= temp->count) {
            superNode->next = temp;
            return superNode;
        }
      
        newHead = temp;
      
        /* Find where supernode belongs and places it in there */
        for (; temp != NULL; temp = temp->next) {
            
            /* Puts it at the end of the list */
            if (temp->next == NULL) {
                temp->next = superNode;
                superNode->next = NULL;
                return newHead;
                
            /* Puts it between nodes */
            } else if (temp->count < superNode->count && 
                       superNode->count <= temp->next->count) {
                superNode->next = temp->next;
                temp->next = superNode;
                return newHead;
            }
        }
    }
    return NULL;
}

/*
 * Sorts the table by ascii value
 */
void reBuildTable()
{
    int i, j;
    struct node *temp;
   
    for (i = 0; i < TABLESIZE && table[i] != NULL; i++) {
        for (j = 0; j < TABLESIZE - 1 && table[j+1] != NULL; j++) {
            if (table[j]->letter > table[j+1]->letter) {
                temp = table[j+1];
                table[j+1] = table[j];
                table[j] = temp;
            }
        }
    }
}

/*
 * Prints the table based on ascii value
 */
void printTable(struct node *head)
{
    int i = 0;
    struct node *nodeFound;
   
    while (table[i] != NULL) {
        if (table[i]->letter < 16) {
            printf("0x0%x: ", table[i]->letter);
        } else {
            printf("0x%x: ", table[i]->letter);
        } 
        nodeFound = searchNode(table[i]->letter, head);
        printPath(nodeFound);
        i++;
    }
}

/*
 * Searches the tree for node to be printed by using recursion
 */
struct node *searchNode(char c, struct node *head)
{
    struct node *left, *right;
   
    if (head == NULL) {
        return NULL;
    }
   
    if (head->letter == c) {
        return head;
    }
   
    left = searchNode(c, head->left);
    right = searchNode(c, head->right);
   
    if (left != NULL) {
        if (left->letter == c) {
            return left;
        }
    } else if (right != NULL) {
        if (right->letter == c) {
            return right;
        }
    }
   
    return NULL;
}

/*
 * Finds the path to the node
 */
void printPath(struct node *node)
{
    int array[100], i = 0;
   
    for (; node->parent != NULL; node = node->parent) {
       array[i] = node->path;
       i++;
    }
   
    for (i--; i >= 0; i--) {
       printf("%d", array[i]);
    }
    printf("\n");
}