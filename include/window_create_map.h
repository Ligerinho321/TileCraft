#ifndef WINDOW_CREATE_MAP_H_INCLUDE
#define WINDOW_CREATE_MAP_H_INCLUDE

#include <utils.h>
#include <window_project.h>
#include <window_menu.h>

#define CREATE_HEIGHT_ELEMENT_PERCENT 0.08
#define CREATE_WIDTH_ELEMENT_PERCENT 0.30
#define CREATE_BUTTON_WIDTH_PERCENT 0.25
#define CREATE_WIDTH_PERCENT 0.4
#define CREATE_HEIGHT_PERCENT 0.6

void create_init(Window_Create *window_create);
void create_input(Data *data);
void create_config_rect(Window_Create *window_create,Settings *setting,int screen_w,int screen_h);
void create_draw(Window_Create *window_create,Settings *setting,SDL_Renderer *renderer);
void create_run(Data *data);
void create_free(Window_Create **window_create);

#endif //WINDOW_CREATE_MAP_H_INCLUDE