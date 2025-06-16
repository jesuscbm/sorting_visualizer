/**
 * @file algorithms.c
 * @brief Implementation of the different algorithms
 * @author Jesús Blázquez
 */
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "algorithms.h"

#define MICRO		   1000000
#define NANO_PER_MICRO 1000

volatile bool sorting_stop = false;

/**
 * @brief Manages the sleep between steps
 *
 * @param info Structure with the needed arguments
 * @param red_index Index of the red bar (-1 for none)
 * @param green_index1 Index of the first green bar (-1 for none)
 * @param green_index2 Index of the second green bar (-1 for none)
 * @param sound_index Index of the bar whose sound to play (-1 for none)
 */
void step(SortArgs* info, int red_index, int green_index1, int green_index2, int sound_index);

void quick_sort(SortArgs* info)
{
	int* arr = info->list;
	int left = info->left;
	int right = info->right;
	int pivot = arr[right];
	int i = left - 1;

	if (left >= right)
		return;

	for (int j = left; j < right; j++) {
		if (arr[j] < pivot) {
			i++;
			// Swap arr[i] and arr[j]
			int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			step(info, -1, j, right, j);
		}
	}

	// Swap arr[i+1] and arr[right] (placing pivot)
	int temp = arr[i + 1];
	arr[i + 1] = arr[right];
	arr[right] = temp;

	step(info, i+1, right, -1, i+1);

	int pivot_index = i + 1;
	info->left = left;
	info->right = pivot_index - 1;
	quick_sort(info);
	info->left = pivot_index + 1;
	info->right = right;
	quick_sort(info);
}

void merge(SortArgs* info, int mid);

void merge_sort(SortArgs* info)
{
	int left = info->left;
	int right = info->right;
	if (left >= right)
		return;

	int mid = (left + right) / 2;
	info->left = left;
	info->right = mid;
	merge_sort(info);
	info->left = mid + 1;
	info->right = right;
	merge_sort(info);

	info->left = left;
	info->right = right;
	merge(info, mid);
}

void merge(SortArgs* info, int mid)
{
	int* arr = info->list;
	int left = info->left;
	int right = info->right;
	int n1 = mid - left + 1;
	int n2 = right - mid;
	int L[n1], R[n2];

	for (int i = 0; i < n1; i++)
		L[i] = arr[left + i];
	for (int j = 0; j < n2; j++)
		R[j] = arr[mid + 1 + j];

	int i = 0, j = 0, k = left;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k++] = L[i++];
		} else {
			arr[k++] = R[j++];
		}
		step(info, -1, left + i, mid + 1 + j, left + i);
	}

	while (i < n1) {
		arr[k++] = L[i++];
		step(info, -1, left + i, -1, left + i);
	}

	while (j < n2) {
		arr[k++] = R[j++];
		step(info, -1, mid + 1 + j, -1, mid + 1 + j);
	}
}

void heapify(SortArgs* info)
{
	int n = info->left;
	int i = info->right;
	int* arr = info->list;
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	if (left < n && arr[left] > arr[largest]) {
		largest = left;
	}
	if (right < n && arr[right] > arr[largest]) {
		largest = right;
	}
	if (largest != i) {
		int temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;
		step(info, -1, i, largest, i);
		info->left = n;
		info->right = largest;
		heapify(info);
	}
}

void heap_sort(SortArgs* info)
{
	int* arr = info->list;
	int left = info->left;
	int right = info->right;
	int n = right - left + 1;

	for (int i = n / 2 - 1; i >= 0; i--) {
		info->left = n;
		info->right = i;
		heapify(info);
	}
	for (int i = n - 1; i >= 0; i--) {
		int temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;

		step(info, -1, 0, i, i);

		info->left = i;
		info->right = 0;
		heapify(info);
	}
}

void insertion_sort(SortArgs* info)
{
	int* arr = info->list;
	int left = info->left;
	int right = info->right;
	for (int i = left + 1; i <= right; i++) {
		int key = arr[i];
		int j = i - 1;

		while (j >= left && arr[j] > key) {
			arr[j + 1] = arr[j];
			j--;
			step(info, -1, j, j + 1, j);
		}
		arr[j + 1] = key;
	}
}

void bubble_sort(SortArgs* info)
{
	int* arr = info->list;
	int left = info->left;
	int right = info->right;

	for (int i = left; i < right; i++) {
		for (int j = left; j < right - i; j++) {
			if (arr[j] > arr[j + 1]) {
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				step(info, -1, j, j + 1, j + 1);
			}
		}
	}
}

void selection_sort(SortArgs* info)
{
	int* arr = info->list;
	int left = info->left;
	int right = info->right;
	for (int i = left; i < right; i++) {
		int min_index = i;
		for (int j = i + 1; j <= right; j++) {
			if (arr[j] < arr[min_index]) {
				min_index = j;
			}
			step(info, -1, j, min_index, j);
		}
		if (min_index != i) {
			int temp = arr[i];
			arr[i] = arr[min_index];
			arr[min_index] = temp;
			step(info, i, -1, min_index, min_index);
		}
	}
}

bool is_sorted(SortArgs* info);
void shuffle(int* arr, int left, int right);

void bogo_sort(SortArgs* info)
{
	int* arr = info->list;
	int left = info->left;
	int right = info->right;
	while (!is_sorted(info)) {
		shuffle(arr, left, right);
		step(info, -1, -1, -1, 0);
	}
}

bool is_sorted(SortArgs* info)
{
	for (int i = info->left; i < info->right; i++) {
		if (info->list[i] > info->list[i + 1])
			return false;
		step(info, -1, i, i+1, i);
	}
	return true;
}

void shuffle(int* arr, int left, int right)
{
	for (int i = left; i < right; i++) {
		int j = rand() % (right - left + 1) + left;
		int temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
}

void step(SortArgs* info, int red_index, int green_index1, int green_index2, int sound_index)
{
	struct timespec step;
	if (sorting_stop) {
		pthread_mutex_unlock(info->mutex);
		pthread_exit(0);
	}

	/* Set all the information for the viewer */
	memset(info->colors, WHITE, sizeof(Color) * LIST_SIZE);
	if (red_index >= 0 && red_index < LIST_SIZE)
		info->colors[red_index] = RED;
	if (green_index1 >= 0 && green_index1 < LIST_SIZE)
		info->colors[green_index1] = GREEN;
	if (green_index2 >= 0 && green_index2 < LIST_SIZE)
		info->colors[green_index2] = GREEN;
	*info->sound_index = sound_index;

	/* Go to sleep */
	step = (struct timespec){ .tv_sec = info->us_step / MICRO,
							  .tv_nsec = (info->us_step % MICRO) * NANO_PER_MICRO };
	pthread_mutex_unlock(info->mutex);
	nanosleep(&step, NULL);
	pthread_mutex_lock(info->mutex);
}
