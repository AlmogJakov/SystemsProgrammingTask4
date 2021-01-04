/* Default library */
#include <stdio.h>
/* For tolower() method (upper case to lower case) */
#include <ctype.h>
/* For dynamic allocations (malloc, "free" method) */
#include <stdlib.h>
/* For strings methods (strcpy, strcat, strcmp, strlen) */
#include <string.h>
/* define */
#define NUM_LETTERS 26
#define EXIT_FAILURE 1

/* Boolean enum */
typedef enum {FALSE=0, TRUE=1} boolean;

/* Node struct - Represents a vertex in the trie */
typedef struct node {
    char letter;
    int childs;
    long unsigned int count;
    struct node* children[NUM_LETTERS];
} node;

/* The method iterates over the tree nodes starting
from the root and releases the memory of each node */
void freeUpMemory(node *pointer) {
    for (int i = 0; i < NUM_LETTERS; i++) {
            if (pointer->children[i]!=NULL)
                freeUpMemory(pointer->children[i]);
        }
    free(pointer);
}

/* The method build a trie from the input char by char 
In each character entry that represents a alphabet letter,
the method converts the letter to a lower case and then
chains the letter to the appropriate place in the tree
while updating the appropriate fields. */
node* buildTrie() {
    char ch;
    node *root = malloc(sizeof(node));
    node *pointer = root;
    boolean prevIsAlphabet = 0;
    fseek (stdin, 0, SEEK_END);
    int length = ftell (stdin);
    fseek (stdin, 0, SEEK_SET);
    /* if length!=-1 then redirection performed & continue reading stdin until feof(stdin) */
    while (((ch = getchar()) != '\n' && ch != EOF) || (length!=-1 && !feof(stdin))) {
        if ((ch >= 'a'&&ch<= 'z')||(ch>='A'&&ch<='Z')) { //
            ch = tolower(ch);
            prevIsAlphabet = TRUE;
            if (pointer->children[ch-'a']!=NULL) {
                pointer = pointer->children[ch-'a'];
            } else {
                node *newnode = malloc(sizeof(node));
                if (!newnode) {
                    fprintf(stderr, "malloc() failed: insufficient memory!\n");
                    freeUpMemory(root);
                    root = NULL;
                }
                newnode->letter = ch;
                newnode->count = 0;
                newnode->childs = 0;
                pointer->childs++;
                pointer->children[ch-'a'] = newnode;
                pointer = newnode;
            }
        } else {
            if (prevIsAlphabet)
                pointer->count++;
            pointer = root;
            prevIsAlphabet = FALSE;
        }
    }
    if (pointer!=root)
        pointer->count++;
    return root;
}

/* The method prints the words in bottom-up lexicography
order (a->z) and free-up every node memory to avoid
memory leak. If an error occurred during the iteration
the method call freeUpMemory() method to free up memory safely.
arg1 = root pointer to free-up memory in case of failure.
arg2 = iterate pointer starting from the root of the trie.
arg3 = chained string from the root of the trie to current node */
int printBottomUp(node *root, node *pointer, char *str) {
    if (pointer==NULL) return 0;
    if (pointer->count>0) {
        printf("%s\t%lu\n",str,pointer->count);
    } 
    if (pointer->childs>0) {
        for (int i = 0; i < NUM_LETTERS; i++) {
            if (pointer->children[i]!=NULL) {
                char *res = malloc(strlen(str) + 1 + 1);
                if (!res) {
                    fprintf(stderr, "malloc() failed: insufficient memory!\n");
                    freeUpMemory(root);
                    return EXIT_FAILURE;
                }
                strcpy(res, str);
                strcat(res, &(pointer->children[i]->letter));
                printBottomUp(root, pointer->children[i],res);
                free(res);
            }
        }
    }
    free(pointer);
    return 0;
}

/* The method prints the words in top-down lexicography
order (z->a) and free-up every node memory to avoid
memory leak. If an error occurred during the iteration
the method call freeUpMemory() method to free up memory safely.
arg1 = root pointer to free-up memory in case of failure 
arg2 = iterate pointer starting from the root of the trie
arg3 = chained string from the root of the trie to current node */
int printTopDown(node *root, node *pointer, char *str) {
    if (pointer==NULL) return 0;
    if (pointer->childs>0) {
        for (int i = NUM_LETTERS-1; i >= 0; i--) {
            if (pointer->children[i]!=NULL) {
                char *res = malloc(strlen(str) + 1 + 1);
                if (!res) {
                    fprintf(stderr, "malloc() failed: insufficient memory!\n");
                    freeUpMemory(root);
                    return EXIT_FAILURE;
                }
                strcpy(res, str);
                strcat(res, &(pointer->children[i]->letter));
                printTopDown(root, pointer->children[i],res);
                free(res);
            }
        }
    }
    if (pointer->count>0) {
        printf("%s\t%lu\n",str,pointer->count);
    } 
    free(pointer);
    return 0;
}

//////////////////* main method *//////////////////
/* The method checks the arguments in the input.
If there is no argument the method will construct the
tree according to the input text and then print the words
frequency in bottom-up lexicography order.
Otherwise, if there is another argument 'r' the method will
construct the tree according to the input string and then
print the word frequency in top-down lexicography order */
int main(int argc, char *argv[]) {
if (argc < 2) {
    node *root = buildTrie();
    if (root!=NULL&&root->childs!=0) {
        char *str = "";
        printBottomUp(root, root, str);
        }
} else if (argc == 2 && strcmp(argv[1], "r") == 0) {
    node *root = buildTrie();
    if (root!=NULL&&root->childs!=0) {
        char *str = "";
        printTopDown(root, root, str);
        }
    }
}