#include <strawberry.h>
#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <windows.h>

typedef struct {
	char* name;
	char* branch;
	char* tag;
	char* commit;
} sb_package;

typedef struct {
	int index;
	sb_package packages[64];
} sb_packages;
static sb_packages packages;

int lua_use_package(lua_State* state) {
	lua_settop(state, 1);
	luaL_checktype(state, 1, LUA_TTABLE);

	lua_getfield(state, 1, "name");
	lua_getfield(state, 1, "branch");
	lua_getfield(state, 1, "tag");
	lua_getfield(state, 1, "commit");

	const char* name = luaL_checkstring(state, -4);
	const char* branch = luaL_optstring(state, -3, "master");
	const char* tag = luaL_optstring(state, -2, "");
	const char* commit = luaL_optstring(state, -1, "");
	//lua_gettable(state, -1);

	lua_pop(state, 2);

	sb_package package;
	package.name = malloc(strlen(name) + 1);
	strcpy_s(package.name, strlen(name) + 1, name);
	package.branch = malloc(strlen(branch) + 1);
	strcpy_s(package.branch, strlen(branch) + 1, branch);
	package.tag = malloc(strlen(tag) + 1);
	strcpy_s(package.tag, strlen(tag) + 1, tag);
	package.commit = malloc(strlen(commit) + 1);
	strcpy_s(package.commit, strlen(commit) + 1, commit);
	packages.packages[packages.index] = package;
	packages.index++;

	return 0;
}

sb_code source_config(char config_script[FILENAME_MAX]) {
	lua_State* state = luaL_newstate();
	luaL_openlibs(state);

	lua_pushcfunction(state, lua_use_package);
	lua_setglobal(state, "use");

	if (luaL_dofile(state, config_script) != LUA_OK) {
		puts(lua_tostring(state, lua_gettop(state)));
		return SB_ERROR;
	}

	lua_pop(state, lua_gettop(state));
	lua_close(state);
	return SB_OK;
}

WINAPI DWORD hoisted_sync_package(void* progress) {
	sb_thread_progress* info = (sb_thread_progress*) progress;
	for (int i = 0; i <= 100; i++) {
		info->complete = i;
		info->total = 100;
		info->suffix = "wasting your time";
		sb_sleep(250);
	}
	return 1;
}

int main(int argc, char** argv) {
	int error = SB_OK;

	printf("using strawberry %i.%i\n", SB_VERSION_MAJOR, SB_VERSION_MINOR);

	char cwd[FILENAME_MAX];
	sb_get_work_directory(cwd, FILENAME_MAX);

	sb_packages packs;
	packs.index = 0;
	packages = packs;

	char config_script[FILENAME_MAX];
	strcpy_s(config_script, FILENAME_MAX, cwd);
	strcat_s(config_script, FILENAME_MAX, "\\strawberry-package.lua");
	error = source_config(config_script);
	if (error != SB_OK) {
		printf("[%i] failed to source '%s'\n", error, config_script);
		return error;
	}

	sb_parameter_parser parser; sb_parameter_initialize_parser(&parser, argc, argv);	
	sb_parameter_create(&parser, "clean", "false", 1, 0);
	sb_parameter_create(&parser, "sync", "false", 1, 0);

	char* should_clean; sb_parameter_get_value(&parser, &should_clean, "clean");
	if (strcmp(should_clean, "true") == SB_OK) {
		printf("cleaning cache\n");
	}

	char* should_sync; sb_parameter_get_value(&parser, &should_sync, "sync");
	if (strcmp(should_sync, "true") == SB_OK) {
		printf("syncing packages\n");
		DWORD thread_ids[packages.index];
		HANDLE threads[packages.index];
		sb_thread_progress progresses[packages.index];
		for (int i = 0; i < packages.index; i++) {
			sb_package package = packages.packages[i];

			sb_thread_progress progress;
			progress.complete = 0;
			progress.total = 0;
			progress.suffix = "";
			progresses[i] = progress;

			threads[i] = CreateThread(NULL, 0, hoisted_sync_package, (void*) &progresses[i], 0, &thread_ids[i]);
		}

		sb_progress_threaded(packages.index, thread_ids, threads, progresses);
	}

	return error;
}
