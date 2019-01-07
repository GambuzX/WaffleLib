#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#define SCREENSAVER_NUMBER_OF_ELEMENTS 4 /* Number of elements in the screensaver */
#define SCREENSAVER_IDLE_TIME 300 /* Time to start screensaver in interrupts */
#define SCREENSAVER_ELE_SPEED 12 /* Speed of the screensaver elements */


/**
 * @defgroup screensaver screensaver module
 * Contains all the code related to the screensaver
 * @{
 */

/**
 * @brief Struct representing an element in the screensaver
 * Holds information about its position, size and movement directions
*/
typedef struct _screensaver_ele {
	uint8_t id; /**<  Element identifier */
	int16_t x, y; /**<  x and y positions */
	uint16_t width, height; /**<  width and height of element */

    int16_t next_x, next_y; /**<  New position after update. Should only be used temporarily */

	int x_move, y_move; /**<  movement in x and y directions */
	//bool collided; /**<  bool representing if this element collided recently */
	bool final_pos; /**<  bool representing if object already reached a new position */

	uint8_t curr_frame; /**< current frame of this element */
	uint8_t n_sprites; /**< number of sprites this element has */
	uint8_t **sprite; /**<  sprite pixmap */
}ScreensaverEle;

/**
 * @brief Initializes the screensaver
 * Allocates memory for the screensaver sprites used and stores them
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int initialize_screensaver();

/**
 * @brief Frees the memory allocated for the screensaver
 */
void free_screensaver();

/**
 * @brief Draws the screensaver and all its elements
 * Calculates new positions and resolves collision
 */
void screensaver_draw();

/**
 * @brief Adds a new element to the screensaver
 *
 * @param x Starting x position of element top left corner
 * @param y Starting y position of element top left corner
 * @param width Width of element in pixels
 * @param height Height of element in pixels
 * @param sprite Sprite array of element
 * @param n_sprites number of frames of new element
 * @return Return 0 upon success and non-zero otherwise
 */
int add_element_to_screensaver(int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t ** sprite, uint8_t n_sprites);

/**
 * @brief Verifies if a given element collides at its new position
 *
 * @param ele Screensaver element that was moved
 * @return Return a pointer to an element colliding with ele, NULL if no collisions
 */
ScreensaverEle * check_collision_at_new_position(ScreensaverEle * ele);

/**
 * @brief Verifies if a given pixel of an element is transparent or not
 * Assumes the coordinates at pixel_x, pixel_y are not transparent
 *
 * @param element Screensaver element to check pixel color
 * @param pixel_x Absolute x coordinate of pixel that was not transparent
 * @param pixel_y Absolute y coordinate of pixel that was not transparent
 * @return true if pixel collides, false otherwise
 */
bool pixel_collides(ScreensaverEle * element, int16_t pixel_x, int16_t pixel_y);

/** @} */

#endif
