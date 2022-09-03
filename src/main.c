#include <stdlib.h> /* size_t, strtol, exit, EXIT_FAILURE */
#include <stdio.h>  /* puts */

#include "app.h"

int main(int p_argc, char **p_argv) {
	app_t app = {0};

	size_t entities_count = 16;
	if (p_argc > 1) {
		if (p_argc != 2) {
			puts("Usage: app [BOXES_COUNT]");

			exit(EXIT_FAILURE);
		}

		entities_count = (size_t)strtol(p_argv[1], NULL, 10);
		if (entities_count > 1024) {
			puts("Error: Boxes count can not be more than 1024");

			exit(EXIT_FAILURE);
		}
	}

	app_init(&app, entities_count);
	app_run(&app);
	app_finish(&app);

	return 0;
}
