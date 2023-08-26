#ifndef WINDOW_MAP_INFORMATIONS_H_INCLUDE
#define WINDOW_MAP_INFORMATIONS_H_INCLUDE

#include <utils.h>
#include <switch_color_utils.h>

#define PROPRIEDADE_SPACING 6
#define PROPRIEDADE_HEIGHT_ELEMENT_PERCENT 0.07

void map_informations_init(Map_Informations *mapInfo,Properties *properties);
void map_informations_config_rect(Map_Informations *mapInfo,Settings *setting);
void map_informations_run(Map_Informations *mapInfo,Settings *setting,SDL_Renderer *renderer,Mouse *mouse);
void map_informations_draw(Map_Informations *mapInfo,Settings *setting,SDL_Renderer *renderer,Mouse *mouse);
void map_informations_free(Map_Informations **mapInfo);

#endif