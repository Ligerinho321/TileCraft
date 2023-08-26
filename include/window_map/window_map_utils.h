#ifndef WINDOW_MAP_UTILS_H_INCLUDE
#define WINDOW_MAP_UTILS_H_INCLUDE

#include <utils.h>
#include <window_tiles.h>
#include <window_layers.h>
#include <window_map_informations.h>

void remove_tag_camada_from_stack(Map_Undo_Redo_Action **stack,Tag_Camadas *tag_camada);
void remove_from_stack(Map_Undo_Redo_Action **stack,char *path_tile,char *path_tag_tile);
Element* push_element(Element *elements,size_t x,size_t y,Tag_Tiles *tag_tile,Tile *tile);
Map_Undo_Redo_Action* push_stack(Map_Undo_Redo_Action *stack,Element *elements,Tag_Camadas *tag_camada,Tipo type);
Map_Undo_Redo_Action* free_stack(Map_Undo_Redo_Action *stack);
void map_undo_redo_manager(Window_Map *wMap,Map_Undo_Redo_Action **stack,Tipo type);
void clear_buffer_matriz(Window_Map *wMap);
void fill_matriz(Window_Map *wMap);
void create_cross_hair(Window_Map *wMap,SDL_Renderer *renderer);
void create_chuncks(Window_Map *wMap);
void push(SDL_Point *pilha,int x,int y,int *top);
void draw_bloco(Window_Map *wMap,SDL_Renderer *renderer,int x,int y,Tile *tile);
void draw_cross_hair(Window_Map *wMap,SDL_Renderer *renderer,int x,int y);
void bar_draw(Window_Map *wMap,Settings *setting,SDL_Renderer *renderer);

void mod_fill_update(Window_Map *wMap,Data *data);
void mod_fill_shape_update(Window_Map *wMap,Data *data);
void mod_clear_rect_update(Window_Map *wMap,Data *data);

#endif