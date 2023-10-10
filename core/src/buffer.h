#ifndef BUFFER_H
#define BUFFER_H

#include <strawberry/common.h>
#include <strawberry/type.h>

void sb_buffer_initialize(sb_buffer* out_buffer, int length);
int sb_buffer_readable(sb_buffer* in_buffer);
int sb_buffer_writable(sb_buffer* in_buffer);
void sb_buffer_cleanup(sb_buffer buffer);

sb_code sb_buffer_write_byte(sb_buffer* in_buffer, sb_byte byte);
sb_code sb_buffer_read_byte(sb_buffer* in_buffer, sb_byte* byte);

#endif
