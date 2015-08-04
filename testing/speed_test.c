#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double mul(const double a, const double b) {
    return a * b;
}

int main(int argc, char** argv) {

    struct timeval start, end;

    double a, b;
    int error;
    error = scanf("%lf", &a);
    error = scanf("%lf", &b);

    gettimeofday(&start, NULL);

    double r = 0;
    int i;
    for (i = 0; i < 100000000; ++i) {
        //(a + i) * b;
        //a = a + i;
        r += mul(a + i, b);
        //mul(a + i, b);
    }

    gettimeofday(&end, NULL);

    printf("%lf\n", r);

    printf("%ld\n", (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));


/*
    int *a = malloc(sizeof(int));

    printf("%p\n", (void *)a);
    int *i;
    for (i = a; i < a + 1000; ++i) {
        printf("%u\n", *(i));
    }
*/

    return 0;
}

