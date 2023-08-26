#ifndef WINDOW_MAP_H_INCLUDE
#define WINDOW_MAP_H_INCLUDE

#include <window_map_utils.h>

#define MAP_SPACING 3
#define MAP_SUB_ELEMENTS_WIDTH 0.2
#define WIDTH_MAP_PERCENT 0.6
#define MAP_HEIGHT_ELEMENT_PERCENT 0.07
#define BAR_WIDTH_BUTTON_PERCENT 0.16
#define BAR_SPACING 6

void map_init(Window_Map *window_map,Settings *setting,SDL_Renderer *renderer,Properties *properties);
void map_config_rect(Window_Map *window_map,Settings *setting);
void map_run(Window_Map *window_map,Data *data);
void map_draw(Window_Map *window_map,Data *data);
void map_input(Window_Map *window_map,Data *data);
void map_free(Window_Map *window_map);

#endif