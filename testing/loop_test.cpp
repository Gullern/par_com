#include <iostream>
#include <sys/time.h>

int main(int argc, char **argv) {

    struct timeval start, end;
    gettimeofday(&start, NULL);

    double r = 0;

    if (false) {
        for (int i = 0; i < 100000; ++i) {
            for (int j = 0; j < 10000; ++j) {
                r += 100;
            }
            r -= 1000;
        }
    } else {
        for (int i = 0; i < 1000000000; ++i) {
            r += 100;
            if (i % 10000 == 0) {
                r -= 1000;
            }
        }
    }

    gettimeofday(&end, NULL);

    printf("%lf\n", r);

    printf("%ld\n", (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));

    return 0;
}

