#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "asctel"

int main(int argc, char *argv[]) {
    int opt = 0;
    bool all, makeSin, makeCos, makeTan, makeExp, makeLog = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            all = true;
            makeSin = true;
            makeCos = true;
            makeTan = true;
            makeExp = true;
            makeLog = true;
            break;
        case 's': makeSin = true; break;
        case 'c': makeCos = true; break;
        case 't': makeTan = true; break;
        case 'e': makeExp = true; break;
        case 'l': makeLog = true; break;
        default: fprintf(stderr, "Usage %s -[asctel]", argv[0]);
        }
    }
    if (all == true) {
    }
    if (makeSin == true) {
        printf("  x            Sin              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        double r = -2 * M_PI;
        while (r <= 2 * M_PI) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", r, Sin(r), sin(r), Sin(r) - sin(r));
            r = r + 0.1;
        }
    }
    if (makeCos == true) {
        printf("  x            Cos              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        double p = -2 * M_PI;
        while (p <= 2 * M_PI) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", p, Cos(p), cos(p), Cos(p) - cos(p));
            p = p + 0.1;
        }
    }
    if (makeTan == true) {
        printf("  x            Tan              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        double w = -M_PI / 3;
        while (w <= M_PI / 3) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", w, Tan(w), tan(w), Tan(w) - tan(w));
            w = w + 0.1;
        }
    }
    if (makeExp == true) {
        printf("  x            Exp              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        double q = 1;
        while (q < 9.9) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", q, Exp(q), exp(q), Exp(q) - exp(q));
            q = q + 0.1;
        }
    }
    if (makeLog == true) {
        printf("  x            Log              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        double t = 1;
        while (t <= 9.9) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", t, Log(t), log(t), Log(t) - log(t));
            t = t + 0.1;
        }
    }
    return 0;
}
