#ifndef WINDOW_EDIT_INFORMATIONS_H_INCLUDE
#define WINDOW_EDIT_INFORMATIONS_H_INCLUDE

#include <utils.h>

#define INFO_HEIGHT_ELEMENT_PERCENT 0.06
#define INFO_SPACING 6

void edit_info_init(Edit_Informations *editInfo);
void edit_info_config_rect(Edit_Informations *editInfo,Settings *setting);
void edit_info_draw(Edit_Informations *editInfo,Settings *setting,SDL_Renderer *renderer,Tile *tile,int size);
void edit_info_run(Edit_Informations *editInfo,SDL_Renderer *renderer,Settings *setting,Mouse *mouse,SDL_Event *event,Tile *tile,int size);
void edit_info_free(Edit_Informations **editInfo);

#endif