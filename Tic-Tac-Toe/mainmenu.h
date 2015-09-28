#ifndef MAINMENU
#define MAINMENU

//header
#include "init.h"

//all library included in init.h

extern int menu_level;
extern int length;
extern bool change_level;
extern SDL_Surface* sub_main;
extern SDL_Surface* sub_selected;
extern SDL_Surface* sub_clicked;
extern SDL_Rect menu_clip[3];

void render_menu();
void menu_clean();

void show_menu(bool first_start);
int handle_menu_event();

#endif
