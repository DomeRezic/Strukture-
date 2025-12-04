#include <stdio.h>
#include <stdlib.h>

#define MAX_TERMS 100

// Structure to represent a polynomial term
typedef struct {
    int coeff;
    int exp;
} Term;

// Function declarations
int read_polynomial(const char *filename, Term **poly, int *n);
int add_polynomials(Term *poly1, int n1, Term *poly2, int n2, Term **result, int *n3);
int multiply_polynomials(Term *poly1, int n1, Term *poly2, int n2, Term **result, int *n3);
int print_polynomial(Term *poly, int n);
int sort_polynomial(Term *poly, int n);

// Helper for quicksort
int compare(const void *a, const void *b) {
    Term *t1 = (Term *)a;
    Term *t2 = (Term *)b;
    return t2->exp - t1->exp; // Descending order by exponent
}

int main() {
    Term *poly1 = NULL, *poly2 = NULL;
    Term *sum = NULL, *product = NULL;
    int n1, n2, nsum, nproduct;

    if (read_polynomial("poly1.txt", &poly1, &n1) == -1) return -1;
    if (read_polynomial("poly2.txt", &poly2, &n2) == -1) return -1;

    if (add_polynomials(poly1, n1, poly2, n2, &sum, &nsum) == -1) return -1;
    if (multiply_polynomials(poly1, n1, poly2, n2, &product, &nproduct) == -1) return -1;

    printf("Polynomial 1: ");
    print_polynomial(poly1, n1);

    printf("Polynomial 2: ");
    print_polynomial(poly2, n2);

    printf("Sum: ");
    print_polynomial(sum, nsum);

    printf("Product: ");
    print_polynomial(product, nproduct);

    // Free dynamically allocated memory
    free(poly1);
    free(poly2);
    free(sum);
    free(product);

    return 0;
}

// Function definitions

int read_polynomial(const char *filename, Term **poly, int *n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    *poly = (Term *)malloc(MAX_TERMS * sizeof(Term));
    if (!*poly) {
        fclose(fp);
        return -1;
    }

    int count = 0;
    while (fscanf(fp, "%d %d", &(*poly)[count].coeff, &(*poly)[count].exp) == 2) {
        count++;
        if (count >= MAX_TERMS) break;
    }
    fclose(fp);
    *n = count;

    if (sort_polynomial(*poly, *n) == -1) return -1; // Sort after reading

    return 0;
}

int sort_polynomial(Term *poly, int n) {
    if (!poly || n <= 0) return -1;
    qsort(poly, n, sizeof(Term), compare);
    return 0;
}

int add_polynomials(Term *poly1, int n1, Term *poly2, int n2, Term **result, int *n3) {
    if (!poly1 || !poly2) return -1;

    *result = (Term *)malloc((n1 + n2) * sizeof(Term));
    if (!*result) return -1;

    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2) {
        if (poly1[i].exp > poly2[j].exp) {
            (*result)[k++] = poly1[i++];
        } else if (poly1[i].exp < poly2[j].exp) {
            (*result)[k++] = poly2[j++];
        } else {
            int sum_coeff = poly1[i].coeff + poly2[j].coeff;
            if (sum_coeff != 0) {
                (*result)[k].exp = poly1[i].exp;
                (*result)[k].coeff = sum_coeff;
                k++;
            }
            i++;
            j++;
        }
    }
    while (i < n1) (*result)[k++] = poly1[i++];
    while (j < n2) (*result)[k++] = poly2[j++];

    *n3 = k;
    return sort_polynomial(*result, *n3);
}

int multiply_polynomials(Term *poly1, int n1, Term *poly2, int n2, Term **result, int *n3) {
    if (!poly1 || !poly2) return -1;

    *result = (Term *)calloc(MAX_TERMS * 2, sizeof(Term));
    if (!*result) return -1;

    int k = 0;
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            int exp = poly1[i].exp + poly2[j].exp;
            int coeff = poly1[i].coeff * poly2[j].coeff;

            int found = 0;
            for (int m = 0; m < k; m++) {
                if ((*result)[m].exp == exp) {
                    (*result)[m].coeff += coeff;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                (*result)[k].exp = exp;
                (*result)[k].coeff = coeff;
                k++;
            }
        }
    }
    *n3 = k;
    return sort_polynomial(*result, *n3);
}

int print_polynomial(Term *poly, int n) {
    if (!poly || n <= 0) return -1;

    for (int i = 0; i < n; i++) {
        if (i > 0 && poly[i].coeff > 0) printf(" + ");
        else if (poly[i].coeff < 0) printf(" - ");
        else if (poly[i].coeff == 0) continue;

        int abs_coeff = poly[i].coeff > 0 ? poly[i].coeff : -poly[i].coeff;
        if (poly[i].exp == 0) printf("%d", abs_coeff);
        else if (poly[i].exp == 1) printf("%dx", abs_coeff);
        else printf("%dx^%d", abs_coeff, poly[i].exp);
    }
    printf("\n");
    return 0;
}

