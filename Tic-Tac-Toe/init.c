//include header file
#include "init.h"

//all library are included in init.h

const int SCREEN_WIDTH = 0x258;//600; cuma test hexadecimal
const int SCREEN_HEIGHT = 01130;//600; cuma test oktadecimal
SDL_Surface* screen = NULL;
SDL_Event e;
Uint8* keystate = NULL;
int mouse_x, mouse_y;
const int FPS = 100;
int current_fps;
int game_mode = 0;
int turn = 1;
bool reset = true;
bool click = false;
extern bool click;
int position = 0;
/*
    0 : menu
    1 : game
    2 : game menu
    3 : credit
*/

bool init_window( char* title[20], int width, int height, int bpp )
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) return false;
    
    screen = SDL_SetVideoMode( width, height, bpp, SDL_SWSURFACE );
    
    if ( screen == NULL ) return false;
    if ( TTF_Init() == -1 ) return false;
    
    SDL_WM_SetCaption( title, NULL );
    keystate = SDL_GetKeyState( NULL );
    current_fps = FPS;
    
    return true;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* target, SDL_Rect* clip )
{
    SDL_Rect offset;
    
    offset.x = x;
    offset.y = y;
    
    SDL_BlitSurface( source, clip, target, &offset );
}

void set_color_key( SDL_Surface* img, int red, int green, int blue )
{
    Uint32 colorkey = SDL_MapRGB( img->format, red, green, blue );
    SDL_SetColorKey( img, SDL_SRCCOLORKEY, colorkey );
}

SDL_Surface* load_img( char* img[100], int mode )
{
    SDL_Surface* temp_img = IMG_Load( img );
    SDL_Surface* final_img = NULL;
    
    if ( temp_img != NULL )
    {
        switch ( mode )
        {
            case 1 : final_img = SDL_DisplayFormat( temp_img ); break;
            case 2 : final_img = SDL_DisplayFormatAlpha( temp_img ); break;
        }
        SDL_FreeSurface( temp_img );
    }
    return final_img;
}

SDL_Surface* render_text( char* font_name[20], int size, char* text[20], int red, int green, int blue )
{
    SDL_Surface* rendered;
    TTF_Font* font = TTF_OpenFont( font_name, size );
    SDL_Color text_color = { red, green, blue };
    rendered = TTF_RenderText_Solid( font, text, text_color );
    TTF_CloseFont( font );
    return rendered;
}

void clean_up()
{
    TTF_Quit();
}
