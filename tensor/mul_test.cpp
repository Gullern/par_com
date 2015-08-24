#include <iostream>
#include <sys/time.h>

#include "tensor.h"

void test();

int main(int argc, char **argv) {
    Tensor<int>::iterator it;
    Tensor<int>::const_iterator cit;
    //test_iterator a;
/*
    A<int>::B<float> test;

    if (typeid(A<int>::B<int>::iterator_category) == typeid(std::random_access_iterator_tag)) {
        std::cout << "nice\n";
    }
*/

    return 0;
}

void test() {
    std::cout << "test-1\n";
    const int N = 1000;
    Tensor<int> t(2, {N, N}, Tensor<int>::Initialization::CONSTANT, 1);
    Tensor<int> s(2, {N, N}, Tensor<int>::Initialization::CONSTANT, 1);

    struct timeval start, end;

    std::cout << "test0\n";

    Tensor<int> r(2, {N, N});
    gettimeofday(&start, NULL);
    t.test_mul(s, r);
    gettimeofday(&end, NULL);

    printf("%ld\n", (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));

    std::cout << "test1\n";

    int **mat_a = new int*[N];
    int **mat_b = new int*[N];
    int **mat_r = new int*[N];
    for (int i = 0; i < N; ++i) {
        mat_a[i] = new int[N];
        mat_b[i] = new int[N];
        mat_r[i] = new int[N];
       for (int j = 0; j < N; ++j) {
            mat_a[i][j] = 1;
            mat_b[i][j] = 1;
            mat_r[i][j] = 1;
        }
    }
/*
    int mat_a[N][N];
    int mat_b[N][N];
    int mat_r[N][N];
*/

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


    printf("%ld\n", (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
    
    std::cout << "thisthis\n";
    bool eq = true;
    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int j = 0; j < N; ++j) {
            if (mat_r[i][j] != *(r.qq_get({i, j}))) {
                eq = false;
            }
        }
    }

    std::cout << eq << std::endl;

    std::cout << r.qq_get({1, 1}) << '\n';
    std::cout << r.qqq_get({1, 1}) << '\n';
}

