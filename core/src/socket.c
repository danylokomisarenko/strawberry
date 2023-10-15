#include "socket.h"
#include "buffer.h"
#include <stdio.h>

sb_code sb_socket_create(sb_socket* out_socket, char* hostname, char* port) {	
	int error = 0;
	struct addrinfo* result = NULL, *ptr = NULL, hint;
	sb_socket sock = INVALID_SOCKET;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	hint.ai_flags = AI_PASSIVE;

	error = getaddrinfo(hostname, port, &hint, &result);
	if (error != 0) {
		printf("failed to getaddrinfo: %d\n", error);
		return SB_ERROR;
	}

	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET) {
		printf("failed to create socket\n");
		return SB_ERROR;
	}

	error = bind(sock, result->ai_addr, (int)result->ai_addrlen);
	if (error != 0) {
		printf("failed to bind socket\n");
		return SB_ERROR;
	}

	printf("listening on '%s:%s'\n", hostname, port);
	if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
		printf("failed to listen\n");
		sb_socket_close(sock);
		sb_socket_cleanup();
		return SB_ERROR;
	}
	freeaddrinfo(result);

	*out_socket = sock;
	return SB_OK;
}

sb_code sb_socket_accept(sb_socket* in_socket, sb_connection* out_connection) {
	sb_connection connection = {};
	sb_socket sock = INVALID_SOCKET;
	struct sockaddr_in info;
	int info_length = sizeof(info);

	sock = accept(*in_socket, (struct sockaddr*)&info, &info_length);
	if (sock == INVALID_SOCKET) {
		printf("failed to accept connection\n");
		return SB_ERROR;
	}

    printf("IP: '%s:%hu'\n", inet_ntoa(info.sin_addr), htons(info.sin_port));

	connection.near_sock = in_socket;
	connection.far_sock = sock;
	*out_connection = connection;

	return SB_OK;
}

sb_code sb_socket_recieve(sb_socket* in_socket, sb_buffer* out_buffer) {
	sb_code error = SB_OK;
	char buffer[out_buffer->length];
	int read = 0;
	
	do {
		read = recv(*in_socket, buffer, sizeof(buffer), 0);

		if (read == 0)
			return SB_OK; /* Connection was closed */
		if (read < 0)
			return SB_READ_ERROR; /* Error reading */

		for (int i = 0; i < read; i++) {
			error = sb_buffer_write_byte(out_buffer, (sb_byte) buffer[i]);
			if (error != SB_OK)
				return error;
		}
	} while (read > 0);

	return SB_OK;
}

sb_code sb_socket_send(sb_socket* in_socket, sb_buffer in_buffer) {
	sb_code error = 0;
	char buffer[in_buffer.length];
	for (int i = 0; i < sizeof(buffer); i++) {
		sb_byte byte;
		error = sb_buffer_read_byte(&in_buffer, &byte);
		if (error != SB_OK)
			return error;
		buffer[i] = (const char) byte;
	}

	if (send(*in_socket, (const char*) buffer, sizeof(buffer), 0) < 0)
		return SB_WRITE_ERROR;

	return SB_OK;
}
