#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100

// Struct for a directory node
typedef struct DirNode {
    char name[MAX_NAME];
    struct DirNode* parent;
    struct DirNode* firstChild;
    struct DirNode* nextSibling;
} DirNode;

// Function declarations
int createDirectory(DirNode* current, const char* name);
int changeDirectory(DirNode** current, const char* name);
int goToParentDirectory(DirNode** current);
int listDirectory(DirNode* current);
int freeDirectory(DirNode* node);

// Helper function declarations
DirNode* findChild(DirNode* current, const char* name);

int main() {
    DirNode* root = (DirNode*)malloc(sizeof(DirNode));
    if (!root) {
        printf("Memory allocation error!\n");
        return -1;
    }
    strcpy(root->name, "root");
    root->parent = NULL;
    root->firstChild = NULL;
    root->nextSibling = NULL;

    DirNode* current = root;
    int choice;
    char name[MAX_NAME];

    while (1) {
        printf("\nCurrent Directory: %s\n", current->name);
        printf("1- md (create directory)\n2- cd dir (change directory)\n3- cd.. (go to parent)\n4- dir (list contents)\n5- exit\nChoice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while(getchar() != '\n'); // clear input buffer
            continue;
        }
        switch (choice) {
            case 1:
                printf("Enter directory name: ");
                scanf("%s", name);
                if (createDirectory(current, name) == -1) {
                    printf("Failed to create directory.\n");
                }
                break;
            case 2:
                printf("Enter directory name to enter: ");
                scanf("%s", name);
                if (changeDirectory(&current, name) == -1) {
                    printf("Directory not found.\n");
                }
                break;
            case 3:
                if (goToParentDirectory(&current) == -1) {
                    printf("Already at root.\n");
                }
                break;
            case 4:
                if (listDirectory(current) == -1) {
                    printf("No subdirectories.\n");
                }
                break;
            case 5:
                freeDirectory(root);
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}

// Function definitions

int createDirectory(DirNode* current, const char* name) {
    if (!current || !name) return -1;
    if (findChild(current, name) != NULL) return -1; // already exists

    DirNode* newDir = (DirNode*)malloc(sizeof(DirNode));
    if (!newDir) return -1;

    strcpy(newDir->name, name);
    newDir->parent = current;
    newDir->firstChild = NULL;
    newDir->nextSibling = NULL;

    if (!current->firstChild) {
        current->firstChild = newDir;
    } else {
        DirNode* temp = current->firstChild;
        while (temp->nextSibling) temp = temp->nextSibling;
        temp->nextSibling = newDir;
    }
    return 0;
}

int changeDirectory(DirNode** current, const char* name) {
    if (!current || !*current || !name) return -1;
    DirNode* child = findChild(*current, name);
    if (!child) return -1;
    *current = child;
    return 0;
}

int goToParentDirectory(DirNode** current) {
    if (!current || !*current || !(*current)->parent) return -1;
    *current = (*current)->parent;
    return 0;
}

int listDirectory(DirNode* current) {
    if (!current) return -1;
    DirNode* temp = current->firstChild;
    if (!temp) return -1;
    printf("Contents:\n");
    while (temp) {
        printf("%s\n", temp->name);
        temp = temp->nextSibling;
    }
    return 0;
}

int freeDirectory(DirNode* node) {
    if (!node) return -1;
    DirNode* child = node->firstChild;
    while (child) {
        DirNode* next = child->nextSibling;
        freeDirectory(child);
        child = next;
    }
    free(node);
    return 0;
}

// Helper function to find child directory by name
DirNode* findChild(DirNode* current, const char* name) {
    if (!current || !name) return NULL;
    DirNode* temp = current->firstChild;
    while (temp) {
        if (strcmp(temp->name, name) == 0) return temp;
        temp = temp->nextSibling;
    }
    return NULL;
}
