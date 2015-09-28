//header
#include "credits.h"
#include "mainmenu.h" //take button from main menu

//all library included in init.h

SDL_Surface* credits_string = NULL;
SDL_Surface* credits_sheet = NULL;
SDL_Surface* show_credits_sheet = NULL;
SDL_Rect credits_clip;
bool visited = false;
int credits_y;
bool button_shown = false;

void init_credits()
{
    credits_sheet = SDL_DisplayFormat(screen);
    if ( SCREEN_WIDTH <= 600 ) credits_string = load_img( "img/credits2.png", 2);
    else credits_string = load_img( "img/credits.png", 2);
    show_credits_sheet = sub_main;
    credits_clip.x = 0;
    credits_clip.y = (2*show_credits_sheet->h/length);
    credits_clip.w = show_credits_sheet->w;   
    credits_clip.h = show_credits_sheet->h/length;
    credits_y = screen->h ;
}

void credits_animation()
{
    if ( credits_y == -(credits_string->h+10) ) credits_y = screen->h;
    if ( credits_y == 0 ) SDL_Delay(1000);
    apply_surface( 0, credits_y--, credits_string, credits_sheet, NULL );
}

void show_credits(bool first_load)
{
    if ( first_load ) init_credits();
    SDL_FillRect( credits_sheet, &credits_sheet->clip_rect, SDL_MapRGB( credits_sheet->format, 0, 0, 0 ) );
    credits_animation();
    if ( button_shown )
    {
        apply_surface( (credits_sheet->w - sub_main->w)/2, (credits_sheet->h - sub_main->h/length), show_credits_sheet, credits_sheet, &credits_clip );
    }
    else
    {
        apply_surface( (credits_sheet->w - sub_main->w)/2, (credits_sheet->h - sub_main->h/length)+75, show_credits_sheet, credits_sheet, &credits_clip );
    }
    apply_surface( 0, 0, credits_sheet, screen, NULL );
}

void handle_credits_event()
{
    if ( e.type == SDL_MOUSEMOTION )
    {
        mouse_x = e.motion.x;
        mouse_y = e.motion.y;
        if (    (mouse_x >= (credits_sheet->w - sub_main->w)/2) &&
                (mouse_x <= (credits_sheet->w - sub_main->w)/2 + show_credits_sheet -> w) &&
                (mouse_y >= (credits_sheet->h - sub_main->h/length)) &&
                (mouse_y <= credits_sheet->h)
            )
        {
            show_credits_sheet = sub_selected;
            button_shown=true;
        }
        else
        {
            show_credits_sheet = sub_main;
            button_shown=false;
        }
    }
    
    if ( e.type == SDL_MOUSEBUTTONDOWN )
    {
        if ( e.button.button == SDL_BUTTON_LEFT )
        {
            if (    (mouse_x >= (credits_sheet->w - sub_main->w)/2) &&
                    (mouse_x <= (credits_sheet->w - sub_main->w)/2 + show_credits_sheet -> w) &&
                    (mouse_y >= (credits_sheet->h - sub_main->h/length)) &&
                    (mouse_y <= credits_sheet->h)
                )
            {
                show_credits_sheet = sub_clicked;
                reset = true;
                visited = false;
                click = true;
            }
        }
    }
    
    if ( e.type == SDL_MOUSEBUTTONUP )
    {
        show_credits_sheet = sub_main;
    }
}

void render_credits()
{
    show_credits(false);
}

void credits_clean()
{
    SDL_FreeSurface(credits_string);
    SDL_FreeSurface(credits_sheet);
    SDL_FreeSurface(show_credits_sheet);
}
