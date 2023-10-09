#ifndef SOCKET_H
#define SOCKET_H

#include <strawberry/socket.h>
#include <strawberry/common.h>

sb_code sb_socket_initialize() {
#ifdef _WIN32
	struct WSAData wsa_data;
	return WSAStartup(MAKEWORD(1,1), &wsa_data);
#else
	return SB_OK;
#endif
}

sb_code sb_socket_cleanup() {
#ifdef _WIN32
	return WSACleanup();
#else
	return SB_OK;
#endif
}

sb_code sb_socket_close(sb_socket in_socket) {
	int error = 0;
#ifdef _WIN32
	error = shutdown(in_socket, SD_BOTH);
	if (error != 0)
		return SB_ERROR;
	error = closesocket(in_socket);
#else
	error = shutdown(in_socket, SHUT_RDWR);
	if (error != 0)
		return SB_ERROR;
	error = close(in_socket);
#endif

	if (error != SB_OK)
		return SB_ERROR; 
	return SB_OK;
}

sb_code sb_socket_create(sb_socket* out_socket, char* port);
sb_code sb_socket_accept(sb_socket* in_socket, sb_connection* out_connection);

#endif
