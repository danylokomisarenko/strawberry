#include "buffer.h"

void sb_buffer_initialize(sb_buffer* out_buffer, int length) {
	sb_buffer buffer = {0};
	buffer.length = length;
	buffer.bytes = malloc(sizeof(sb_byte) * length);
	buffer.index = 0;
	*out_buffer = buffer;
}

int sb_buffer_readable(sb_buffer* in_buffer) {
	return in_buffer->index;
}

int sb_buffer_writable(sb_buffer* in_buffer) {
	return in_buffer->length - in_buffer->index;
}

void sb_buffer_cleanup(sb_buffer buffer) {
	buffer.length = -1;
	free(buffer.bytes);
	buffer.index = -1;
}

sb_code sb_buffer_write_byte(sb_buffer* in_buffer, sb_byte byte) {
	if (sb_buffer_writable(in_buffer) == 0)
		return SB_BUFFER_OVERFLOW;
	in_buffer->index++;
	in_buffer->bytes[in_buffer->index] = byte;

	return SB_OK;
}

sb_code sb_buffer_read_byte(sb_buffer* in_buffer, sb_byte* byte) {
	if (sb_buffer_readable(in_buffer) == 0)
		return SB_BUFFER_UNDERFLOW;
	*byte = in_buffer->bytes[in_buffer->index];
	in_buffer->index--;

	return SB_OK;
}
