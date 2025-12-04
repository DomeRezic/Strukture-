#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure for an item in an invoice
typedef struct Item {
    char name[100];
    int quantity;
    double price;
    struct Item *next;
} Item;

// Structure for an invoice
typedef struct Invoice {
    char date[11]; // YYYY-MM-DD
    Item *items;
    struct Invoice *next;
} Invoice;

// Function declarations
int read_invoice_file(const char *filename, Invoice **invoice_list);
int insert_invoice_sorted(Invoice **head, Invoice *new_invoice);
int insert_item_sorted(Item **head, Item *new_item);
int query_item(Invoice *invoice_list, const char *item_name, const char *start_date, const char *end_date);
int free_invoices(Invoice *invoice_list);
int free_items(Item *item_list);

// Helper functions
int compare_dates(const char *date1, const char *date2);
int read_invoice_names(const char *filename, char ***files, int *count);

int main() {
    Invoice *invoice_list = NULL;
    char **invoice_files = NULL;
    int file_count = 0;

    if (read_invoice_names("racuni.txt", &invoice_files, &file_count) == -1) {
        printf("Error reading racuni.txt\n");
        return -1;
    }

    for (int i = 0; i < file_count; i++) {
        if (read_invoice_file(invoice_files[i], &invoice_list) == -1) {
            printf("Error reading invoice file: %s\n", invoice_files[i]);
        }
        free(invoice_files[i]);
    }
    free(invoice_files);

    // Example query
    char item_name[100];
    char start_date[11], end_date[11];
    printf("Enter item name: ");
    scanf("%99s", item_name);
    printf("Enter start date (YYYY-MM-DD): ");
    scanf("%10s", start_date);
    printf("Enter end date (YYYY-MM-DD): ");
    scanf("%10s", end_date);

    query_item(invoice_list, item_name, start_date, end_date);

    // Free memory
    free_invoices(invoice_list);

    return 0;
}

// Function definitions

// Reads the list of invoice filenames from racuni.txt
int read_invoice_names(const char *filename, char ***files, int *count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    int capacity = 10;
    *files = (char **)malloc(capacity * sizeof(char *));
    if (!*files) {
        fclose(fp);
        return -1;
    }

    char line[256];
    *count = 0;
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0; // Remove newline
        if (*count >= capacity) {
            capacity *= 2;
            char **temp = realloc(*files, capacity * sizeof(char *));
            if (!temp) {
                fclose(fp);
                return -1;
            }
            *files = temp;
        }
        (*files)[*count] = strdup(line);
        if (!(*files)[*count]) {
            fclose(fp);
            return -1;
        }
        (*count)++;
    }
    fclose(fp);
    return 0;
}

// Reads a single invoice file and inserts it into the invoice list
int read_invoice_file(const char *filename, Invoice **invoice_list) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    Invoice *new_invoice = (Invoice *)malloc(sizeof(Invoice));
    if (!new_invoice) {
        fclose(fp);
        return -1;
    }
    new_invoice->items = NULL;
    new_invoice->next = NULL;

    if (!fgets(new_invoice->date, sizeof(new_invoice->date), fp)) {
        fclose(fp);
        free(new_invoice);
        return -1;
    }
    new_invoice->date[strcspn(new_invoice->date, "\r\n")] = 0; // remove newline

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0; // remove newline
        char name[100];
        int quantity;
        double price;
        if (sscanf(line, "%99[^,], %d, %lf", name, &quantity, &price) != 3) continue;

        Item *new_item = (Item *)malloc(sizeof(Item));
        if (!new_item) continue;
        strcpy(new_item->name, name);
        new_item->quantity = quantity;
        new_item->price = price;
        new_item->next = NULL;

        if (insert_item_sorted(&new_invoice->items, new_item) == -1) {
            free(new_item);
        }
    }

    fclose(fp);
    return insert_invoice_sorted(invoice_list, new_invoice);
}

// Inserts an invoice into the linked list sorted by date
int insert_invoice_sorted(Invoice **head, Invoice *new_invoice) {
    if (!head || !new_invoice) return -1;

    if (!*head || compare_dates(new_invoice->date, (*head)->date) < 0) {
        new_invoice->next = *head;
        *head = new_invoice;
        return 0;
    }

    Invoice *current = *head;
    while (current->next && compare_dates(new_invoice->date, current->next->date) >= 0) {
        current = current->next;
    }
    new_invoice->next = current->next;
    current->next = new_invoice;
    return 0;
}

// Inserts an item into the linked list sorted by name
int insert_item_sorted(Item **head, Item *new_item) {
    if (!head || !new_item) return -1;

    if (!*head || strcmp(new_item->name, (*head)->name) < 0) {
        new_item->next = *head;
        *head = new_item;
        return 0;
    }

    Item *current = *head;
    while (current->next && strcmp(new_item->name, current->next->name) >= 0) {
        current = current->next;
    }
    new_item->next = current->next;
    current->next = new_item;
    return 0;
}

// Queries total quantity and money spent on an item in a date range
int query_item(Invoice *invoice_list, const char *item_name, const char *start_date, const char *end_date) {
    if (!invoice_list || !item_name || !start_date || !end_date) return -1;

    int total_quantity = 0;
    double total_money = 0.0;

    for (Invoice *inv = invoice_list; inv != NULL; inv = inv->next) {
        if (compare_dates(inv->date, start_date) < 0) continue;
        if (compare_dates(inv->date, end_date) > 0) break;

        for (Item *it = inv->items; it != NULL; it = it->next) {
            if (strcmp(it->name, item_name) == 0) {
                total_quantity += it->quantity;
                total_money += it->quantity * it->price;
            }
        }
    }

    printf("Item: %s\n", item_name);
    printf("Total quantity: %d\n", total_quantity);
    printf("Total money spent: %.2lf\n", total_money);
    return 0;
}

// Frees all invoices and their items
int free_invoices(Invoice *invoice_list) {
    while (invoice_list) {
        Invoice *temp = invoice_list;
        invoice_list = invoice_list->next;
        free_items(temp->items);
        free(temp);
    }
    return 0;
}

// Frees all items in an invoice
int free_items(Item *item_list) {
    while (item_list) {
        Item *temp = item_list;
        item_list = item_list->next;
        free(temp);
    }
    return 0;
}

// Compares two dates in YYYY-MM-DD format
int compare_dates(const char *date1, const char *date2) {
    return strcmp(date1, date2);
}
