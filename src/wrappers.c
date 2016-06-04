/* Language Includes */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* SDL Library */
#include <SDL.h>

/* Local Includes */
#include "extern.h"
#include "wrappers.h"
#include "global.h"

int initSDL(char * title)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
       fprintf(stderr, "[%s][%s: %d]Fatal Error: SDL window init error: %s\n", getDate(), __FILE__, __LINE__, SDL_GetError());
       return 0;
    }

    /* Antialiasing buffer, OpenGL */
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

    Global->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if(Global->window == NULL)
    {
        fprintf(stderr, "[%s][%s: %d]Fatal Error: SDL window error: %s\n", getDate(), __FILE__, __LINE__, SDL_GetError());
        return 0;
    }

    /* Use flag SDL_RENDERER_SOFTWARE for legacy machines */
    Global->renderer = SDL_CreateRenderer(Global->window, -1, SDL_RENDERER_ACCELERATED);

    /* 0 nearest (default), 1 linear (OpenGL, Direct3D), 2 anisotropic filtering (Direct3D) */
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    if(Global->renderer == NULL)
    {
        fprintf(stderr, "[%s][%s: %d]Fatal Error: SDL Renderer error: %s\n", getDate(), __FILE__, __LINE__, SDL_GetError());
        return 0;
    }
    else
    {
        SDL_SetRenderDrawColor(Global->renderer, 0x0, 0x0, 0x0, 0xFF);
    }

    SDL_ShowCursor(SDL_DISABLE);

    /* Initialize Window */
    updateWindow();

    return 1;
}

SDL_Texture * loadTexture(char * image, SDL_Surface * surface)
{
    SDL_Texture * texture = NULL;

    if(surface == NULL)
    {
        if(image == NULL)
        {
            fprintf(stderr, "[%s][%s: %d]Warning: image string NULL\n", getDate(), __FILE__, __LINE__);
            return NULL;

        }

        if(strstr(image, "bmp") == NULL)
        {
            fprintf(stderr, "[%s][%s: %d]Warning: Images are not bitmap\n", getDate(), __FILE__, __LINE__);
            return NULL;
        }

        surface = SDL_LoadBMP(image);

        if(surface == NULL)
        {
            fprintf(stderr, "[%s][%s: %d]Warning: Could not load image %s into surface, error: %s\n", getDate(), __FILE__, __LINE__, image, SDL_GetError());
            return NULL;
        }
    }

    texture = SDL_CreateTextureFromSurface(Global->renderer, surface);

    if(texture == NULL)
    {
        fprintf(stderr, "[%s][%s: %d]Warning: Could not create texture %s, error: %s\n", getDate(), __FILE__, __LINE__, image, SDL_GetError());
    }

    SDL_FreeSurface(surface);

    return texture;
}

SDL_Texture * loadTextureBack(char * image, uint8_t colourR, uint8_t colourG, uint8_t colourB)
{
    SDL_Texture * texture = NULL;
    SDL_Surface * surface = NULL;

    surface = SDL_LoadBMP(image);

    if(surface == NULL)
    {
        fprintf(stderr, "[%s][%s: %d]Warning: Could not load image %s into surface, error: %s\n", getDate(), __FILE__, __LINE__, image, SDL_GetError());
        return NULL;
    }

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, colourR, colourG, colourB));

    texture = loadTexture(image, surface);

    if(texture == NULL)
    {
        fprintf(stderr, "[%s][%s: %d]Warning: Could not create textureBack %s, error: %s\n", getDate(), __FILE__, __LINE__, image, SDL_GetError());
    }

    return texture;
}

void applyTexture(int x, int y, SDL_Texture * source, SDL_Rect * clip)
{
    SDL_Rect offset = *clip;

    offset.x = x;
    offset.y = y;

    if(SDL_RenderCopy(Global->renderer, source, clip, &offset))
    {
        fprintf(stderr, "[%s][%s: %d]Warning: Could not render copy, error: %s\n", getDate(), __FILE__, __LINE__, SDL_GetError());
    }
}

void setTextureAlpha(SDL_Texture * texture, uint8_t alpha)
{
    if(SDL_SetTextureAlphaMod(texture, alpha))
    {
        fprintf(stderr, "[%s][%s: %d]Warning: Could not set texture alpha mod, error: %s\n", getDate(), __FILE__, __LINE__, SDL_GetError());
    }
}

void updateWindow()
{
    SDL_RenderPresent(Global->renderer);
}

void delayFramesPerSecond(uint32_t timer)
{
    if((SDL_GetTicks() - timer) < (1000 / Global->framesPerSecond))
    {
        SDL_Delay((1000 / Global->framesPerSecond) - (SDL_GetTicks() - timer));
    }
}

int getNativeHeight()
{
    int height;

    SDL_GetWindowSize(Global->window, NULL, &height);

    return height;
}

int getNativeWidth()
{
    int width;

    SDL_GetWindowSize(Global->window, &width, NULL);

    return width;
}

char * getAbsolutePath(char * relativeString)
{
    static char absoluteString[BUFFER_SIZE] = {0};

    if(relativeString == NULL)
    {
        fprintf(stderr, "[%s][%s: %d]Warning: relative string NULL\n", getDate(), __FILE__, __LINE__);
        return NULL;
    }

    (void)strcpy(absoluteString, Global->basePath);
    (void)strcat(absoluteString, relativeString);

    return absoluteString;
}

void clearScreen()
{
    if(SDL_RenderClear(Global->renderer))
    {
        fprintf(stderr, "[%s][%s: %d]Warning: Could not clear screen renderer, error: %s\n", getDate(), __FILE__, __LINE__, SDL_GetError());
    }
}

void setWindowSize(int width, int height)
{
    if(width <= 0 || height <= 0)
    {
        fprintf(stderr, "[%s][%s: %d]Warning: set window size height or width is < 0\n", getDate(), __FILE__, __LINE__);
        return;
    }

    SDL_SetWindowSize(Global->window, width, height);
}
