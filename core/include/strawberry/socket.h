#ifndef INCLUDE_SOCKET_H
#define INCLUDE_SOCKET_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include "common.h"

typedef SOCKET sb_socket;

typedef struct {
   sb_socket* near_sock;
   sb_socket far_sock;
} sb_connection;

SB_DEF_EXTERN sb_socket_initialize();
SB_DEF_EXTERN sb_socket_cleanup();
SB_DEF_EXTERN sb_socket_close(sb_socket in_socket);
SB_DEF_EXTERN sb_socket_create(sb_socket* out_socket, char* hostname, char* port);
SB_DEF_EXTERN sb_socket_accept(sb_socket* in_socket, sb_connection* out_connection);

#endif
