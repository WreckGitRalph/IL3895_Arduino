/*
 * Marcus Niekraszewicz
 * Graphics and text functions for GoodDisplay IL3895 based ePaper displays
 * Created on March 17, 2018, 5:16 PM
 */

#include "screen.h"

enum orientation {
	VERTICAL=0,
	HORIZONTAL=1
	};

/*
 * IL3895_init
 * Initialize the display
 * Input: rotation - HORIZONTAL, VERTICAL
 * Future: take display model as argument, set config appropriately
 */
void IL3895_init(enum orientation init_orientation);

/*
 * Epaper_Update
 * Write the current display data to the screen
 */
extern void Epaper_Update(void);

/*
 * IL3895_test
 * Put driver into test mode, displaying a series of test images indefinitely
 */
void IL3895_test(void);

/*
* Function Name: Write_String
 * Description : write a string starting at the given coordinates 
 * Input : x - x-coordinate of the top left corner
 *          y - y-coordinate of the top left corner
 *          colour - BLACK, WHITE
 *          inString - ASCII string to write
 * Output : None
 * Note: Co-ordinates do not change relative to the screen
 * 	orientation, but the position top left corner of the 
 * 	string does.
 */
void Write_String(uint16_t x, uint16_t y, uint8_t colour, char inString[]);

/*
* Function Name: Draw rectangle
 * Description : Draw a rectangle of a defined size at the given coordinates 
 * Input : x - x1- starting x coordinate of the rectangle
 *              x2- ending x coordinate of the rectangle
 *              y1- starting y coordinate of the rectangle
 *              y2- ending y coordinate of the rectangle
 *          colour - fill colour of the rectangle: BLACK, WHITE, GREY
 * Output : None
 */
void Draw_Rectangle(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint8_t colour);

/*
 * Function Name: Draw_Image_Fullscreen
 * Description:   Print a 151x151 image to the display. The image must be stored as a bitmask
 *                byte array in images.h
 * Input: img - the name of the image
 * Output: None
 */

void Draw_Image_Fullscreen(uint8_t image);
