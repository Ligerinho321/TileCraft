#ifndef WINDOW_NEW_TILE_SET_H_INCLUDE
#define WINDOW_NEW_TILE_SET_H_INCLUDE

#include <utils.h>

void new_tile_init(Window_New_Tile_Set *newTile);
void new_tile_config_rect(Window_New_Tile_Set *newTile,Settings *setting,int screen_w,int screen_h);
void new_tile_draw(Window_New_Tile_Set *newTile,Settings *setting,SDL_Renderer *renderer);
void new_tile_input(Data *data);
void new_tile_run(Data *data);
void new_tile_free(Window_New_Tile_Set **newTile);

#endif