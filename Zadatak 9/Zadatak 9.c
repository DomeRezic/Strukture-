#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// Function declarations
Node* insert(Node* root, int value, int* err);
int replace(Node* root, int* err);
int inorder_to_file(Node* root, FILE* file, int* err);
int free_tree(Node* root, int* err);

int main() {
    Node* root = NULL;
    int err = 0;
    FILE* file;

    srand(time(NULL));

    // Create tree with 10 random numbers in range <10,90>
    for (int i = 0; i < 10; i++) {
        int num = rand() % 81 + 10;  // 10..90
        root = insert(root, num, &err);
        if (err == -1) {
            printf("Error inserting element %d\n", num);
            free_tree(root, &err);
            return -1;
        }
    }

    // Write initial tree to file
    file = fopen("tree_before.txt", "w");
    if (!file) {
        printf("Error opening file for writing\n");
        free_tree(root, &err);
        return -1;
    }
    if (inorder_to_file(root, file, &err) == -1) {
        printf("Error writing tree to file\n");
        fclose(file);
        free_tree(root, &err);
        return -1;
    }
    fclose(file);

    // Replace node values with sum of left and right subtrees
    if (replace(root, &err) == -1) {
        printf("Error replacing tree values\n");
        free_tree(root, &err);
        return -1;
    }

    // Write modified tree to file
    file = fopen("tree_after.txt", "w");
    if (!file) {
        printf("Error opening file for writing\n");
        free_tree(root, &err);
        return -1;
    }
    if (inorder_to_file(root, file, &err) == -1) {
        printf("Error writing tree to file\n");
        fclose(file);
        free_tree(root, &err);
        return -1;
    }
    fclose(file);

    // Free all memory
    if (free_tree(root, &err) == -1) {
        printf("Error freeing memory\n");
        return -1;
    }

    printf("Program finished successfully.\n");
    return 0;
}

// Function to insert a value into BST
Node* insert(Node* root, int value, int* err) {
    if (!root) {
        root = (Node*)malloc(sizeof(Node));
        if (!root) {
            *err = -1;
            return NULL;
        }
        root->data = value;
        root->left = root->right = NULL;
        *err = 0;
        return root;
    }
    if (value < root->data) {
        root->left = insert(root->left, value, err);
    } else {
        root->right = insert(root->right, value, err);
    }
    return root;
}

// Helper function to calculate sum of subtree
int subtree_sum(Node* root, int* err) {
    if (!root) return 0;
    return root->data + subtree_sum(root->left, err) + subtree_sum(root->right, err);
}

// Replace each node with sum of left and right subtrees
int replace(Node* root, int* err) {
    if (!root) return 0;

    int left_sum = replace(root->left, err);
    int right_sum = replace(root->right, err);

    int old_value = root->data;
    root->data = left_sum + right_sum;

    return root->data + old_value;
}

// Inorder traversal to write tree to file
int inorder_to_file(Node* root, FILE* file, int* err) {
    if (!root) return 0;

    if (inorder_to_file(root->left, file, err) == -1) return -1;

    if (fprintf(file, "%d ", root->data) < 0) {
        *err = -1;
        return -1;
    }

    if (inorder_to_file(root->right, file, err) == -1) return -1;

    return 0;
}

// Free all nodes of the tree
int free_tree(Node* root, int* err) {
    if (!root) return 0;

    if (free_tree(root->left, err) == -1) return -1;
    if (free_tree(root->right, err) == -1) return -1;

    free(root);
    return 0;
}
