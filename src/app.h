#ifndef APP_H__HEADER_GUARD__
#define APP_H__HEADER_GUARD__

#include <stdlib.h>  /* exit, EXIT_FAILURE, size_t, srand, rand */
#include <stdbool.h> /* bool, true, false */
#include <time.h>    /* time, time_t */
#include <math.h>    /* cos, sin, atan2 */
#include <string.h>  /* memset */
#include <stdio.h>   /* snprintf */

#include <SDL2/SDL.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define TITLE "Bouncy squares"

#define SCREEN_W 800
#define SCREEN_H 450

#define FPS_CAP 60

#define FRICTION     0.96
#define ACCELERATION 0.35

#define BOX_SCALE 45

#define SIZE_OF(p_arr) (sizeof(p_arr) / sizeof(p_arr[0]))

typedef struct {
	size_t w, h;
	float  x, y;
	float  vel_x, vel_y;
	float  bounce;
} entity_t;

SDL_Rect entity_get_SDL_Rect(entity_t *p_entity);
bool     entity_touches_point(entity_t *p_entity, int p_x, int p_y);

typedef struct {
	SDL_Window   *window;
	SDL_Renderer *renderer;

	SDL_Event    event;
	const Uint8 *key_states;
	SDL_Point    mouse;

	size_t fps, tick;

	entity_t entities[1024];
	size_t   entities_count;

	entity_t *selected;
	bool      dragging;
	float     dir, dist;

	bool quit, paused;
} app_t;

void app_init(app_t *p_app, size_t p_entities_count);
void app_finish(app_t *p_app);
void app_run(app_t *p_app);

void app_calc_shoot_info(app_t *p_app);

void app_render(app_t *p_app);
void app_events(app_t *p_app);

#endif
