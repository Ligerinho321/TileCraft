#ifndef WINDOW_PROJECT_UTILS_H_INCLUDE
#define WINDOW_PROJECT_UTILS_H_INCLUDE

#include <utils.h>
#include <window_map.h>
#include <window_menu.h>
#include <window_edit.h>

void removeBlocosMatriz(Window_Project *window_project,char *path_tile);
Tag* check_tag_by_path(Window_Project *window_project,char *path_tag);
int check_tag_tile_path(void *element,char *path);
void select_tag_by_path(Data *data,char *path_tag);
void config_tag_rect(Window_Project *window_project,Settings *setting,int type);
void update_tags(Data *data);
void reload_tag_tile(Window_Project *window_project,SDL_Renderer *renderer,char *path_tag_tile,char *path_tile,Tipo type);
void remove_tag(Window_Project *window_project,Tag *target_tag);
void push_tag(Window_Project *window_project,Settings *setting,SDL_Renderer *renderer,Tag *tag);
void free_tags(Window_Project *window_project);
void draw_modal(Window_Project *window_project,SDL_Renderer *renderer,Settings *setting);
void check_this_is_not_save(Window_Project *window_project);
void save_element(Data *data);
void cancel_exiting_closing(Window_Project *window_project);

#endif