#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11

// =================== STRUCT DEFINITIONS ===================
typedef struct CityNode {
    char name[50];
    int population;
    struct CityNode* left;
    struct CityNode* right;
} CityNode;

typedef struct CountryNode {
    char name[50];
    CityNode* cityTree;
    struct CountryNode* next; // For collisions in the hash table (sorted by country name)
} CountryNode;

// =================== FUNCTION DECLARATIONS ===================

// Hash table functions
int hash_country(char* country);
int insert_country_hash(CountryNode** table, char* countryName, CityNode* cityTree);
int print_country_hash(CountryNode** table);
int free_country_hash(CountryNode** table);

// City tree functions
CityNode* insert_city_tree(CityNode* root, char* name, int population);
int free_city_tree(CityNode* root);
int print_city_tree_inorder(CityNode* root);

// File reading
CityNode* read_city_file(char* filename);

// Search
int search_city_in_hash(CountryNode** table, char* country, int population);

// =================== MAIN FUNCTION ===================
int main() {
    FILE* drzave = fopen("drzave.txt", "r");
    if (!drzave) {
        printf("Cannot open drzave.txt\n");
        return -1;
    }

    CountryNode* countryTable[TABLE_SIZE] = {0};
    char countryName[50], cityFile[50];

    while (fscanf(drzave, "%s %s", countryName, cityFile) == 2) {
        CityNode* cityTree = read_city_file(cityFile);
        if (!cityTree) continue;

        if (insert_country_hash(countryTable, countryName, cityTree) == -1) {
            free_city_tree(cityTree);
        }
    }
    fclose(drzave);

    printf("=== Countries and cities (hash table) ===\n");
    print_country_hash(countryTable);

    char searchCountry[50];
    int minPop;
    printf("\nEnter country to search: ");
    scanf("%s", searchCountry);
    printf("Enter minimum population: ");
    scanf("%d", &minPop);

    search_city_in_hash(countryTable, searchCountry, minPop);

    free_country_hash(countryTable);

    return 0;
}

// =================== FUNCTION DEFINITIONS ===================

// -------- HASH FUNCTION --------
int hash_country(char* country) {
    int sum = 0;
    for (int i = 0; i < 5 && country[i]; i++) {
        sum += (int)country[i];
    }
    return sum % TABLE_SIZE;
}

// -------- COUNTRY HASH TABLE FUNCTIONS --------
int insert_country_hash(CountryNode** table, char* countryName, CityNode* cityTree) {
    int index = hash_country(countryName);
    CountryNode* newNode = (CountryNode*)malloc(sizeof(CountryNode));
    if (!newNode) return -1;
    strcpy(newNode->name, countryName);
    newNode->cityTree = cityTree;
    newNode->next = NULL;

    // Insert sorted by country name
    if (!table[index] || strcmp(countryName, table[index]->name) < 0) {
        newNode->next = table[index];
        table[index] = newNode;
        return 0;
    }

    CountryNode* current = table[index];
    while (current->next && strcmp(current->next->name, countryName) < 0)
        current = current->next;

    newNode->next = current->next;
    current->next = newNode;

    return 0;
}

int print_country_hash(CountryNode** table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i]) {
            printf("Hash index %d:\n", i);
            CountryNode* cur = table[i];
            while (cur) {
                printf("  Country: %s\n", cur->name);
                printf("  Cities:\n");
                print_city_tree_inorder(cur->cityTree);
                cur = cur->next;
            }
        }
    }
    return 0;
}

int free_country_hash(CountryNode** table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        CountryNode* cur = table[i];
        while (cur) {
            free_city_tree(cur->cityTree);
            CountryNode* temp = cur;
            cur = cur->next;
            free(temp);
        }
        table[i] = NULL;
    }
    return 0;
}

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

int print_city_tree_inorder(CityNode* root) {
    if (!root) return 0;
    print_city_tree_inorder(root->left);
    printf("    %s (%d)\n", root->name, root->population);
    print_city_tree_inorder(root->right);
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
int search_city_in_hash(CountryNode** table, char* country, int population) {
    int index = hash_country(country);
    CountryNode* cur = table[index];
    while (cur && strcmp(cur->name, country) != 0)
        cur = cur->next;

    if (!cur) {
        printf("Country %s not found.\n", country);
        return 0;
    }

    printf("Cities in %s with population > %d:\n", country, population);
    CityNode* stack[100]; // iterative inorder to avoid recursion if needed
    int top = -1;
    CityNode* node = cur->cityTree;

    while (top != -1 || node) {
        while (node) {
            stack[++top] = node;
            node = node->left;
        }
        node = stack[top--];
        if (node->population > population)
            printf("    %s (%d)\n", node->name, node->population);
        node = node->right;
    }
    return 1;
}
