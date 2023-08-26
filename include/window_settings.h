#ifndef WINDOW_SETTINGS_H_INCLUDE
#define WINDOW_SETTINGS_H_INCLUDE

#include <utils.h>
#include <switch_color_utils.h>
#include <window_file.h>
#include <file_utils.h>

#define NAME_PERCENT 0.2
#define SWITCH_COLOR_WIDTH_PERCENT 0.2
#define SWITCH_COLOR_HEIGHT_PERCENT 0.2
#define SETTING_BUTTON_WIDTH_PERCENT 0.1
#define SETTING_ELEMENT_HEIGHT_PERCENT 0.05


void setting_init(Window_Settings *window_settings,Settings *setting);
void setting_config_rect(Window_Settings *window_settings,Settings *setting,int screen_w,int screen_h);
void setting_draw(Window_Settings *window_settings,Settings *setting,SDL_Renderer *renderer);
void setting_input(Data *data);
void setting_run(Data *data);
void setting_free(Window_Settings **window_settings);

#endif