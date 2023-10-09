#include "socket.h"
#include <stdio.h>

sb_code sb_socket_create(sb_socket* out_socket, char* port) {	
	int error = 0;
	struct addrinfo* result = NULL, *ptr = NULL, hint;
	sb_socket sock = INVALID_SOCKET;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	hint.ai_flags = AI_PASSIVE;

	error = getaddrinfo(NULL, port, &hint, &result);
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
	freeaddrinfo(result);

	if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
		printf("failed to listen\n");
		sb_socket_close(sock);
		sb_socket_cleanup();
		return SB_ERROR;
	}

	*out_socket = sock;
	return SB_OK;
}

sb_code sb_socket_accept(sb_socket* in_socket, sb_connection* out_connection) {
	sb_connection connection = {};
	sb_socket sock = INVALID_SOCKET;

	sock = accept(*in_socket, NULL, NULL);
	if (sock == INVALID_SOCKET) {
		printf("failed to accept connection\n");
		return SB_ERROR;
	}

	connection.near_sock = in_socket;
	connection.far_sock = sock;
	*out_connection = connection;

	return SB_OK;
}
