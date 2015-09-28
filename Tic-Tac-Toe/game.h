#ifndef GAME
#define GAME

#include "init.h"

//all library included in init.h

extern bool game_load;
extern SDL_Surface* win_message;

void show_gameplay(bool first_load);
void handle_game_event();
void init_game();
void game_clean();
void render_gameplay();

#endif
