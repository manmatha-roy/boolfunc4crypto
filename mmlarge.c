#include <stdio.h>
#include <stdlib.h>

// Helper function to compute the XOR of an array of bits
int xor_array(int *arr, int length) {
    int result = 0;
    for (int i = 0; i < length; i++) {
        result ^= arr[i];
    }
    return result;
}

int array_equality(int *arr1, int *arr2, int length) {
    for (int i = 0; i < length; i++) {
        if(arr1[i] != arr2[i])
            return 1;
    }
    return 0;
}

int array_sum(int *arr1, int *arr2, int length) {
    int result = 0;
    for (int i = 0; i < length; i++) {
            result = arr1[i] + arr2[2];
    }
    return result;
}
int array_product(int *arr1, int *arr2, int length) {
    int result = 0;
    for (int i = 0; i < length; i++) {
            result = result + (arr1[i] * arr2[i]);
    }
    return result;
}
int is_filled_with(int *arr, int length, int v){
    for (int i = 0; i < length; i++) {
        if(arr[i] != v)
            return 1;
    }
    return 0;
}

// Function g(rr) from Definition D:subsubfunc
int g(int *rr, int t) {
    int iota = (t - 1) / 2;
    int result = 0;

    // First part of the function: XOR between r_i and r_{i+iota}
    for (int i = 0; i < iota; i++) {
        result ^= rr[i] * rr[i + iota];
    }

    // Second part: XOR from i = 2iota+1 to t
    for (int i = 2*iota; i < t; i++) {
        result ^= rr[i];
    }

    return result;
}

// Function p(zz, rr) from Definition D:bents
int p(int *zz, int *rr, int t) {
    int result = 0;
    for (int i = 0; i < t; i++) {
        result ^= zz[i] * rr[i];
    }
    return result;
}

// Function q(zz, rr) from Definition D:bents
int q(int *zz, int *rr, int t) {

    if(is_filled_with(zz,t, 0)==0) {
        return g(rr, t);
    } else {
        int result = 0;
        for (int i = 1; i < t; i++) {
            result ^= zz[i] * rr[i - 1];
        }
        result ^= zz[0] * rr[t - 1];
        result ^= zz[1] * rr[t - 1];
        return result;
    }
}

// Function u(zz, rr) from Definition D:subfunction
int u(int *zz, int *rr, int t) {
    int p_value = p(zz, rr, t);
    int q_value = q(zz, rr, t);
    return p_value * q_value;
}

// Function v(zz, rr) from Definition D:subfunction
int v(int *zz, int *rr, int t) {
    int p_value = p(zz, rr, t);
    int q_value = q(zz, rr, t);
    return (p_value ^ 1) * q_value ;
}

// Main function to generate the truth table for f(xx, yy)
void generate_truth_table(int t) {
    int n = 4 * t;
    int k = 2 * t;  // The number of variables on each half
    int size = 1 << k;  // Total number of combinations on each half (2^2t)
    
    // Allocate memory for input vectors
    int xx[k], yy[k];

    // Generate the truth table
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int z = 0; z < k; z++) {
                xx[k] = (i >> z) & 1;
                yy[k] = (j >> z) & 1;
            }

            // Determine which case of the function to compute
            int result = 0;
            if (is_filled_with(xx, k, 1) == 0 && array_equality(xx, yy, k) == 1) {
                result = u(yy, yy+t, t);
            } else if (array_equality(xx, yy, k) == 0) {
                result = v(xx, xx+t, t);
            } else {
                // Case for the else part
                result = array_product((xx, yy, k), yy, k);
            }

            // Print the result for the current input pair (xx, yy)
            printf("%d ", result);
        }
        printf("\n");
    }
}

int main() {
    int t;
    printf("Enter the value of t: ");
    scanf("%d", &t);

    if (t >= 3) {
        generate_truth_table(t);
    } else {
        printf("t must be greater than or equal to 3.\n");
    }

    return 0;
}
