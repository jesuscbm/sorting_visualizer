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
	MENU,			/**< Currently showing the menu */
	QUICK_SORT,		/**< Currently sorting with quick sort */
	MERGE_SORT,		/**< Currently sorting with merge sort */
	HEAP_SORT,		/**< Currently sorting with heap sort */
	BUBBLE_SORT,	/**< Currently sorting with bubble sort */
	SELECTION_SORT, /**< Currently sorting with selection sort */
	INSERTION_SORT, /**< Currently sorting with insertion sort */
	BOGO_SORT		/**< Currently sorting with bogo sort */
} state;

/**
 * @brief List to sort
 */
int* list;

/**
 * @brief Rectangles to draw
 */
SDL_FRect* rects;

/**
 * @brief Thread that does the sorting
 */
pthread_t thread = 0;

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
		fprintf(stderr, "Unable to allocate memory: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	shuffleList();

	/* Initialize rectangles */
	rects = calloc(LIST_SIZE, sizeof(SDL_FRect));
	if (!rects) {
		fprintf(stderr, "Unable to allocate memory: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

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
		case SDLK_ESCAPE:
			state = MENU;
			updateThread();
			break;
		case SDLK_RETURN:
		case SDLK_Q:
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

	// Update rects according to the list
	for (int i = 0; i < LIST_SIZE; i++) {
		rects[i].x = 1.0f + i * ((float)WIDTH / LIST_SIZE);
		rects[i].h = list[i] * ((float)HEIGHT / LIST_SIZE);
		rects[i].y = HEIGHT - rects[i].h;
		rects[i].w = ((float)WIDTH / LIST_SIZE) - 1.0f;
	}

	// Paint them
	SDL_SetRenderDrawColorFloat(renderer, 0.0f, 1.0f, 0.0f, 1.0f);
	for (int i = 0; i < LIST_SIZE; i++) {
		SDL_RenderFillRect(renderer, &rects[i]);
	}

	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

/**
 * Called once at the end
 */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void* threadFunction(void* _)
{
	switch (state) {
	case QUICK_SORT:
		quick_sort(list, 0, LIST_SIZE - 1, US_STEP);
		break;
	case MERGE_SORT:
		merge_sort(list, 0, LIST_SIZE - 1, US_STEP);
		break;
    case HEAP_SORT:
        heap_sort(list, 0, LIST_SIZE - 1, US_STEP);
        break;
	case BUBBLE_SORT:
		bubble_sort(list, 0, LIST_SIZE - 1, US_STEP);
		break;
	case SELECTION_SORT:
		selection_sort(list, 0, LIST_SIZE - 1, US_STEP);
		break;
	case INSERTION_SORT:
		insertion_sort(list, 0, LIST_SIZE - 1, US_STEP);
		break;
	case BOGO_SORT:
		bogo_sort(list, 0, LIST_SIZE - 1, US_STEP);
		break;
	default:
		break;
	}
	return NULL;
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

void updateThread(void)
{
	if (thread != 0) {
		sorting_stop = true;
		pthread_join(thread, NULL);
		thread = 0;
	}
	if (state == MENU) {
		return;
	}
	shuffleList();
	sorting_stop = false;
	pthread_create(&thread, NULL, threadFunction, NULL);
}

void showMenu(void)
{
	char* types[] = { "Quick Sort",		"Merge Sort",	  "Heap Sort", "Bubble Sort",
					  "Selection Sort", "Insertion Sort", "Bogo Sort" };
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
