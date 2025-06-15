/**
 * @file algorithms.c
 * @brief Implementation of the different algorithms
 * @author Jesús Blázquez
 */
#include <stdlib.h>
#include <time.h>

#include "algorithms.h"

#define MICRO		   1000000
#define NANO_PER_MICRO 1000

#define STEP(step, us_step) \
	if (sorting_stop) \
		return; \
	step.tv_sec = us_step / MICRO; \
	step.tv_nsec = (us_step % MICRO) * NANO_PER_MICRO; \
	nanosleep(&step, NULL);

volatile bool sorting_stop = false;

void quick_sort(int* arr, int left, int right, int us_step)
{
	if (left >= right)
		return;

	int pivot = arr[right];
	int i = left - 1;
	struct timespec step;

	for (int j = left; j < right; j++) {
		if (arr[j] < pivot) {
			i++;
			// Swap arr[i] and arr[j]
			int temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			STEP(step, us_step);
		}
	}

	// Swap arr[i+1] and arr[right] (placing pivot)
	int temp = arr[i + 1];
	arr[i + 1] = arr[right];
	arr[right] = temp;

	STEP(step, us_step);

	int pivot_index = i + 1;
	quick_sort(arr, left, pivot_index - 1, us_step);
	quick_sort(arr, pivot_index + 1, right, us_step);
}

void merge(int* arr, int left, int mid, int right, int us_step);

void merge_sort(int* arr, int left, int right, int us_step)
{
	if (left >= right)
		return;

	int mid = (left + right) / 2;
	merge_sort(arr, left, mid, us_step);
	merge_sort(arr, mid + 1, right, us_step);
	merge(arr, left, mid, right, us_step);
}

void heapify(int* arr, int n, int i, int us_step, struct timespec* step)
{
	if (sorting_stop)
		return;

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
		STEP((*step), us_step);
		heapify(arr, n, largest, us_step, step);
	}
}

void heap_sort(int* arr, int left, int right, int us_step)
{
	int n = right - left + 1;
	struct timespec step;
	for (int i = n / 2 - 1; i >= 0; i--) {
		heapify(arr, n, i, us_step, &step);
	}
	for (int i = n - 1; i >= 0; i--) {
		int temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;

		STEP(step, us_step);

		heapify(arr, i, 0, us_step, &step);
	}
}

void insertion_sort(int* arr, int left, int right, int us_step)
{
	for (int i = left + 1; i <= right; i++) {
		int key = arr[i];
		int j = i - 1;
		struct timespec step;
		while (j >= left && arr[j] > key) {
			arr[j + 1] = arr[j];
			j--;
			STEP(step, us_step);
		}
		arr[j + 1] = key;
	}
}

void bubble_sort(int* arr, int left, int right, int us_step)
{
	struct timespec step;
	for (int i = left; i < right; i++) {
		for (int j = left; j < right - i; j++) {
			if (arr[j] > arr[j + 1]) {
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				STEP(step, us_step);
			}
		}
	}
}

void selection_sort(int* arr, int left, int right, int us_step)
{
	struct timespec step;
	for (int i = left; i < right; i++) {
		int min_index = i;
		for (int j = i + 1; j <= right; j++) {
			if (arr[j] < arr[min_index]) {
				min_index = j;
			}
		}
		if (min_index != i) {
			int temp = arr[i];
			arr[i] = arr[min_index];
			arr[min_index] = temp;
			STEP(step, us_step);
		}
	}
}

bool is_sorted(int* arr, int left, int right);
void shuffle(int* arr, int left, int right);

void bogo_sort(int* arr, int left, int right, int us_step)
{
	struct timespec step;
	while (!is_sorted(arr, left, right)) {
		shuffle(arr, left, right);
		STEP(step, us_step);
	}
}

bool is_sorted(int* arr, int left, int right)
{
	for (int i = left; i < right; i++) {
		if (arr[i] > arr[i + 1])
			return false;
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

void merge(int* arr, int left, int mid, int right, int us_step)
{
	int n1 = mid - left + 1;
	int n2 = right - mid;
	int L[n1], R[n2];

	struct timespec step;

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
		STEP(step, us_step);
	}

	while (i < n1) {
		arr[k++] = L[i++];
		STEP(step, us_step);
	}

	while (j < n2) {
		arr[k++] = R[j++];
		STEP(step, us_step);
	}
}
