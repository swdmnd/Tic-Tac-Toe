//header
#include "mainmenu.h"


//all library included in init.h

SDL_Surface* menu_sheet = NULL;
SDL_Surface* main_sprite = NULL;
SDL_Surface* selected_sprite = NULL;
SDL_Surface* clicked_sprite = NULL;
SDL_Surface* show = NULL;
SDL_Surface* current_main = NULL;
SDL_Surface* current_selected = NULL;
SDL_Surface* current_clicked = NULL;
SDL_Rect menu_clip[3];
SDL_Rect show_clip;
SDL_Rect sheet_offset;
int length = sizeof( menu_clip )/sizeof( menu_clip[0] );
int margin = 50, selected = 0;
int menu_level = 1;
bool change_level = false;
bool initiate = true;

SDL_Surface* sub_main = NULL;
SDL_Surface* sub_selected = NULL;
SDL_Surface* sub_clicked = NULL;

void change_menu_level()
{
    if ( menu_level == 0 )
    {
        menu_level = 1;
    }
    else
    {
        reset = true;
    }
}

bool init_menu()
{
    if ( initiate )
    {
        menu_sheet = SDL_DisplayFormat( screen );      
        main_sprite = load_img( "img/main_menu.png", 2);
        selected_sprite = load_img( "img/selected_menu.png", 2);
        clicked_sprite = load_img( "img/clicked_menu.png", 2);
        sub_main = load_img( "img/sub_main.png", 2);
        sub_selected = load_img( "img/sub_selected.png", 2);
        sub_clicked = load_img( "img/sub_clicked.png", 2);
        initiate = false;
    }
    if ( menu_level == 0 )
    {        
        show = main_sprite;
        current_main = main_sprite;
        current_selected = selected_sprite;
        current_clicked = clicked_sprite;
    }
    else if ( menu_level == 1)
    {        
        show = sub_main;
        current_main = sub_main;
        current_selected = sub_selected;
        current_clicked = sub_clicked;
    }
    
    int i = 0;
    for ( i = 0; i < length; i++ )
    {
        menu_clip[i].x = 0;
        menu_clip[i].y = i * current_main->h/length;
        menu_clip[i].w = current_main->w;   
        menu_clip[i].h = current_main->h/length;  
    }
    show_clip = menu_clip[0];
}

void show_menu(bool first_start)
{
    if ( first_start ) init_menu();
    SDL_FillRect( menu_sheet, &menu_sheet->clip_rect, SDL_MapRGB( menu_sheet->format, 0, 0, 0 ) );
    int i = 0;
    for ( i = 0; i < length ; i++ )
    {
        if (i == selected) apply_surface( 0, i * ( current_main->h/length + margin ), show, menu_sheet, &show_clip );
        else apply_surface( 0, i * ( current_main->h/length + margin ), current_main, menu_sheet, &menu_clip[i] );
    }
    
    sheet_offset.x = 0;
    sheet_offset.y = 0;
    sheet_offset.w = current_main->w;
    sheet_offset.h = current_main->h + ( margin * ( length - 1 ) );
    apply_surface( ( screen->w - current_main->w )/2, ( screen->h - sheet_offset.h )/2, menu_sheet, screen, &sheet_offset );
    
    if (change_level) change_menu_level();
}

int handle_menu_event()
{
       
    if ( e.type == SDL_MOUSEMOTION)
    {
        mouse_x = e.motion.x;
        mouse_y = e.motion.y;
        
        int i = 0;
        
        if (
                ( mouse_x >= ( screen->w - current_main->w )/2 ) && 
                ( mouse_y >= ( screen->h - sheet_offset.h )/2 ) &&
                ( mouse_x <= ( screen->w - current_main->w )/2 + menu_clip[0].w ) && 
                ( mouse_y <= ( screen->h - sheet_offset.h )/2 + (menu_clip[0].h + (length-1)*margin) * length )
            )
        {
            for ( i = 0; i < 3; i++ )
            {
                if (
                        ( mouse_x >= ( screen->w - current_main->w )/2 ) && 
                        ( mouse_y >= ( screen->h - sheet_offset.h )/2 + (i * ( current_main->h/length + margin ) ) ) &&
                        ( mouse_x <= ( screen->w - current_main->w )/2 + menu_clip[i].w ) && 
                        ( mouse_y <= ( screen->h - sheet_offset.h )/2 + (i+1) * menu_clip[i].h + i * margin)
                    )
                {
                    show = current_selected;
                    selected = i;
                    show_clip = menu_clip[selected];
                }
            }
        }
        else
        {
            show = current_main;
            show_clip = menu_clip[selected];
        }
    }
        
    if ( e.type == SDL_MOUSEBUTTONDOWN )
    {
        if ( e.button.button == SDL_BUTTON_LEFT )
        {
            mouse_x = e.motion.x;
            mouse_y = e.motion.y;
            
            int i = 0;
            
            for (i = 0; i < 3; i++)
            {
                if (
                        ( mouse_x >= ( screen->w - current_main->w )/2 ) && 
                        ( mouse_y >= ( screen->h - sheet_offset.h )/2 + (i * ( current_main->h/length + margin ) ) ) &&
                        ( mouse_x <= ( screen->w - current_main->w )/2 + menu_clip[i].w ) && 
                        ( mouse_y <= ( screen->h - sheet_offset.h )/2 + (i+1) * menu_clip[i].h + i * margin)
                    )
                {
                    show = current_clicked;
                    selected = i;
                    show_clip = menu_clip[selected];
                    switch (i)
                    {
                        case 0:
                            if (menu_level == 0) change_level = true;
                            else
                            {
                                position = 2;
                                game_mode = 1;
                            }
                            break;
                            
                        case 1:
                            if (menu_level == 0) position = 3;
                            else
                            {
                                position = 1;
                                game_mode = 2;
                            }
                            break;
                            
                        case 2:
                            if (menu_level == 0) return 1;
                            else reset = true;
                            break;
                    }
                    click = true;
                }
            }
        }
    }
    
    if ( e.type == SDL_MOUSEBUTTONUP )
    {
        show = current_main;
        show_clip = menu_clip[selected];
    }
    return 0;
}

void render_menu()
{
    show_menu(false);
}

void menu_clean()
{
    SDL_FreeSurface(menu_sheet);
    SDL_FreeSurface(main_sprite);
    SDL_FreeSurface(selected_sprite);
    SDL_FreeSurface(clicked_sprite);
    SDL_FreeSurface(current_main);
    SDL_FreeSurface(show);
    SDL_FreeSurface(sub_main);
    SDL_FreeSurface(sub_selected);
    SDL_FreeSurface(sub_clicked);
}
