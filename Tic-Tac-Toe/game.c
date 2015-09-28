//header
#include "game.h"

//all library included in init.h

SDL_Surface* x_piece = NULL;
SDL_Surface* o_piece = NULL;
SDL_Surface* current_piece = NULL;
SDL_Surface* piece_board = NULL;
SDL_Surface* tile_selected = NULL;
SDL_Surface* tile_clicked = NULL;
SDL_Surface* tile_show = NULL;
SDL_Surface* board = NULL;
SDL_Surface* win_message = NULL;
SDL_Surface* gameplay = NULL;
SDL_Rect board_clip[2];
SDL_Rect board_tile[9];
SDL_Rect win_highlight[3];
int fill[9];
const int tile_w = 200, tile_h = 200;
SDL_Rect tile_position;
bool game_load = true;
bool game_first_init = true;
bool game_end = false;

const int WINBOARD[8][3] = {{0,1,2},
                            {3,4,5},
                            {6,7,8},
                            {0,3,6},
                            {1,4,7},
                            {2,5,8},
                            {0,4,8},
                            {2,4,6}};

void init_game()
{
    int i = 0;
    int j = 0;
    int x = 0;
    if (game_first_init)
    {
        //set sprites
        board = SDL_DisplayFormat( screen );
        gameplay = SDL_DisplayFormat( screen );
        piece_board = SDL_DisplayFormat( screen );
        SDL_FillRect( piece_board, &piece_board->clip_rect, SDL_MapRGB( piece_board->format, 255, 255, 255 ) );
        set_color_key( piece_board, 255, 255, 255 );
        x_piece = load_img( "img/x.png", 1 );
        set_color_key( x_piece, 255, 255, 255);
        o_piece = load_img( "img/o.png", 1 );
        set_color_key( o_piece, 255, 255, 255 );
        tile_selected = load_img( "img/tile_selected.png", 2);
        tile_clicked = load_img( "img/tile_clicked.png", 2);
        //end of set sprites
        
        game_end = false;
        
        //set board
        for (i = 0; i<2; i++)
        {
            board_clip[i].x = i * tile_w;
            board_clip[i].y = 0;
            board_clip[i].w = tile_w;
            board_clip[i].h = tile_h;
        }
    }
        
    SDL_Surface* temp_board = load_img( "img/board.png", 2 );
    for ( i=0; i < 3; i++ )
    {
        for ( j = 0; j < 3; j++ )
        {
            if (game_first_init)
            {
                board_tile[x].x = j*tile_w;
                board_tile[x].y = i*tile_h;
                board_tile[x].w = tile_w;
                board_tile[x].h = tile_h;
            }
            if ( (( i%2 == 0 ) && ( j%2 == 0 )) || (( i%2 != 0 ) && ( j%2 != 0 )))
            {
                apply_surface( board_tile[x].x, board_tile[x].y, temp_board, board, &board_clip[0]);
            }
            else if ( (( i%2 != 0 ) && ( j%2 == 0 )) || (( i%2 == 0 ) && ( j%2 != 0 )))
            {
                apply_surface( board_tile[x].x, board_tile[x].y, temp_board, board, &board_clip[1]);
            }
            x++;
        }
    }
    SDL_FreeSurface(temp_board);
    //end of set board
    
    //set board fill
    for ( i = 0; i < 9; i++)
    {
        fill[i] = 0;
    }
    //end of set board fill
    
}

void show_gameplay(bool first_start)
{
    if ( first_start ) init_game();
    SDL_FillRect( gameplay, &gameplay->clip_rect, SDL_MapRGB( gameplay->format, 0, 0, 0 ) );
    apply_surface( 0, 0, board, gameplay, NULL );
    if ( game_end ) {
        int i;
        for ( i = 0; i < 3; i++) {
            apply_surface( win_highlight[i].x, win_highlight[i].y, tile_clicked, gameplay, NULL );
        }
    }
    apply_surface( 0, 0, piece_board, gameplay, NULL );
    if (tile_show != NULL) apply_surface( tile_position.x, tile_position.y, tile_show, gameplay, NULL );
    apply_surface( 0, 0, gameplay, screen, NULL );
}

void render_gameplay()
{
    show_gameplay(false);
}

int check_winner()
{
    int i;
    for(i = 0; i < 8; ++i)
    {
        if ((fill[WINBOARD[i][0]]!=0) &&
            (fill[WINBOARD[i][0]] == fill[WINBOARD[i][1]]) &&
            (fill[WINBOARD[i][1]] == fill[WINBOARD[i][2]])) {
                win_highlight[0] = board_tile[WINBOARD[i][0]];
                win_highlight[1] = board_tile[WINBOARD[i][1]];
                win_highlight[2] = board_tile[WINBOARD[i][2]];
                return fill[WINBOARD[i][0]]; // 1 = player 1 win; 2 = player 2 win
            }
    }
    
    for(i = 0; i < 9; ++i)
    {
        if(fill[i]!=0 && i!=8) continue;
        if(fill[i]!=0 && i==8) return 3; //return 3; //draw
        if(fill[i]==0) return 0;
    }
}

void handle_move( int move_point )
{
    if ( fill[move_point] == 0 )
    {
        if ( turn == 1 )
        {
            current_piece = x_piece;
            fill[move_point] = 1;
            turn++;
        }
        else if ( turn == 2 )
        {
            current_piece = o_piece;
            fill[move_point] = 2;
            turn--;
        }
        apply_surface( tile_position.x, tile_position.y, current_piece, piece_board, NULL );
    }
    switch (check_winner())
    {
        case 1 :
            game_end = true;
            position = 4;
            break;
                
        case 2 :
            game_end = true;
            position = 5;
            break;
                
        case 3 :
            position = 6;
            break;
    }
}

/*
	AI Logic (Based on Michael Dawson's Book, Beginning C++ Game Programming, ISBN: 1-59200-205-6) :
	1. AI looks for its winning move. Checks every tile one by one, and do the move if there is.
	2. AI looks for player's winning move. Checks every tile one by one, and fill up the tile if there is a one-deadly-move from the opponent.
	3. AI fills up center tile if it is still available.
	4. AI fills up one of corner tile randomly, if there is one available.
	5. AI fills up one of side tile randomly, if there is one available.
*/

void com_move()
{
    while ( turn == 2 )
    {
    	//check com winning move
        int i;
        for ( i = 0; i < 9; i++ )
        {
            if ( fill[i] == 0 )
            {
                fill[i] = 2;
                if ( check_winner() == 2 )
                {
                    fill[i] = 0;
                    tile_position = board_tile[i];
                    handle_move( i );
                    break;
                }
                else
                {
                    fill[i] = 0;
                }
            }
        }
        if (turn != 2) break;
        
        //check player winning move
        for ( i = 0; i < 9; i++ )
        {
            if ( fill[i] == 0 )
            {
                fill[i] = 1;
                if ( check_winner() == 1 )
                {
                    fill[i] = 0;
                    tile_position = board_tile[i];
                    handle_move( i );
                    break;
                }
                else
                {
                    fill[i] = 0;
                }
            }
        }
        if (turn != 2) break;
        
        //fill the center tile
        if ( fill[4] == 0 )
        {
            tile_position = board_tile[4];
            handle_move( 4 );
        }
        if (turn != 2) break;
        
        //set up a seed for "random" number (got from rand()) from current time
        srand(time(0));
        int x = -1;
        const int CORNER[4] = {0,2,6,8};
        const int SIDE[4] = {1,3,5,7};
        
        //randomly fill up the corner
        if ( ( fill[0] == 0 ) || ( fill[2] == 0 ) || ( fill[6] == 0 ) || ( fill[8] == 0 ) )
        {
            while ( ( x == -1 ) || ( fill[x] != 0 ) ) x = CORNER[rand()%4];
            tile_position = board_tile[x];
            handle_move( x );
            break;
        }
        
        //randomly fill up the side
        if ( ( fill[1] == 0 ) || ( fill[3] == 0 ) || ( fill[5] == 0 ) || ( fill[7] == 0 ) )
        {
            while ( ( x == -1 ) || ( fill[x] != 0 ) ) x = SIDE[rand()%4];
            tile_position = board_tile[x];
            handle_move( x );
            break;
        }
    }
}


void handle_game_event()
{
    if( e.type == SDL_KEYDOWN )
    {
        if ( e.key.keysym.sym == SDLK_ESCAPE )
        {
            reset = true;
        }
    }
    
    if ( turn == 1 || game_mode == 2)
    {
        if( e.type == SDL_MOUSEMOTION )
        {
            mouse_x = e.motion.x;
            mouse_y = e.motion.y;
            
            int i = 0;
            
            if (    ( mouse_x >= 0 ) &&
                    ( mouse_x <= 600 ) &&
                    ( mouse_y >= 0 ) &&
                    ( mouse_y <= 600 )
                )
            {
                for ( i = 0; i < 9; i++ )
                {
                    if (    ( mouse_x >= board_tile[i].x ) &&
                            ( mouse_x <= board_tile[i].x + board_tile[i].w ) &&
                            ( mouse_y >= board_tile[i].y ) &&
                            ( mouse_y <= board_tile[i].y + board_tile[i].h )
                        )
                    {
                        tile_show = tile_selected;
                        tile_position = board_tile[i];
                    }
                }
            }
            else
            {
                tile_show = NULL;
            }
        }
        
        if ( e.type == SDL_MOUSEBUTTONDOWN )
        {
            if ( e.button.button == SDL_BUTTON_LEFT )
            {
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;
            
                int i = 0;
            
                if (    ( mouse_x >= 0 ) &&
                        ( mouse_x <= 600 ) &&
                        ( mouse_y >= 0 ) &&
                        ( mouse_y <= 600 )
                    )
                {
                    for ( i = 0; i < 9; i++ )
                    {
                        if (    ( mouse_x >= board_tile[i].x ) &&
                                ( mouse_x <= board_tile[i].x + board_tile[i].w ) &&
                                ( mouse_y >= board_tile[i].y ) &&
                                ( mouse_y <= board_tile[i].y + board_tile[i].h )
                            )
                        {
                            tile_show = tile_clicked;
                            tile_position = board_tile[i];
                            handle_move( i );
                        }
                    }
                }
            }
        }
    }
    else if ( turn == 2 ) com_move();
    if ( e.type == SDL_MOUSEBUTTONUP )
    {
        tile_show = NULL;
    }
}

void game_clean()
{
    SDL_FreeSurface(piece_board);
    SDL_FreeSurface(x_piece);
    SDL_FreeSurface(o_piece);
    SDL_FreeSurface(current_piece);
    SDL_FreeSurface(tile_selected);
    SDL_FreeSurface(tile_clicked);
    SDL_FreeSurface(tile_show);
    SDL_FreeSurface(board);
    SDL_FreeSurface(win_message);
    SDL_FreeSurface(gameplay);
}
