#include <stdio.h>
#include <strawberry.h>

int main(int argc, char** argv) {
	printf("using %s %i.%i\n", SB_PROJECT_NAME, SB_VERSION_MAJOR, SB_VERSION_MINOR);

	sb_code error = SB_OK;
	sb_socket socket;

	error = sb_socket_initialize();
	if (error != SB_OK) {
		printf("failed to initialize sockets\n");
		return error;
	}

	error = sb_socket_create(&socket, "7632");
	if (error != SB_OK)
		return error;

	printf("listening on '7632'\n");
	while (1) {
		sb_connection connection;
		error = sb_socket_accept(&socket, &connection);
		if (error != SB_OK)
			continue;

		printf("accepted connection!\n");
	}

	sb_socket_cleanup();
	return SB_OK;
}
