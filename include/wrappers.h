/*  
 * wrappers.h
 *
 * All SDL wrapper functions declarations go here
 *
 */

#ifndef WRAPPERS_
#define WRAPPERS_

/*  
 * Set the alpha layer onto a texture
 *
 * @param texture   The texture to set alpha to
 * @param alpha     The alpha number 0-255
 *
 */
void setTextureAlpha(SDL_Texture * texture, uint8_t alpha);

/*  
 * Load new SDL Texture
 *
 * @param image     Path to the image to load
 * @param surface   Optional parameter, preloaded SDL Surface or NULL to create
 *
 * @return the newly loaded SDL Texture
 *
 */
SDL_Texture * loadTexture(char * image, SDL_Surface * surface);

/*  
 * Load new SDL Texture with background colour removed
 *
 * @param image     Path to the image to load
 * @param colourR   (R)GB colour to be removed
 * @param colourG   R(G)B colour to be removed
 * @param colourB   RG(B) colour to be removed
 *
 * @return the newly loaded SDL Texture
 *
 */
SDL_Texture * loadTextureBack(char * image, uint8_t colourR, uint8_t colourG, uint8_t colourB);

/*  
 * Apply Texture to global renderer
 *
 * @param x         The x coordinate to apply on the renderer
 * @param y         The y coordinate to apply on the renderer
 * @param source    The source Texture to apply
 * @param clip      The Texture clip of the object on the spritesheet
 *
 */
void applyTexture(int x, int y, SDL_Texture * source, SDL_Rect * clip);

/*  
 * Initialize SDL
 *
 * @param title     The window title
 *
 * @return 1 on success 0 on failure 
 *
 */
int init(char * title);

/*  
 * Clear the renderer to the last set colour
 *
 */
void clearScreen();

/*  
 * Delay the game by the remaining frames per second
 *
 * @param timer     The global timer
 *
 */
void delayFramesPerSecond(uint32_t timer);

/*  
 * Update the window with the latest renderer
 *
 */
void updateWindow();
#endif
