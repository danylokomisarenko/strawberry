#ifndef PROGRESS_H
#define PROGRESS_H

#include <strawberry/common.>
#include <strawberry/type.h>

void sb_progress(int complete, int total, int length, int complete_color, char* complete_char, int left_color, char* left_char, char* suffix);

#endif
