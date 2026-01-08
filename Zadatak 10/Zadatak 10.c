#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =================== STRUCT DEFINITIONS ===================
typedef struct CityNode {
    char name[50];
    int population;
    struct CityNode* left;
    struct CityNode* right;
} CityNode;

typedef struct CityListNode {
    char name[50];
    int population;
    struct CityListNode* next;
} CityListNode;

typedef struct CountryListNode {
    char name[50];
    CityNode* cityTree;
    struct CountryListNode* next;
} CountryListNode;

typedef struct CountryTreeNode {
    char name[50];
    CityListNode* cityList;
    struct CountryTreeNode* left;
    struct CountryTreeNode* right;
} CountryTreeNode;

// =================== FUNCTION DECLARATIONS ===================

// City tree functions
CityNode* insert_city_tree(CityNode* root, char* name, int population);
int free_city_tree(CityNode* root);

// City linked list functions
CityListNode* insert_city_list_sorted(CityListNode* head, char* name, int population);
int free_city_list(CityListNode* head);

// Country linked list functions
int insert_country_list_sorted(CountryListNode** head, char* name, CityNode* cityTree);
int print_country_list(CountryListNode* head);
int free_country_list(CountryListNode* head);

// Country tree functions
CountryTreeNode* insert_country_tree(CountryTreeNode* root, char* name, CityListNode* cityList);
int print_country_tree(CountryTreeNode* root);
int free_country_tree(CountryTreeNode* root);

// File reading
CityNode* read_city_file(char* filename);

// Search
int search_city_in_country_tree(CountryTreeNode* root, char* country, int population);

// =================== MAIN FUNCTION ===================
int main() {
    FILE* drzave = fopen("drzave.txt", "r");
    if (!drzave) {
        printf("Cannot open drzave.txt\n");
        return -1;
    }

    CountryListNode* countryList = NULL;
    CountryTreeNode* countryTree = NULL;
    char countryName[50], cityFile[50];

    while (fscanf(drzave, "%s %s", countryName, cityFile) == 2) {
        CityNode* cityTree = read_city_file(cityFile);
        if (!cityTree) continue;

        insert_country_list_sorted(&countryList, countryName, cityTree);

        // Convert city tree to sorted city list
        CityListNode* cityList = NULL;
        void inorder_to_list(CityNode* root) {
            if (!root) return;
            inorder_to_list(root->left);
            cityList = insert_city_list_sorted(cityList, root->name, root->population);
            inorder_to_list(root->right);
        }
        inorder_to_list(cityTree);

        countryTree = insert_country_tree(countryTree, countryName, cityList);

        free_city_tree(cityTree);
    }
    fclose(drzave);

    printf("=== Countries and cities (linked list) ===\n");
    print_country_list(countryList);

    printf("\n=== Countries and cities (tree) ===\n");
    print_country_tree(countryTree);

    char searchCountry[50];
    int minPop;
    printf("\nEnter country to search: ");
    scanf("%s", searchCountry);
    printf("Enter minimum population: ");
    scanf("%d", &minPop);

    search_city_in_country_tree(countryTree, searchCountry, minPop);

    free_country_list(countryList);
    free_country_tree(countryTree);

    return 0;
}

// =================== FUNCTION DEFINITIONS ===================

// -------- CITY TREE FUNCTIONS --------
CityNode* insert_city_tree(CityNode* root, char* name, int population) {
    if (!root) {
        root = (CityNode*)malloc(sizeof(CityNode));
        if (!root) return NULL;
        strcpy(root->name, name);
        root->population = population;
        root->left = root->right = NULL;
        return root;
    }
    if (population < root->population || (population == root->population && strcmp(name, root->name) < 0))
        root->left = insert_city_tree(root->left, name, population);
    else
        root->right = insert_city_tree(root->right, name, population);
    return root;
}

int free_city_tree(CityNode* root) {
    if (!root) return 0;
    free_city_tree(root->left);
    free_city_tree(root->right);
    free(root);
    return 0;
}

// -------- CITY LINKED LIST FUNCTIONS --------
CityListNode* insert_city_list_sorted(CityListNode* head, char* name, int population) {
    CityListNode* newNode = (CityListNode*)malloc(sizeof(CityListNode));
    if (!newNode) return head;
    strcpy(newNode->name, name);
    newNode->population = population;
    newNode->next = NULL;

    if (!head || population < head->population || (population == head->population && strcmp(name, head->name) < 0)) {
        newNode->next = head;
        return newNode;
    }

    CityListNode* current = head;
    while (current->next && (current->next->population < population ||
           (current->next->population == population && strcmp(current->next->name, name) < 0))) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
    return head;
}

int free_city_list(CityListNode* head) {
    while (head) {
        CityListNode* temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}

// -------- COUNTRY LINKED LIST FUNCTIONS --------
int insert_country_list_sorted(CountryListNode** head, char* name, CityNode* cityTree) {
    CountryListNode* newNode = (CountryListNode*)malloc(sizeof(CountryListNode));
    if (!newNode) return -1;
    strcpy(newNode->name, name);
    newNode->cityTree = cityTree;
    newNode->next = NULL;

    if (!(*head) || strcmp(name, (*head)->name) < 0) {
        newNode->next = *head;
        *head = newNode;
        return 0;
    }

    CountryListNode* current = *head;
    while (current->next && strcmp(current->next->name, name) < 0)
        current = current->next;

    newNode->next = current->next;
    current->next = newNode;
    return 0;
}

int print_country_list(CountryListNode* head) {
    while (head) {
        printf("Country: %s\n", head->name);
        printf("Cities:\n");
        void inorder(CityNode* root) {
            if (!root) return;
            inorder(root->left);
            printf("\t%s (%d)\n", root->name, root->population);
            inorder(root->right);
        }
        inorder(head->cityTree);
        head = head->next;
    }
    return 0;
}

int free_country_list(CountryListNode* head) {
    while (head) {
        free_city_tree(head->cityTree);
        CountryListNode* temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}

// -------- COUNTRY TREE FUNCTIONS --------
CountryTreeNode* insert_country_tree(CountryTreeNode* root, char* name, CityListNode* cityList) {
    if (!root) {
        root = (CountryTreeNode*)malloc(sizeof(CountryTreeNode));
        if (!root) return NULL;
        strcpy(root->name, name);
        root->cityList = cityList;
        root->left = root->right = NULL;
        return root;
    }
    if (strcmp(name, root->name) < 0)
        root->left = insert_country_tree(root->left, name, cityList);
    else
        root->right = insert_country_tree(root->right, name, cityList);
    return root;
}

int print_country_tree(CountryTreeNode* root) {
    if (!root) return 0;
    print_country_tree(root->left);
    printf("Country: %s\n", root->name);
    printf("Cities:\n");
    CityListNode* cur = root->cityList;
    while (cur) {
        printf("\t%s (%d)\n", cur->name, cur->population);
        cur = cur->next;
    }
    print_country_tree(root->right);
    return 0;
}

int free_country_tree(CountryTreeNode* root) {
    if (!root) return 0;
    free_country_tree(root->left);
    free_country_tree(root->right);
    free_city_list(root->cityList);
    free(root);
    return 0;
}

// -------- FILE READING FUNCTION --------
CityNode* read_city_file(char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return NULL;

    CityNode* root = NULL;
    char line[100];
    while (fgets(line, sizeof(line), f)) {
        char city[50];
        int population;
        if (sscanf(line, " %49[^,], %d", city, &population) == 2) {
            root = insert_city_tree(root, city, population);
        }
    }
    fclose(f);
    return root;
}

// -------- SEARCH FUNCTION --------
int search_city_in_country_tree(CountryTreeNode* root, char* country, int population) {
    if (!root) return 0;
    if (strcmp(country, root->name) < 0)
        return search_city_in_country_tree(root->left, country, population);
    else if (strcmp(country, root->name) > 0)
        return search_city_in_country_tree(root->right, country, population);
    else {
        printf("Cities in %s with population > %d:\n", country, population);
        CityListNode* cur = root->cityList;
        int found = 0;
        while (cur) {
            if (cur->population > population) {
                printf("\t%s (%d)\n", cur->name, cur->population);
                found = 1;
            }
            cur = cur->next;
        }
        if (!found) printf("\tNo cities found.\n");
        return 1;
    }
}
