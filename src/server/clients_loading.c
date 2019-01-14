#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clients_loading.h"
#include "constants.h"
#include "list.h"
#include "player.h"


void load_clients(const int argc, char *argv[], struct list *players, struct list *libs)
{
    for (int i = 0 ; i < argc ; i++) {
	if (list__size(players) == MAX_NB_PLAYERS) {
	    printf(BOLD(YELLOW("WARNING: A maximum of %d players is allowed,"
			       "therefore only the first %d clients were loaded"))"\n",
		   MAX_NB_PLAYERS, MAX_NB_PLAYERS);
	    break;
	}
	
	printf("Trying to load %s...", argv[i]);

	// If the path does not contains a '/', dlopen won't search the current directory
	// and therefore the client won't be loaded
	char *new_path = malloc(sizeof(char) * strlen(argv[i]) + 3);
	*new_path = '\0'; // Ensure that there is nothing written in new_path beforehand
      
	if (strchr(argv[i], '/') == NULL) {
	    strcat(new_path, "./");
	}
	strcat(new_path, argv[i]);

	void *lib = dlopen(new_path, RTLD_LAZY);
	free(new_path);
	if (lib == NULL) {
	    error(dlerror());
	    continue;
	}
	
	struct player p;
	p.get_name = dlsym(lib, "get_player_name");
	p.initialize_player = dlsym(lib, "initialize");
        p.play_move = dlsym(lib, "play");
	p.finalize_player = dlsym(lib, "finalize");

	if (p.get_name == NULL || p.initialize_player == NULL
	    || p.play_move == NULL || p.finalize_player == NULL) {
	    error(dlerror());
            dlclose(lib);
	    continue;
	}

	if (list__push_back(libs, lib) != SUCCESS) {
	    error("couldn't allocate memory for a library");
	    free_clients(players, libs);
	    exit(1);
	}

	if (list__push_back(players, &p) != SUCCESS) {
	    error("couldn't allocate memory for a player");
	    free_clients(players, libs);
	    exit(1);
	}
	
	printf(BOLD(GREEN("\tLOADED"))"\n");
    }
}


void free_clients(struct list *players, struct list *libs)
{
    list__free(players);
    list__free(libs);
}


void error(const char *message)
{
    fflush(stdout); // Empty buffer so that "FAILED" does not appear before "Trying to load..."
    fprintf(stderr, BOLD(RED("\tFAILED"))" : %s\n", message);
}
