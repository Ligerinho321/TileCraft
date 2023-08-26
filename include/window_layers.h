#ifndef WINDOW_LAYERS_H_INCLUDE
#define WINDOW_LAYERS_H_INCLUDE

#include <utils.h>

#define LAYER_SPACING 6
#define LAYER_HEIGHT_ELEMENT_PERCENT 0.075
#define LAYER_BUTTON_WIDTH_PERCENT 0.1
#define LAYER_SIZE_SCROLL_BAR_PERCENT 0.06

void layers_init(Window_Layers *window_layers,Settings *setting,Properties *propriedades);
void layers_config_rect(Window_Layers *window_layers,Settings *setting);
void layers_run(Window_Layers *window_layers,Window_Map *wMap,Data *data);
void layers_draw(Window_Layers *window_layers,Settings *setting,SDL_Renderer *renderer);
void layers_input(Window_Layers *window_layers,Window_Map *wMap,Data *data);
void layers_free(Window_Layers **window_layers);

void remove_block_in_layers(Window_Layers *window_layers,char *path_tile,char *path_tag_tile);
void create_tag_layer(Window_Layers *window_layers,Settings *setting);

#endif