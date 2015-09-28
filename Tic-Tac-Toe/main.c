/*
    REMEMBER! Game Sequence
    1. event - Watch for events
    2. logic - Handle events
    3. render - Render the logic
    
    ALWAYS :
    1. Initialize pointers. NULL and Uninitialized/Undefined are different.
    2. Avoid using undefined variables, this could result Undefined Behavior. 
		http://en.wikipedia.org/wiki/Undefined_behavior ; 
		http://en.wikipedia.org/wiki/Uninitialized_variable ;
*/

/*==============================================================================================================================
	Untuk Tugas Akhir Praktikum PDKP 2013, Semester 1, Teknik Elektro, Fakultas Teknik, Universitas Diponegoro.
	Asisten Pembimbing : Munji Hanafi
	Kelompok : C11
	Praktikan :
	1. Muhammad Arief Fatkhurrahman
	2. Maulinda Setiawan
	
	Started on November 19, 2013
	*Almost* finished on November 22, 2013 - Paper ACC-ed
	AI logic fix on November 23, 2013 - Based on Michael Dawson's Book, Beginning C++ Game Programming, ISBN: 1-59200-205-6
	
	Written in C language
	Using SDL 1.2.15 - www.libsdl.org
	Basic tutorial on www.lazyfoo.net - SDL | SDL2 | OpenGL | Game Programming
==============================================================================================================================*/

#include "init.h"
#include "mainmenu.h"
#include "game.h"
#include "credits.h"

//all library are included in init.h

int main(int argc, char *argv[])
{   
    //start SDL & init screen
    if ( init_window( "Tic-Tac-Toe", SCREEN_WIDTH, SCREEN_HEIGHT, 32 ) == false ) return 1;
    
    bool quit = false;
    while ( quit == false )
    {
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) );
        if ( reset == true )
        {
            position = 0;
            menu_level = 0;
            reset = false;
            change_level = false;
            game_load = true;
            show_menu(true);
        }
        
        switch ( position )
        {
            case 0 : //main menu
                //event and logic
                if ( change_level && menu_level == 1 )
                {
                    change_level = false;
                    show_menu(true);
                }
                while ( SDL_PollEvent( &e ) )
                {
                    if (handle_menu_event() == 1) quit = true;
                }
                //render
                render_menu();
                break;
                
            case 1 : //game
                if (game_load)
                {
                    show_gameplay( true );
                    game_load = false;
                }
                
                while ( SDL_PollEvent( &e ) )
                {
                    handle_game_event();
                }
                
                render_gameplay();
                break;
                
            case 2 : //game menu
                if (game_mode == 1)
                {
                    SDL_Surface* text = render_text( "font/lazy.ttf", 40, "First turn? [Y / N]", 0, 255, 0);
                    apply_surface( (screen->w - text->w)/2, (screen->h - text->h)/2, text, screen, NULL);
                    SDL_FreeSurface(text);
                    SDL_Flip(screen);
                    while ( SDL_PollEvent( &e ) )
                    {
                        if ( e.type == SDL_KEYDOWN )
                        {
                            if (e.key.keysym.sym == SDLK_y)
                            {
                                turn = 1;
                                position = 1;
                                break;
                            }
                            if (e.key.keysym.sym == SDLK_n)
                            {
                                turn = 2;
                                position = 1;
                                break;
                            }
                        }
                    }
                            
                }
                break;
                
            case 3 : //credit
                if (!visited)
                {
                    show_credits(true);
                    visited = true;
                }
                
                while ( SDL_PollEvent( &e ) )
                {
                    handle_credits_event();
                }
                render_credits();
                break;
                
            case 4 : //win message
                SDL_Delay(1000);
                if ( game_mode == 1 ) win_message = render_text( "font/lazy.ttf", 50, "YOU WIN!", 0, 255, 0);
                else win_message = render_text( "font/lazy.ttf", 50, "X WINS!", 255, 0, 0);
                apply_surface( ( screen->w - win_message->w )/2, ( screen->h - win_message->h )/2, win_message, screen, NULL);
                SDL_Flip(screen);
                SDL_Delay(1000);
                reset = true;
                break;
                
            case 5 :
                SDL_Delay(1000);
                if ( game_mode == 1 ) win_message = render_text( "font/lazy.ttf", 50, "YOU LOSE!", 255, 0, 0);
                else win_message = render_text( "font/lazy.ttf", 50, "O WINS!", 255, 0, 0);
                apply_surface( ( screen->w - win_message->w )/2, ( screen->h - win_message->h )/2, win_message, screen, NULL);
                SDL_Flip(screen);
                SDL_Delay(1000);
                reset = true;
                break;
                
            case 6 :
                SDL_Delay(1000);
                position = 1;
                game_load = true;
                break;
        }
        
        if ( e.type == SDL_QUIT )
        {
            quit = true;
        }
        
        if ( keystate[ SDLK_RCTRL ] || keystate[ SDLK_LCTRL ] )
        {
            current_fps = 1000000;
        } else current_fps = FPS;
        
        SDL_Delay(1000/current_fps);
        
        SDL_Flip( screen );
        if (quit || click)
        {
            SDL_Delay( 250 );
            click = false;
        }
    }
    
    
    //quit
    game_clean();
    credits_clean();
    menu_clean();
    clean_up();
    SDL_Quit();
    
    return 0;
}
