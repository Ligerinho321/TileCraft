#ifndef WINDOW_TILES_H_INCLUDE
#define WINDOW_TILES_H_INCLUDE

#include <utils.h>
#include <window_file.h>

#define TILE_HEIGHT_ELEMENT_PERCENT 0.04
#define TILE_TAG_HEIGHT_ELEMENT_PERCENT 0.06
#define TILE_BUTTON_WIDTH_PERCENT 0.3
#define TILE_SPACING 6
#define TILE_TAG_RECT_HEIGHT_PERCENT 0.4
#define BLOCOS_SCROLL_BAR_SIZE_PERCENT 0.06

void tiles_init(Window_Tiles *wTiles);
void tiles_config_rect(Window_Tiles *wTiles,Settings *setting);
void tiles_run(Window_Tiles *wTiles,Window_Map *wMap,Window_Layers *wLayers,Data *data);
void tiles_draw(Window_Tiles *wTiles,Settings *setting,SDL_Renderer *renderer);
void tiles_input(Window_Tiles *wTiles,Window_Map *wMap,Window_Layers *wLayers,Data *data);
void tiles_free(Window_Tiles **wTiles);

void remove_tag_tile(Window_Tiles *wTiles,char *path);

#endif