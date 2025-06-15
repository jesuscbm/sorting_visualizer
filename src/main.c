/**
 * @file main.c
 * @brief Main file. Contains the visualization
 * @author Jesús Blázquez
 */
#include <SDL3/SDL.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#define SDL_MAIN_USE_CALLBACKS 1 /* No need for main() */
#include <SDL3/SDL_main.h>

#include "algorithms.h"
#include "config.h"

/**
 * @brief Line height
 */
#define LINE_HEIGHT 12

/**
 * @brief Maximum number of characters in buffer
 */
#define MAXCHAR 100

/**
 * @brief Window for the application
 */
static SDL_Window* window;

/**
 * @brief Renderer for the application
 */
static SDL_Renderer* renderer;

/**
 * @brief Current state of the application
 */
enum State {
	QUITTING,		/**< Quitting the application */
	MENU,			/**< Currently showing the menu */
	QUICK_SORT,		/**< Currently sorting with quick sort */
	MERGE_SORT,		/**< Currently sorting with merge sort */
	HEAP_SORT,		/**< Currently sorting with heap sort */
	BUBBLE_SORT,	/**< Currently sorting with bubble sort */
	SELECTION_SORT, /**< Currently sorting with selection sort */
	INSERTION_SORT, /**< Currently sorting with insertion sort */
	BOGO_SORT		/**< Currently sorting with bogo sort */
					/*, NEW_SORT */
} state;

/**
 * @brief List to sort
 */
static int* list;

/**
 * @brief Colors to draw the rectangles in
 */
static Color* colors;

/**
 * @brief Rectangles to draw
 */
SDL_FRect* rects;

/**
 * @brief Thread that does the sorting
 */
pthread_t thread = 0;

/**
 * @brief Mutex for the thread. Used to avoid race conditions
 */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Function that does the sorting
 */
void* threadFunction(void* _);

/**
 * @brief Resets and shuffles the list
 */
void shuffleList(void);

/**
 * @brief Updates the thread that does the sorting. Stopping it and creating a new one if needed
 */
void updateThread(void);

/**
 * @brief Shows the menu
 */
void showMenu(void);

/**
 * @brief Initializes the application. Called once
 */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	if (!SDL_SetAppMetadata("Sorting Visualizer", "0.1", "org.sort")) {
		fprintf(stderr, "Unable to set app metadata: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	SDL_CreateWindowAndRenderer("Sorting Visualizer", WIDTH, HEIGHT, 0, &window, &renderer);
	if (!window) {
		fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	if (!renderer) {
		fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	/* Prepare list */
	srand(time(NULL));

	list = calloc(LIST_SIZE, sizeof(int));
	if (!list) {
		fprintf(stderr, "Unable to allocate memory.\n");
		return SDL_APP_FAILURE;
	}
	shuffleList();

	/* Prepare colors */

	colors = calloc(LIST_SIZE, sizeof(Color));
	if (!colors) {
		fprintf(stderr, "Unable to allocate memory.\n");
		return SDL_APP_FAILURE;
	}

	/* Initialize rectangles */
	rects = calloc(LIST_SIZE, sizeof(SDL_FRect));
	if (!rects) {
		fprintf(stderr, "Unable to allocate memory.\n");
		return SDL_APP_FAILURE;
	}

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_unlock(&mutex);
	state = MENU;

	return SDL_APP_CONTINUE;
}

/**
 * @brief Handles events
 */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT)
		return SDL_APP_SUCCESS;
	if (event->type == SDL_EVENT_KEY_DOWN) {
		switch (event->key.key) {
		case SDLK_1:
			state = QUICK_SORT;
			updateThread();
			break;
		case SDLK_2:
			state = MERGE_SORT;
			updateThread();
			break;
		case SDLK_3:
			state = HEAP_SORT;
			updateThread();
			break;
		case SDLK_4:
			state = BUBBLE_SORT;
			updateThread();
			break;
		case SDLK_5:
			state = SELECTION_SORT;
			updateThread();
			break;
		case SDLK_6:
			state = INSERTION_SORT;
			updateThread();
			break;
		case SDLK_7:
			state = BOGO_SORT;
			updateThread();
			break;
		/*
		case SDLK_8:
			state = NEW_SORT;
			updateThread();
			break;
		*/
		case SDLK_ESCAPE:
			state = MENU;
			updateThread();
			break;
		case SDLK_RETURN:
		case SDLK_Q:
			state = QUITTING;
			updateThread();
			return SDL_APP_SUCCESS;
			break;
		default:
			break;
		}
	}

	return SDL_APP_CONTINUE;
}

/**
 * Once every frame
 */
SDL_AppResult SDL_AppIterate(void* appstate)
{
	SDL_SetRenderDrawColorFloat(renderer, 0.0f, 0.0f, 0.0f, 1.0f);
	SDL_RenderClear(renderer);

	if (state == MENU) {
		showMenu();
		SDL_RenderPresent(renderer);
		return SDL_APP_CONTINUE;
	}

	// We will be accessing the list, so we have to lock
	pthread_mutex_lock(&mutex);
	// Update rects according to the list
	for (int i = 0; i < LIST_SIZE; i++) {
		rects[i].x = 1.0f + i * ((float)WIDTH / LIST_SIZE);
		rects[i].h = list[i] * ((float)HEIGHT / LIST_SIZE);
		rects[i].y = HEIGHT - rects[i].h;
		rects[i].w = ((float)WIDTH / LIST_SIZE) - 1.0f;
	}

	// Paint them
	for (int i = 0; i < LIST_SIZE; i++) {
		switch (colors[i]) {
		case RED:
			SDL_SetRenderDrawColorFloat(renderer, 1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case GREEN:
			SDL_SetRenderDrawColorFloat(renderer, 0.0f, 1.0f, 0.0f, 1.0f);
			break;
		default:
			SDL_SetRenderDrawColorFloat(renderer, 1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
		SDL_RenderFillRect(renderer, &rects[i]);
	}
	pthread_mutex_unlock(&mutex);
	SDL_SetRenderDrawColorFloat(renderer, 1.0f, 1.0f, 1.0f, 1.0f);

	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

/**
 * Called once at the end
 */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	free(list);
	free(colors);
	free(rects);
	pthread_mutex_destroy(&mutex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void* threadFunction(void* _)
{
	SortArgs info = { .list = list,
					  .colors = colors,
					  .left = 0,
					  .right = LIST_SIZE - 1,
					  .us_step = US_STEP,
					  .mutex = &mutex };
	/* The thread monopolizes the mutex, only letting go in between steps */
	pthread_mutex_lock(&mutex);
	switch (state) {
	case QUICK_SORT:
		quick_sort(&info);
		break;
	case MERGE_SORT:
		merge_sort(&info);
		break;
	case HEAP_SORT:
		heap_sort(&info);
		break;
	case BUBBLE_SORT:
		bubble_sort(&info);
		break;
	case SELECTION_SORT:
		selection_sort(&info);
		break;
	case INSERTION_SORT:
		insertion_sort(&info);
		break;
	case BOGO_SORT:
		bogo_sort(&info);
		break;
	/*
	case NEW_SORT:
				new_sort(&info);
				break;
	*/
	default:
		break;
	}

	for (int i = 0; i < LIST_SIZE; i++)
		colors[i] = GREEN;
	pthread_mutex_unlock(&mutex);

	return NULL;
}

void showMenu(void)
{
	char* types[] = { "Quick Sort",		"Merge Sort",	  "Heap Sort", "Bubble Sort",
					  "Selection Sort", "Insertion Sort", "Bogo Sort" /*, "New Sort" */ };
	char print[MAXCHAR];
	int n_types = sizeof(types) / sizeof(types[0]);

	// We divide by 4 and not by 2 because we are scaling it.
	float y = (float)HEIGHT / 4 - ((float)n_types * LINE_HEIGHT) / 4;

	SDL_SetRenderScale(renderer, 2.0f, 2.0f);
	SDL_SetRenderDrawColorFloat(renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < n_types; i++) {
		sprintf(print, "%d. %s", i + 1, types[i]);
		SDL_RenderDebugText(renderer, 10.0f, y, print);
		y += LINE_HEIGHT;
	}
	SDL_SetRenderScale(renderer, 1.0f, 1.0f);
}

void updateThread(void)
{
	if (thread != 0) {
		sorting_stop = true;
		pthread_join(thread, NULL);
		thread = 0;
		sorting_stop = false;
	}
	if (state == MENU || state == QUITTING) {
		return;
	}
	shuffleList();
	pthread_create(&thread, NULL, threadFunction, NULL);
}

void shuffleList(void)
{
	/* We may have lost info, so we need to reset the list */
	for (int i = 0; i < LIST_SIZE; i++) {
		list[i] = i + 1;
	}
	/* Shuffle list */
	for (int i = 0; i < LIST_SIZE; i++) {
		int j = rand() % LIST_SIZE;
		int temp = list[i];
		list[i] = list[j];
		list[j] = temp;
	}
}
