#ifndef FILE_UTILS_H_INCLUDE
#define FILE_UTILS_H_INCLUDE

#include "structs.h"
#include "window_map.h"

void set_color(SDL_Color *color,char *name,char *value);
void load_settings(Settings *setting);
void print_color(FILE *file,char *name,SDL_Color color);
void save_settings(Settings *setting);

void save_map(Window_Map *wMap);
void load_map(Window_Map *wMap,Settings *setting,SDL_Window *window,SDL_Renderer *renderer);
void export_map(Window_Map *wMap);

void save_tile_set_based_on_an_image(char *path_file,char *path_image,int tile_width,int tile_height,int spacing,int margin);
void save_tile_set_based_on_multiple_images(Tile *tile,char *path);

void load_tile_set_based_on_multiple_images(Tile **tiles,char *path,SDL_Renderer *renderer);
Tile* load_tile_set_based_on_an_image(char *path,SDL_Renderer *renderer,size_t *number_of_columns);

#endif