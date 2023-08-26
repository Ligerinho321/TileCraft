#ifndef UTILS_H_INCLUDE
#define UTILS_H_INCLUDE

#include <structs.h>
#include <surface_utils.h>
#include <string_utils.h>
#include <file_utils.h>

void init_settings(Data *data);

void free_settings(Data *data);

void init_input(Input *input,char *value,Tipo type);

void free_input(Input *input);

SDL_Rect create_rect(int x,int y,int w,int h);

void set_size_rect(SDL_Rect *rect,TTF_Font *font,char *text,int spacing_x,int spacing_y);

void get_size_text(TTF_Font *font,char *text,int *width,int *height);

void calcSize(ScrollBar *scrollBar,int type);


Bool check_tile_by_path(Tile *tiles,char *path_tile);

void append_tag_tile(Window_Tiles *window_tiles,SDL_Renderer *renderer,char *path,Tipo type_tag_tile);

void free_tiles(Tile **blocos);

void sort_tiles(Tile *blocos,size_t *size,int *first_id);

void update_tiles(Tile *tiles,ScrollBar *vertScrollBar,ScrollBar *horiScrollBar,float scale,size_t number_of_columns,int max_width);

Bool remove_tile(Tile **tiles,char *path);

void append_tile(Tile **tiles,char *path,SDL_Renderer *renderer);

Tile* get_tile_in_tag_tile(Tile *tiles,int id,char *path);

Tile* get_tile_in_tag_tiles(Tag_Tiles *tag_tiles,int id,char *path);

Tag_Tiles* get_tag_tile(Tag_Tiles *tag_tiles,Tile *tile);

void sort_tag_tiles(Tag_Tiles *tag_tiles);


void update_input(Input *input,SDL_Event *event,Mouse *mouse,SDL_Rect *trans_rect);

void ScrollBarUpdate(ScrollBar *scrollBar,int type,Mouse *mouse);

int collidePointOffset(Mouse *mouse,SDL_Rect *rect,int offsetX,int offsetY,SDL_Rect *transRect);

Bool collidePointMap(Mouse *mouse,Properties *properties);

int collidePoint(Mouse *mouse,SDL_Rect *rect);

void createMouse(Mouse *mouse,SDL_Renderer *renderer);


void get_pos_in_map(Mouse *mouse,Properties *properties);

void convert_to_screen(int x,int y,SDL_Point *point,Properties *properties,SDL_Rect rect);

void convertIsometric(int x, int y, int *destX, int *destY);

void convertCartisian(float x, float y, float *destX, float *destY);

void world_to_screen(int *x,int *y,float transX,float transY,float scale,SDL_Rect *rect);

void screenToWorld(int screenX,int screenY,float *worldX,float *worldY,float transX,float transY,float scale);

#endif