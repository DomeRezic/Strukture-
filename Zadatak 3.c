#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person {
    char firstName[50];
    char lastName[50];
    int birthYear;
    struct Person *next;
} Person;

// ---------- Function Prototypes ----------
Person* createPerson(const char* firstName, const char* lastName, int birthYear);
void insertAtBeginning(Person **head, const char* firstName, const char* lastName, int birthYear);
void insertAtEnd(Person **head, const char* firstName, const char* lastName, int birthYear);
void insertAfter(Person *head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear);
void insertBefore(Person **head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear);
void deleteByLastName(Person **head, const char* lastName);
Person* findByLastName(Person *head, const char* lastName);
void sortByLastName(Person **head);
void printList(Person *head);
void writeToFile(Person *head, const char* filename);
void readFromFile(Person **head, const char* filename);
void freeList(Person **head);

// ---------- Function Implementations ----------
Person* createPerson(const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strcpy(newPerson->firstName, firstName);
    strcpy(newPerson->lastName, lastName);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;
    return newPerson;
}

void insertAtBeginning(Person **head, const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = createPerson(firstName, lastName, birthYear);
    newPerson->next = *head;
    *head = newPerson;
}

void insertAtEnd(Person **head, const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = createPerson(firstName, lastName, birthYear);
    if (*head == NULL) {
        *head = newPerson;
        return;
    }
    Person* temp = *head;
    while (temp->next != NULL) temp = temp->next;
    temp->next = newPerson;
}

Person* findByLastName(Person *head, const char* lastName) {
    while (head != NULL) {
        if (strcmp(head->lastName, lastName) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

void deleteByLastName(Person **head, const char* lastName) {
    if (*head == NULL) return;

    Person* temp = *head;
    Person* prev = NULL;

    if (strcmp(temp->lastName, lastName) == 0) {
        *head = temp->next;
        free(temp);
        printf("Deleted successfully.\n");
        return;
    }

    while (temp != NULL && strcmp(temp->lastName, lastName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Person not found.\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
    printf("Deleted successfully.\n");
}

void insertAfter(Person *head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear) {
    Person* target = findByLastName(head, targetLastName);
    if (!target) {
        printf("Person with last name %s not found.\n", targetLastName);
        return;
    }
    Person* newPerson = createPerson(firstName, lastName, birthYear);
    newPerson->next = target->next;
    target->next = newPerson;
    printf("Inserted after %s.\n", targetLastName);
}

void insertBefore(Person **head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear) {
    if (*head == NULL) return;

    if (strcmp((*head)->lastName, targetLastName) == 0) {
        insertAtBeginning(head, firstName, lastName, birthYear);
        printf("Inserted before %s.\n", targetLastName);
        return;
    }

    Person* prev = NULL;
    Person* curr = *head;
    while (curr != NULL && strcmp(curr->lastName, targetLastName) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("Person with last name %s not found.\n", targetLastName);
        return;
    }

    Person* newPerson = createPerson(firstName, lastName, birthYear);
    newPerson->next = curr;
    prev->next = newPerson;
    printf("Inserted before %s.\n", targetLastName);
}

void sortByLastName(Person **head) {
    if (*head == NULL || (*head)->next == NULL) return;

    int swapped;
    Person *ptr1, *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->lastName, ptr1->next->lastName) > 0) {
                char tempFirst[50], tempLast[50];
                int tempYear;

                strcpy(tempFirst, ptr1->firstName);
                strcpy(tempLast, ptr1->lastName);
                tempYear = ptr1->birthYear;

                strcpy(ptr1->firstName, ptr1->next->firstName);
                strcpy(ptr1->lastName, ptr1->next->lastName);
                ptr1->birthYear = ptr1->next->birthYear;

                strcpy(ptr1->next->firstName, tempFirst);
                strcpy(ptr1->next->lastName, tempLast);
                ptr1->next->birthYear = tempYear;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("List sorted by last names.\n");
}

void printList(Person *head) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }
    printf("\n--- Person List ---\n");
    while (head) {
        printf("%s %s, born %d\n", head->firstName, head->lastName, head->birthYear);
        head = head->next;
    }
}

void writeToFile(Person *head, const char* filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    while (head) {
        fprintf(fp, "%s %s %d\n", head->firstName, head->lastName, head->birthYear);
        head = head->next;
    }
    fclose(fp);
    printf("List written to file '%s'.\n", filename);
}

void readFromFile(Person **head, const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    freeList(head);
    char first[50], last[50];
    int year;
    while (fscanf(fp, "%s %s %d", first, last, &year) == 3) {
        insertAtEnd(head, first, last, year);
    }
    fclose(fp);
    printf("List loaded from file '%s'.\n", filename);
}

void freeList(Person **head) {
    Person *temp;
    while (*head) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

// ---------- Helper: Input Person ----------
void inputPersonData(char *first, char *last, int *year) {
    printf("Enter first name: ");
    scanf("%s", first);
    printf("Enter last name: ");
    scanf("%s", last);
    printf("Enter year of birth: ");
    scanf("%d", year);
}

// ---------- MAIN FUNCTION ----------
int main() {
    Person* head = NULL;
    int choice;
    char first[50], last[50], target[50];
    int year;

    do {
        printf("\n============================\n");
        printf("  PERSON LIST MANAGEMENT\n");
        printf("============================\n");
        printf("1. Add person at beginning\n");
        printf("2. Add person at end\n");
        printf("3. Add person after someone\n");
        printf("4. Add person before someone\n");
        printf("5. Delete person by last name\n");
        printf("6. Find person by last name\n");
        printf("7. Print all persons\n");
        printf("8. Sort by last name\n");
        printf("9. Save to file\n");
        printf("10. Load from file\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                inputPersonData(first, last, &year);
                insertAtBeginning(&head, first, last, year);
                break;
            case 2:
                inputPersonData(first, last, &year);
                insertAtEnd(&head, first, last, year);
                break;
            case 3:
                printf("Enter last name of target person: ");
                scanf("%s", target);
                inputPersonData(first, last, &year);
                insertAfter(head, target, first, last, year);
                break;
            case 4:
                printf("Enter last name of target person: ");
                scanf("%s", target);
                inputPersonData(first, last, &year);
                insertBefore(&head, target, first, last, year);
                break;
            case 5:
                printf("Enter last name to delete: ");
                scanf("%s", target);
                deleteByLastName(&head, target);
                break;
            case 6: {
                printf("Enter last name to find: ");
                scanf("%s", target);
                Person* found = findByLastName(head, target);
                if (found)
                    printf("Found: %s %s, born %d\n", found->firstName, found->lastName, found->birthYear);
                else
                    printf("Person not found.\n");
                break;
            }
            case 7:
                printList(head);
                break;
            case 8:
                sortByLastName(&head);
                break;
            case 9:
                writeToFile(head, "people.txt");
                break;
            case 10:
                readFromFile(&head, "people.txt");
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    freeList(&head);
    return 0;
}
