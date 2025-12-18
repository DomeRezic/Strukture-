#include <stdio.h>
#include <stdlib.h>

// Node structure for the BST
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// Function declarations
Node* createNode(int data);
Node* insert(Node* root, int data);
Node* findMin(Node* root);
Node* deleteNode(Node* root, int data);
Node* findNode(Node* root, int data);
void inorder(Node* root);
void preorder(Node* root);
void postorder(Node* root);
void levelOrder(Node* root);
int height(Node* root);
void freeTree(Node* root);
void enqueue(Node** queue, int* rear, Node* node);
Node* dequeue(Node** queue, int* front);

// Main function
int main() {
    Node* root = NULL;
    int choice, value;

    while (1) {
        printf("\nBinary Search Tree Operations:\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Find\n");
        printf("4. Inorder Traversal\n");
        printf("5. Preorder Traversal\n");
        printf("6. Postorder Traversal\n");
        printf("7. Level Order Traversal\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter value to insert: ");
                if (scanf("%d", &value) != 1) { printf("-1\n"); break; }
                root = insert(root, value);
                break;
            case 2:
                printf("Enter value to delete: ");
                if (scanf("%d", &value) != 1) { printf("-1\n"); break; }
                root = deleteNode(root, value);
                break;
            case 3:
                printf("Enter value to find: ");
                if (scanf("%d", &value) != 1) { printf("-1\n"); break; }
                if (findNode(root, value))
                    printf("%d found in the tree.\n", value);
                else
                    printf("%d not found.\n", value);
                break;
            case 4:
                inorder(root);
                printf("\n");
                break;
            case 5:
                preorder(root);
                printf("\n");
                break;
            case 6:
                postorder(root);
                printf("\n");
                break;
            case 7:
                levelOrder(root);
                printf("\n");
                break;
            case 8:
                freeTree(root);
                exit(0);
            default:
                printf("-1\n");
        }
    }

    freeTree(root);
    return 0;
}

// Function definitions

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("-1\n");
        return NULL;
    }
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, int data) {
    if (!root) return createNode(data);
    if (data < root->data) root->left = insert(root->left, data);
    else if (data > root->data) root->right = insert(root->right, data);
    return root;
}

Node* findMin(Node* root) {
    while (root && root->left) root = root->left;
    return root;
}

Node* deleteNode(Node* root, int data) {
    if (!root) return root;
    if (data < root->data) root->left = deleteNode(root->left, data);
    else if (data > root->data) root->right = deleteNode(root->right, data);
    else {
        if (!root->left) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        Node* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

Node* findNode(Node* root, int data) {
    if (!root) return NULL;
    if (data == root->data) return root;
    if (data < root->data) return findNode(root->left, data);
    return findNode(root->right, data);
}

void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void preorder(Node* root) {
    if (!root) return;
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node* root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

int height(Node* root) {
    if (!root) return 0;
    int lHeight = height(root->left);
    int rHeight = height(root->right);
    return (lHeight > rHeight ? lHeight : rHeight) + 1;
}

void enqueue(Node** queue, int* rear, Node* node) {
    queue[*rear] = node;
    (*rear)++;
}

Node* dequeue(Node** queue, int* front) {
    (*front)++;
    return queue[*front - 1];
}

void levelOrder(Node* root) {
    if (!root) return;
    int front = 0, rear = 0;
    Node** queue = (Node**)malloc(1000 * sizeof(Node*)); // max 1000 nodes
    if (!queue) { printf("-1\n"); return; }

    enqueue(queue, &rear, root);

    while (front < rear) {
        Node* current = dequeue(queue, &front);
        printf("%d ", current->data);
        if (current->left) enqueue(queue, &rear, current->left);
        if (current->right) enqueue(queue, &rear, current->right);
    }
    free(queue);
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
