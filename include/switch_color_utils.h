#ifndef SWITCH_COLOR_UTILS_H_INCLUDE
#define SWITCH_COLOR_UTILS_H_INCLUDE

#include <utils.h>

void init_switch_color(Switch_Color *switch_color,SDL_Color *buffer_color,Settings *setting);
void init_switch_uint8_t(Switch_Uint8_t *switch_uint8_t,char *name,uint8_t value,Settings *setting);

void config_switch_color(Switch_Color *switch_color,Settings *setting);
void config_switch_value_rect(Switch_Color *switch_color,Settings *setting);
void config_switch_sigla_rect(Switch_Color *switch_color,Settings *setting);
void config_switch_interruptor(Switch_Color *switch_color,Switch_Uint8_t *switch_uint8_t);

void set_pos_interruptor(Switch_Color *switch_color,Switch_Uint8_t *switch_uint8_t);
void set_value_switch(Switch_Uint8_t *switch_uint8_t,Settings *setting);

void input_switch_color(Switch_Color *switch_color,SDL_Event *event,Mouse *mouse,Settings *setting,SDL_Rect *rect_offset);
void input_switch_uint8_t(Switch_Uint8_t *switch_uint8_t,Mouse *mouse,Settings *setting);

void draw_switch_uint8_t(Switch_Uint8_t *switch_uint8_t,SDL_Renderer *renderer,Settings *setting);
void draw_switch_color(Switch_Color *switch_color,SDL_Renderer *renderer,Settings *setting);

void free_switch_color(Switch_Color *switch_color);

#endif