#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int get_length_ppp(double ***arr) {
    int len = 0;
    while (arr[len]) {
        ++len;
    }
    return len;
}

int get_length_pp(double **arr) {
    int len = 0;
    while (arr[len]) {
        ++len;
    }
    return len;
}

int max(int a, int b) {
    return a > b ? a : b;
}

double ***transpose(double ***arr) {
    int rows = get_length_ppp(arr);
    int rows_new = 0;
    for (int i = 0; i < rows; ++i) {
        int curr_length = get_length_pp(arr[i]);
        rows_new = max(rows_new, curr_length);
    }

    double ***ans = calloc(rows_new + 1, sizeof(*ans));

    for (int i = 0; i < rows_new; ++i) {
        ans[i] = calloc(rows + 1, sizeof(**ans));
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; arr[i][j] != NULL; ++j) {
            ans[j][i] = calloc(1, sizeof(***ans));
            ans[j][i][0] = arr[i][j][0];
        }
    }

    for (int i = 0; i < rows_new; ++i) {
        int j = rows;
        while (ans[i][j] == NULL && j >= 0) {
            --j;
        }
        int length = j + 1;
        for (; j>= 0; --j) {
            if (ans[i][j] == NULL) {
                ans[i][j] = calloc(1, sizeof(***ans));
                ans[i][j][0] = 0;
            }
        }
        ans[i] = realloc(ans[i], length * sizeof(***ans));
    }
    return ans;
}