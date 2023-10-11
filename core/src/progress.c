#include "progress.h"
#include <strawberry/common.h>
#include <stdio.h>

void sb_progress(int complete, int total, int length, int complete_color, char* complete_char, int left_color, char* left_char, char* suffix) {
	SB_FIX_UNICODE();
	int percent = total > 0 ? (100 * complete) / total : 0;
    printf("\r%s [ ", percent == 100 ? "\x1B[38;5;46m✔\x1B[0m" : "\x1B[38;5;160m×\x1B[0m");
    for (int i = 0; i < length; i++) {
        if (i < (length * percent) / 100) {
            printf("\x1B[38;5;%im%s", complete_color, complete_char);
        } else {
            printf("\x1B[38;5;%im%s", left_color, left_char);
        }
    }
    printf(" \x1B[0m] %%%i | %s", percent, suffix);
    fflush(stdout);
}

void sb_progress_regular(int complete, int total, char* suffix) {
	sb_progress(complete, total, 50, 39, "━", 33, "-", "wasting your time");
}
