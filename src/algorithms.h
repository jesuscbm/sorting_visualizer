/**
 * @file algorithms.h
 * @brief Header for the algorithms
 * @author Jesús Blázquez
 */
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stdbool.h>

/**
 * @brief Flag to stop sorting
 */
extern volatile bool sorting_stop;

/**
 * Quick sort. Divide and conquer with pivot.
 *
 * @param arr Array to sort
 * @param left Left index
 * @param right Right index
 * @param us_step Microseconds between step and step
 */
void quick_sort(int* arr, int left, int right, int us_step);

/**
 * Merge sort. Divide and conquer.
 *
 * @param arr Array to sort
 * @param left Left index
 * @param right Right index
 * @param us_step Microseconds between step and step
 */
void merge_sort(int* arr, int left, int right, int us_step);

/**
 * Heap sort. Counterintuitive but fast.
 *
 * @param arr Array to sort
 * @param left Left index
 * @param right Right index
 * @param us_step Microseconds between step and step
 */
void heap_sort(int* arr, int left, int right, int us_step);

/**
 * Insertion sort. Local algorithm.
 *
 * @param arr Array to sort
 * @param left Left index
 * @param right Right index
 * @param us_step Microseconds between step and step
 */
void insertion_sort(int* arr, int left, int right, int us_step);

/**
 * Bubble sort. Local algorithm, always slow.
 *
 * @param arr Array to sort
 * @param left Left index
 * @param right Right index
 * @param us_step Microseconds between step and step
 */
void bubble_sort(int* arr, int left, int right, int us_step);

/**
 * Selection sort. Local algorithm.
 *
 * @param arr Array to sort
 * @param left Left index
 * @param right Right index
 * @param us_step Microseconds between step and step
 */
void selection_sort(int* arr, int left, int right, int us_step);

/**
 * Bogo sort. Esoteric algorithm.
 *
 * @param arr Array to sort
 * @param left Left index
 * @param right Right index
 * @param us_step Microseconds between step and step
 */
void bogo_sort(int* arr, int left, int right, int us_step);

#endif
