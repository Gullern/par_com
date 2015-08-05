#include <iostream>
#include <sys/time.h>

#include "tensor.h"

int main(int argc, char **argv) {

    std::cout << "test-1\n";
    const int N = 100;
    Tensor<int> t(2, {N, N});
    Tensor<int> s(2, {N, N});

    struct timeval start, end;

    std::cout << "test0\n";

    gettimeofday(&start, NULL);
    t.test_mul(s);
    gettimeofday(&end, NULL);

    printf("%ld\n", (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));

    std::cout << "test1\n";

/*
    int **mat_a = new int*[N];
    int **mat_b = new int*[N];
    int **mat_r = new int*[N];
    for (int i = 0; i < N; ++i) {
        mat_a[i] = new int[N];
        mat_
    }

    int **mat_b[N][N] = new int[N][N];
    int **mat_r[N][N] = new int[N][N];
*/


    int mat_a[N][N];
    int mat_b[N][N];
    int mat_r[N][N];

    std::cout << "test2\n";

    gettimeofday(&start, NULL);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            mat_r[i][j] = mat_a[i][0] * mat_b[0][j];
            for (int k = 1; k < N; ++k) {
                mat_r[i][j] += mat_a[i][k] * mat_b[k][j];
            }
        }
    }

    gettimeofday(&end, NULL);

    std::cout << "this\n" << mat_r[0][0] << std::endl;

    printf("%ld\n", (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
    
    return 0;
}

