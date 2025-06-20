/**
 * @file config.h
 * @brief Configuration file
 * @author Jesús Blázquez
 */
#ifndef CONFIG_H
#define CONFIG_H

/**
 * @brief Width of the window
 */
#define WIDTH 800

/**
 * @brief Height of the window
 */
#define HEIGHT 600

/**
 * @brief Microseconds between step and step. Lower values make things go faster
 */
#define US_STEP 10000

/**
 * @brief Number of elements in the list
 */
#define LIST_SIZE 200

/**
 * @brief Spacing between bars. Can make things not go properly if too big
 */
#define SPACING 0.0f

/**
 * @brief Minimum frequency
 */
#define MIN_FREQ 220.0f

/**
 * @brief Maximum frequency
 */
#define MAX_FREQ 880.0f

#endif
