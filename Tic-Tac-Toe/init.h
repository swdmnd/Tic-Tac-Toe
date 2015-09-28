#ifndef INIT
#define INIT

//native library
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//sdl library
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern SDL_Surface* screen;
extern SDL_Event e;
extern Uint8* keystate;
extern int mouse_x, mouse_y;
extern int position;
extern const int FPS;
extern int current_fps;
extern int game_mode;
extern int turn;
extern bool reset;
extern bool click;

bool init_window( char* title[20], int width, int height, int bpp );

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* target, SDL_Rect* clip );
void set_color_key( SDL_Surface* img, int red, int green, int blue );

SDL_Surface* load_img( char* img[20], int mode );
SDL_Surface* render_text( char* font_name[20], int size, char* text[50], int red, int green, int blue );

void clean_up();

#endif
