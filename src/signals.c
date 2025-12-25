#include <signal.h>
#include <stdio.h>
#include "signals.h"

volatile sig_atomic_t stop = 0;

void on_sigint(int signum) {
        (void)signum;
        stop = 1;
        printf("\n");
}

