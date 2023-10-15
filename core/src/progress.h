#ifndef PROGRESS_H
#define PROGRESS_H

#include <strawberry/console.h>

void sb_progress(int complete, int total, int length, int complete_color, char* complete_char, int left_color, char* left_char, char* suffix);
void sb_progress_regular(int complete, int total, char* suffix);
void sb_progress_threaded(int count, DWORD* thread_ids, HANDLE* threads, sb_thread_progress** progresses);

#endif
