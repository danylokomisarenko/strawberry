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

typedef struct {
	int index;
	sb_package* package;
	sb_thread_progress progress;
} sb_package_progress;

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
	sb_package_progress* info = (sb_package_progress*) progress;
	for (int i = 0; i <= 20 * (info->index + 1); i++) {
		info->progress.complete = i;
		info->progress.total = 20 * (info->index + 1);
		info->progress.suffix = "wasting your time";
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
		int count = packages.index;
		DWORD thread_ids[count];
		HANDLE threads[count];
		sb_package_progress progresses[count];
		for (int i = 0; i < count; i++) {
			sb_package_progress package_progress;
			package_progress.index = i;
			package_progress.package = &packages.packages[i];
			progresses[i] = package_progress;

			threads[i] = CreateThread(NULL, 0, hoisted_sync_package, (void*) &progresses[i], 0, &thread_ids[i]);
		}

		sb_thread_progress* thread_progresses[count];
		for (int i = 0; i < count; i++) {
			thread_progresses[i] = &progresses[i].progress;
		}
		sb_progress_threaded(count, thread_ids, threads, thread_progresses);
	}

	return error;
}
