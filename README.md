# Visualizer for Sorting Algorithms

Developed in C with [SDL3](https://examples.libsdl.org/SDL3/). This small
project allows you to visualize how different sorting algorithms work, like
those youtube videos we all have seen. This is not the first one nor the best
one, but this one is in C and that is something. Includes:

- [Quick Sort](https://en.wikipedia.org/wiki/Quicksort)
- [Merge Sort](https://en.wikipedia.org/wiki/Merge_sort)
- [Heap Sort](https://en.wikipedia.org/wiki/Heapsort)
- [Bubble Sort](https://en.wikipedia.org/wiki/Bubble_sort)
- [Insertion Sort](https://en.wikipedia.org/wiki/Insertion_sort)
- [Selection Sort](https://en.wikipedia.org/wiki/Selection_sort)
- [Bogo Sort](https://en.wikipedia.org/wiki/Bogo_sort)

The algorithms are implemented in `src/algorithms.c` and the visualizer is in
`src/main.c`.

## Usage

To compile and run the visualizer you will need to have
[SDL3](https://examples.libsdl.org/SDL3/) installed, as well as `make` and
`gcc` installed. To compile, just run:

```bash
make
./main
```

## Configuring

The visualizer can be configured in `src/config.h`. Options are:

- `WIDTH` and `HEIGHT` for the window size
- `US_STEP` between updates for the algorithms
- `LIST_SIZE` for the size of the list

## Adding New Algorithms

### Creating a New Algorithm

The new algorithm must be implemented in `src/algorithms.c` and declared in
`src/algorithms.h`. The algorithms are executed in a separate thread, which has
the advantage of us being able to reuse code from the algorithms directly, and
merely insert `STEP(step, us_step)` when we want the visualizer to see the code.

### Adding the algorithm to the code

Adding a new algorithm to the code is relatively simple. Steps to follow are:

1. Add the algorithm declaration to `src/algorithms.h` and the implementation
   to `src/algorithms.c`
2. Add the name of the algorithm to the `State` enum in `src/main.c`
3. In the switch statement in the function `SDL_AppEvent` add the key handler
   for the new algorithm:

```c
    /* In main.c */
		switch (event->key.key) {
        /* ... */
		case SDLK_8:
			state = NEW_SORT;
			updateThread();
			break;
		case SDLK_ESCAPE:
        /* ... */
		}
```

4. In the function `showMenu` add the new algorithm to the types list.
5. Finally, in the function `threadFunction` add the new algorithm to the
   `switch` statement:

```c
    /* In main.c */
	switch (state) {
        /* ... */
        case BOGO_SORT:
            new_sort(list, 0, LIST_SIZE - 1, US_STEP);
            break;
        /* ... */
	}
```

## TODO:

- Consider semaphores so that code is genuinely multithreaded
- Controls for speed and number of bars
- Add sounds
- Add multiple colors (f.e, red to indicate comparison)
- Add more algorithms
- Use SDL3_ttf??
