#include <strawberry.h>
#include <stdio.h>

int main(int argc, char** argv) {
	sb_parameter_parser parser; sb_parameter_initialize_parser(&parser, argc, argv);	
	sb_parameter_create(&parser, "clean", "false", 1, 0);
	sb_parameter_create(&parser, "sync", "false", 1, 0);
	sb_parameter_create(&parser, "install", "", 0, 0);

	char* should_clean; sb_parameter_get_value(&parser, &should_clean, "clean");
	if (strcmp(should_clean, "true") == SB_OK) {
		printf("cleaning cache\n");
	}

	char* should_sync; sb_parameter_get_value(&parser, &should_sync, "sync");
	if (strcmp(should_sync, "true") == SB_OK) {
		printf("syncing packages\n");
	}

	char* to_install; sb_parameter_get_value(&parser, &to_install, "install");
	if (strcmp(to_install, "") != SB_OK) {
		printf("installing '%s'\n", to_install);
	}

	return SB_OK;
}
