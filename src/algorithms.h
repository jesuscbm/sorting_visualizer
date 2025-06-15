/**
 * @file algorithms.h
 * @brief Header for the algorithms
 * @author Jesús Blázquez
 */
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <pthread.h>
#include <stdbool.h>

#include "config.h"

/**
 * @brief Flag to stop sorting
 */
extern volatile bool sorting_stop;

/**
 * @brief Color enumeration for the bars
 */
typedef enum Color { WHITE = 0, RED, GREEN } Color;

/**
 * @brief Structure with the needed arguments for the algorithms
 */
typedef struct {
	int* list;			   /**< List of integers */
	Color* colors;		   /**< List of colors */
	int left;			   /**< Left index of the list */
	int right;			   /**< Right index of the list */
	int us_step;		   /**< Microseconds between step and step */
	pthread_mutex_t* mutex; /**< Mutex */
} SortArgs;

/**
 * Quick sort. Divide and conquer with pivot.
 *
 * @param info Structure with the needed arguments.
 */
void quick_sort(SortArgs* info);

/**
 * Merge sort. Divide and conquer.
 *
 * @param info Structure with the needed arguments
 */
void merge_sort(SortArgs* info);

/**
 * Heap sort. Counterintuitive but fast.
 *
 * @param info Structure with the needed arguments
 */
void heap_sort(SortArgs* info);

/**
 * Insertion sort. Local algorithm.
 *
 * @param info Structure with the needed arguments
 */
void insertion_sort(SortArgs* info);

/**
 * Bubble sort. Local algorithm, always slow.
 *
 * @param info Structure with the needed arguments
 */
void bubble_sort(SortArgs* info);

/**
 * Selection sort. Local algorithm.
 *
 * @param info Structure with the needed arguments
 */
void selection_sort(SortArgs* info);

/**
 * Bogo sort. Esoteric algorithm.
 *
 * @param info Structure with the needed arguments
 */
void bogo_sort(SortArgs* info);

#endif
