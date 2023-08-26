#ifndef WINDOW_PROJECT_H_INCLUDE
#define WINDOW_PROJECT_H_INCLUDE

#include <window_project_utils.h>

#define PROJECT_BUTTON_WIDTH_PERCENT 0.12
#define PROJECT_BAR_HEIGHT_PERCENT 0.06
#define PROJECT_BAR_SPACING 6
#define PROJECT_BUTTON_QUIT_SIZE 30

#define PROJECT_MODAL_WIDTH_PERCENT 0.3
#define PROJECT_MODAL_HEIGHT_PERCENT 0.15
#define PROJECT_MODAL_BUTTON_WIDTH_PERCENT 0.25
#define PROJECT_MODAL_HEIGHT_ELEMENT_PERCENT 0.3;

void project_init(Window_Project *window_project,Settings *setting,SDL_Renderer *renderer,Tag *tag);
void project_config_rect(Window_Project *window_project,Settings *setting,int screen_w,int screen_h);
void project_run(Data *data);
void project_draw(Window_Project *window_project,Settings *setting,SDL_Renderer *renderer);
void project_input(Data *data);
void project_free(Window_Project **window_project);

#endif