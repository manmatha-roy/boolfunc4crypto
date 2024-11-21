#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_K 20 // Adjust as necessary for larger values of k
#define MAX_T (MAX_K / 2)
#define MAX_N (1 << MAX_K) // 2^k

// Function to compute g(r) based on the definition
int g_function(int *r, int t) {
    int iota = (t - 1) / 2;
    int term1 = 0, term2 = 0;
    for (int i = 0; i < iota; i++) {
        term1 ^= (r[i] & r[i + iota]);
    }
    for (int i = 2 * iota; i < t; i++) {
        term2 ^= r[i];
    }
    return term1 ^ term2;
}

// Function to compute p(z, r)
int p_function(int *z, int *r, int t) {
    int result = 0;
    for (int i = 0; i < t; i++) {
        result ^= (z[i] & r[i]);
    }
    return result;
}

// Function to compute q(z, r)
int q_function(int *z, int *r, int t) {
    int all_zero = 1;
    for (int i = 0; i < t; i++) {
        if (z[i] != 0) {
            all_zero = 0;
            break;
        }
    }

    if (all_zero) {
        return g_function(r, t);
    } else {
        int g_r = g_function(r, t);
        int term1 = g_r;
        for (int i = 0; i < t; i++) {
            term1 &= (z[i] ^ 1);
        }
        int term2 = 0, term3 = 0, term4 = 0;
        for (int i = 1; i < t; i++) {
            term2 ^= (z[i] & r[i - 1]);
        }
        term3 = z[0] & r[t - 1];
        term4 = z[1] & r[t - 1];
        return term1 ^ term2 ^ term3 ^ term4;
    }
}

// Function to compute u(z, r)
int u_function(int *z, int *r, int t) {
    return p_function(z, r, t) * q_function(z, r, t);
}

// Function to compute v(z, r)
int v_function(int *z, int *r, int t) {
    return (p_function(z, r, t) ^ 1) * q_function(z, r, t);
}

// Function to compute f(x, y)
int f_function(int *x, int *y, int k, int t) {
    int x_is_one = 1, y_is_one = 1;
    for (int i = 0; i < k; i++) {
        if (x[i] != 1) x_is_one = 0;
        if (y[i] != 1) y_is_one = 0;
    }

    if (x_is_one && !y_is_one) {
        return u_function(y, y, t);
    } else if (memcmp(x, y, k * sizeof(int)) == 0) {
        return v_function(x, x, t);
    } else {
        int result = 0;
        for (int i = 0; i < k; i++) {
            result ^= ((x[i] ^ y[i]) & y[i]);
        }
        return result;
    }
}

// Walsh-Hadamard transform
void walsh_hadamard_transform(int *truth_table, int size, int *spectrum) {
    memcpy(spectrum, truth_table, size * sizeof(int));
    for (int len = 1; len < size; len *= 2) {
        for (int i = 0; i < size; i += 2 * len) {
            for (int j = 0; j < len; j++) {
                int a = spectrum[i + j];
                int b = spectrum[i + j + len];
                spectrum[i + j] = a + b;
                spectrum[i + j + len] = a - b;
            }
        }
    }
}

// Generate the truth table for f(x, y)
void generate_truth_table(int k, int *truth_table) {
    int t = k / 2;
    int n = 1 << k;
    int x[MAX_T], y[MAX_T];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int bit = 0; bit < k; bit++) {
                x[bit] = (i >> bit) & 1;
                y[bit] = (j >> bit) & 1;
            }
            truth_table[i * n + j] = f_function(x, y, k, t);
        }
    }
}

// Main function
int main() {
    int k = 6; // Replace with desired even k
    int n = 1 << k;
    int *truth_table = (int *)malloc(n * n * sizeof(int));
    int *spectrum = (int *)malloc(n * n * sizeof(int));

    // Generate truth table
    generate_truth_table(k, truth_table);

    // Print truth table
    printf("Truth Table:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", truth_table[i * n + j]);
        }
        printf("\n");
    }

    // Compute Walsh-Hadamard spectrum
    walsh_hadamard_transform(truth_table, n * n, spectrum);

    // Print spectrum
    printf("\nWalsh-Hadamard Spectrum:\n");
    for (int i = 0; i < n * n; i++) {
        printf("%d ", spectrum[i]);
    }
    printf("\n");

    free(truth_table);
    free(spectrum);
    return 0;
}
