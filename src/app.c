#include "app.h"

SDL_Rect entity_get_SDL_Rect(entity_t *p_entity) {
	return (SDL_Rect){
		.x = p_entity->x, .y = p_entity->y,
		.w = p_entity->w, .h = p_entity->h
	};
}

bool entity_touches_point(entity_t *p_entity, int p_x, int p_y) {
	return p_entity->x < p_x &&
	       p_entity->y < p_y &&
	       p_entity->x + p_entity->w > p_x &&
	       p_entity->y + p_entity->h > p_y;
}
void app_init(app_t *p_app, size_t p_entities_count) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("%s", SDL_GetError());

		exit(EXIT_FAILURE);
	}

	p_app->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                                 SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
	if (p_app->window == NULL) {
		SDL_Log("%s", SDL_GetError());

		exit(EXIT_FAILURE);
	}

	p_app->renderer = SDL_CreateRenderer(p_app->window, -1, SDL_RENDERER_ACCELERATED);
	if (p_app->renderer == NULL) {
		SDL_Log("%s", SDL_GetError());

		exit(EXIT_FAILURE);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	if (SDL_RenderSetLogicalSize(p_app->renderer, SCREEN_W, SCREEN_H) != 0) {
		SDL_Log("%s", SDL_GetError());

		exit(EXIT_FAILURE);
	}

	if (SDL_SetRenderDrawBlendMode(p_app->renderer, SDL_BLENDMODE_BLEND) != 0) {
		SDL_Log("%s", SDL_GetError());

		exit(EXIT_FAILURE);
	}

	time_t random = rand();
	srand(time(&random));

	p_app->selected = NULL;
	p_app->entities_count = p_entities_count;
	for (size_t i = 0; i < p_app->entities_count; ++ i) {
		p_app->entities[i].w = BOX_SCALE;
		p_app->entities[i].h = BOX_SCALE;
		p_app->entities[i].x = rand() % (SCREEN_W - BOX_SCALE);
		p_app->entities[i].y = 10;

		p_app->entities[i].bounce = (float)(rand() % 9) / 10;
	}

	p_app->key_states = SDL_GetKeyboardState(NULL);
}

void app_finish(app_t *p_app) {
	SDL_DestroyRenderer(p_app->renderer);
	SDL_DestroyWindow(p_app->window);

	SDL_Quit();
}

void app_run(app_t *p_app) {
	size_t fps_timer = 0;

	do {
		size_t now   = SDL_GetTicks();
		size_t delta = now - fps_timer;

		p_app->fps = 1000 / delta;
		fps_timer   = now;

		char title[32] = {0};
		snprintf(title, sizeof(title), TITLE" FPS: %zu", p_app->fps);

		SDL_SetWindowTitle(p_app->window, title);

		app_render(p_app);
		app_events(p_app);

		SDL_Delay(1000 / FPS_CAP);

		++ p_app->tick;
	} while (!p_app->quit);
}

void app_calc_shoot_info(app_t *p_app) {
	SDL_Point center = {
		.x = p_app->selected->x + p_app->selected->w / 2,
		.y = p_app->selected->y + p_app->selected->h / 2
	};

	p_app->dir  = atan2(p_app->mouse.y - center.y, p_app->mouse.x - center.x);
	p_app->dist = sqrt((p_app->mouse.x - center.x) * (p_app->mouse.x - center.x) +
	                       (p_app->mouse.y - center.y) * (p_app->mouse.y - center.y));
}

void app_render(app_t *p_app) {
	SDL_SetRenderDrawColor(p_app->renderer, 13, 43, 69, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(p_app->renderer);

	SDL_SetRenderDrawColor(p_app->renderer, 208, 129, 89, SDL_ALPHA_OPAQUE);

	for (size_t i = 0; i < p_app->entities_count; ++ i) {
		if (&p_app->entities[i] == p_app->selected)
			continue;

		SDL_SetRenderDrawColor(p_app->renderer,
		                       255 * p_app->entities[i].bounce, 105, 122, SDL_ALPHA_OPAQUE);

		SDL_Rect rect = entity_get_SDL_Rect(&p_app->entities[i]);
		SDL_RenderFillRect(p_app->renderer, &rect);
	}

	if (p_app->selected != NULL) {
		SDL_Rect rect = entity_get_SDL_Rect(p_app->selected);

		SDL_SetRenderDrawColor(p_app->renderer, 0, 0, 0, 80);

		SDL_Rect shadow = rect;
		shadow.x += 10;
		shadow.y += 10;
		SDL_RenderFillRect(p_app->renderer, &shadow);

		SDL_SetRenderDrawColor(p_app->renderer, 161, 125, 142, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(p_app->renderer, &rect);

		if (!p_app->dragging) {
			SDL_SetRenderDrawColor(p_app->renderer, 227, 212, 195, SDL_ALPHA_OPAQUE);

			SDL_Point center = {
				.x = p_app->selected->x + p_app->selected->w / 2,
				.y = p_app->selected->y + p_app->selected->h / 2
			};

			float x = cos(p_app->dir) * p_app->dist, y = sin(p_app->dir) * p_app->dist;
			SDL_RenderDrawLine(p_app->renderer, center.x, center.y, center.x + x, center.y + y);
		}
	}

	SDL_RenderPresent(p_app->renderer);
}

void app_events(app_t *p_app) {
	while (SDL_PollEvent(&p_app->event)) {
		switch (p_app->event.type) {
		case SDL_QUIT:        p_app->quit = true; break;
		case SDL_WINDOWEVENT: app_render(p_app);  break;

		case SDL_KEYDOWN:
			switch (p_app->event.key.keysym.sym) {
			case SDLK_ESCAPE: p_app->quit   = true;           break;
			case SDLK_SPACE:  p_app->paused = !p_app->paused; break;

			case SDLK_r:
				for (size_t i = 0; i < p_app->entities_count; ++ i) {
					p_app->entities[i].y     = 10;
					p_app->entities[i].vel_y = 0;
					p_app->entities[i].vel_x = 0;
				}

				break;

			case SDLK_c:
				for (size_t i = 0; i < p_app->entities_count; ++ i) {
					p_app->entities[i].vel_y = 40 - rand() % 80;
					p_app->entities[i].vel_x = 40 - rand() % 80;
				}

				break;

			case SDLK_m:
				for (size_t i = 0; i < p_app->entities_count; ++ i) {
					float dir = atan2(p_app->mouse.y -
					                  p_app->entities[i].y - p_app->entities[i].h / 2,
					                  p_app->mouse.x -
					                  p_app->entities[i].x - p_app->entities[i].w / 2);

					p_app->entities[i].vel_y = sin(dir) * (10 + rand() % 10);
					p_app->entities[i].vel_x = cos(dir) * (10 + rand() % 10);
				}

				break;

			default: break;
			}

			break;

		case SDL_MOUSEMOTION:
			if (p_app->selected != NULL && !p_app->dragging)
				app_calc_shoot_info(p_app);

			p_app->mouse.x = p_app->event.motion.x;
			p_app->mouse.y = p_app->event.motion.y;

			break;

		case SDL_MOUSEBUTTONDOWN:
			if (p_app->event.button.button == SDL_BUTTON_LEFT) {
				if (!p_app->dragging && p_app->selected != NULL)
					break;

				for (size_t i = 0; i < SIZE_OF(p_app->entities); ++ i) {
					if (entity_touches_point(&p_app->entities[i], p_app->mouse.x, p_app->mouse.y)) {
						p_app->selected        = &p_app->entities[i];
						p_app->selected->vel_y = 0;
						p_app->selected->vel_x = 0;

						p_app->dragging = true;

						break;
					}
				}
			} else if (p_app->event.button.button == SDL_BUTTON_RIGHT) {
				if (p_app->dragging && p_app->selected != NULL)
					break;

				for (size_t i = 0; i < SIZE_OF(p_app->entities); ++ i) {
					if (entity_touches_point(&p_app->entities[i], p_app->mouse.x, p_app->mouse.y)) {
						p_app->selected        = &p_app->entities[i];
						p_app->selected->vel_y = 0;
						p_app->selected->vel_x = 0;

						p_app->dragging = false;
						app_calc_shoot_info(p_app);

						break;
					}
				}
			}

			break;

		case SDL_MOUSEBUTTONUP:
			if (p_app->selected == NULL)
				break;

			if (p_app->event.button.button == SDL_BUTTON_LEFT) {
				p_app->selected = NULL;
				p_app->dragging = false;
			} else if (p_app->event.button.button == SDL_BUTTON_RIGHT) {
				app_calc_shoot_info(p_app);

				p_app->selected->vel_x = cos(p_app->dir) * p_app->dist / 4;
				p_app->selected->vel_y = sin(p_app->dir) * p_app->dist / 4;

				p_app->selected = NULL;
			}

			break;

		default: break;
		}
	}

	if (p_app->selected != NULL && p_app->dragging) {
		p_app->selected->x = (float)p_app->mouse.x - p_app->selected->w / 2;
		p_app->selected->y = (float)p_app->mouse.y - p_app->selected->h / 2;
	}

	if (p_app->paused)
		return;

	for (size_t i = 0; i < SIZE_OF(p_app->entities); ++ i) {
		if (p_app->selected == &p_app->entities[i])
			continue;

		p_app->entities[i].vel_y += ACCELERATION;
		p_app->entities[i].y     += p_app->entities[i].vel_y;
		p_app->entities[i].x     += p_app->entities[i].vel_x;

		if (p_app->entities[i].x + p_app->entities[i].w > SCREEN_W) {
			p_app->entities[i].x      = SCREEN_W - p_app->entities[i].w;
			p_app->entities[i].vel_x *= -p_app->entities[i].bounce;
		} if (p_app->entities[i].x < 0) {
			p_app->entities[i].x      = 0;
			p_app->entities[i].vel_x *= -p_app->entities[i].bounce;
		}

		if (p_app->entities[i].y + p_app->entities[i].h > SCREEN_H) {
			p_app->entities[i].y      = SCREEN_H - p_app->entities[i].h;
			p_app->entities[i].vel_y *= -p_app->entities[i].bounce;
		} else if (p_app->entities[i].y < 0) {
			p_app->entities[i].y      = 0;
			p_app->entities[i].vel_y *= -p_app->entities[i].bounce;
		}
	}
}
