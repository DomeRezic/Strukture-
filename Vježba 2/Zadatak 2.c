#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure
typedef struct {
    char firstName[30];
    char lastName[30];
    int yearOfBirth;
} Person;

// Function prototypes
void addToBeginning(Person **list, int *size, Person newPerson);
void addToEnd(Person **list, int *size, Person newPerson);
void printList(Person *list, int size);
int findByLastName(Person *list, int size, const char *lastName);
int deleteByLastName(Person **list, int *size, const char *lastName);
Person createPerson(const char *firstName, const char *lastName, int yearOfBirth);

// Function implementations

Person createPerson(const char *firstName, const char *lastName, int yearOfBirth) {
    Person p;
    strcpy(p.firstName, firstName);
    strcpy(p.lastName, lastName);
    p.yearOfBirth = yearOfBirth;
    return p;
}

// A. Add new element to the beginning of the list
void addToBeginning(Person **list, int *size, Person newPerson) {
    *list = realloc(*list, (*size + 1) * sizeof(Person));
    if (*list == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Shift elements to the right
    for (int i = *size; i > 0; i--) {
        (*list)[i] = (*list)[i - 1];
    }

    (*list)[0] = newPerson;
    (*size)++;
}

// C. Add new element to the end of the list
void addToEnd(Person **list, int *size, Person newPerson) {
    *list = realloc(*list, (*size + 1) * sizeof(Person));
    if (*list == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    (*list)[*size] = newPerson;
    (*size)++;
}

// B. Print the list
void printList(Person *list, int size) {
    if (size == 0) {
        printf("The list is empty.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        printf("%d. %s %s (%d)\n", i + 1, list[i].firstName, list[i].lastName, list[i].yearOfBirth);
    }
}

// D. Find element in list by last name
int findByLastName(Person *list, int size, const char *lastName) {
    for (int i = 0; i < size; i++) {
        if (strcmp(list[i].lastName, lastName) == 0) {
            return i; // return index
        }
    }
    return -1; // not found
}

// E. Delete a specific element from the list (by last name)
int deleteByLastName(Person **list, int *size, const char *lastName) {
    int index = findByLastName(*list, *size, lastName);
    if (index == -1) {
        return 0; // not found
    }

    for (int i = index; i < *size - 1; i++) {
        (*list)[i] = (*list)[i + 1];
    }

    (*size)--;
    *list = realloc(*list, (*size) * sizeof(Person));
    return 1; // deleted successfully
}

// Main function
int main() {
    Person *people = NULL;
    int size = 0;

    // A. Add to beginning
    addToBeginning(&people, &size, createPerson("John", "Doe", 1990));
    addToBeginning(&people, &size, createPerson("Alice", "Smith", 1985));

    // B. Print list
    printf("Initial list:\n");
    printList(people, size);
    printf("\n");

    // C. Add to end
    addToEnd(&people, &size, createPerson("Bob", "Johnson", 2000));
    printf("After adding to end:\n");
    printList(people, size);
    printf("\n");

    // D. Find element
    const char *searchLastName = "Doe";
    int index = findByLastName(people, size, searchLastName);
    if (index != -1)
        printf("Found %s %s, born in %d\n", people[index].firstName, people[index].lastName, people[index].yearOfBirth);
    else
        printf("Person with last name '%s' not found.\n", searchLastName);
    printf("\n");

    // E. Delete element
    const char *deleteLastName = "Smith";
    if (deleteByLastName(&people, &size, deleteLastName))
        printf("Deleted person with last name '%s'.\n", deleteLastName);
    else
        printf("Person with last name '%s' not found.\n", deleteLastName);
    printf("\n");

    printf("Final list:\n");
    printList(people, size);

    free(people);
    return 0;
}
