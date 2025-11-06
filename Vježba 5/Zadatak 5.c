#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Node structure for linked list stack
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Function declarations
int push(Node** top, int value);
int pop(Node** top, int* value);
int evaluatePostfix(const char* expr, int* result);
int isOperator(char c);

int main() {
    FILE* file = fopen("postfix.txt", "r");
    if (!file) {
        printf("Failed to open file.\n");
        return -1;
    }

    char expr[256];
    if (!fgets(expr, sizeof(expr), file)) {
        printf("Failed to read from file.\n");
        fclose(file);
        return -1;
    }
    fclose(file);

    // Remove newline if present
    expr[strcspn(expr, "\n")] = '\0';

    int result;
    int evalStatus = evaluatePostfix(expr, &result);
    if (evalStatus == -1) {
        printf("Error evaluating postfix expression.\n");
        return -1;
    }

    printf("Result: %d\n", result);
    return 0;
}

// Push value onto stack (update via pointer)
int push(Node** top, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return -1; // malloc failed
    newNode->data = value;
    newNode->next = *top;
    *top = newNode;
    return 0;
}

// Pop value from stack
int pop(Node** top, int* value) {
    if (*top == NULL) return 0; // stack empty
    Node* temp = *top;
    *value = temp->data;
    *top = temp->next;
    free(temp);
    return 1;
}

// Check if character is an operator
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Evaluate postfix expression
int evaluatePostfix(const char* expr, int* result) {
    Node* stack = NULL;
    const char* p = expr;

    while (*p != '\0') {
        // Skip spaces and tabs
        while (isspace(*p)) p++;
        if (*p == '\0') break;

        // If it's a number (handles negative numbers)
        if (isdigit(*p) || (*p == '-' && isdigit(*(p + 1)))) {
            char* end;
            long num = strtol(p, &end, 10);
            if (push(&stack, (int)num) == -1) return -1;
            p = end; // move pointer past number
        }
        // If it's an operator
        else if (isOperator(*p)) {
            int val2, val1;
            if (!pop(&stack, &val2) || !pop(&stack, &val1)) {
                printf("Not enough operands for operator '%c'\n", *p);
                return -1;
            }

            int res;
            switch (*p) {
                case '+': res = val1 + val2; break;
                case '-': res = val1 - val2; break;
                case '*': res = val1 * val2; break;
                case '/':
                    if (val2 == 0) {
                        printf("Division by zero error\n");
                        return -1;
                    }
                    res = val1 / val2;
                    break;
            }

            if (push(&stack, res) == -1) return -1;
            p++; // move past operator
        }
        // Invalid character
        else {
            printf("Invalid character '%c' in expression\n", *p);
            return -1;
        }
    }

    // Final result
    if (!pop(&stack, result)) {
        printf("No result found, empty stack\n");
        return -1;
    }

    if (stack != NULL) {
        printf("Error: extra operands left in stack\n");
        return -1;
    }

    return 0;
}

