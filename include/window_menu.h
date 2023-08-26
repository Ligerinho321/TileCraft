#ifndef WINDOW_MENU_H_INCLUDE
#define WINDOW_MENU_H_INCLUDE

#include <utils.h>
#include <window_create_map.h>
#include <window_file.h>
#include <window_settings.h>
#include <window_new_tile_set.h>

#define MENU_BUTTON_WIDTH_PERCENT 0.2
#define MENU_HEIGHT_ELEMENT_PERCENT 0.08

void menu_init(Window_Menu *wMenu);
void menu_config_rect(Window_Menu *wMenu,Settings *setting,int screen_w,int screen_h);
void menu_run(Data *data);
void menu_draw(Window_Menu *wMenu,Settings *setting,SDL_Renderer *renderer);
void menu_input(Data *data);
void menu_free(Window_Menu **wMenu);

#endif