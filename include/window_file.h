#ifndef WINDOW_FILE_H_INCLUDE
#define WINDOW_FILE_H_INCLUDE

#include <utils.h>
#include <window_menu.h>
#include <window_edit.h>
#include <file_utils.h>

#define FILE_BUTTON_WIDTH_PERCENT 0.15
#define FILE_HEIGHT_ELEMENT_PERCENT 0.06
#define FILE_HEIGHT_DIRECTORY_ELEMENT_PERCENT 0.08
#define FILE_SIZE_SCROLLBAR_PERCENT 0.03
#define FILE_WIDTH_PERCENT 0.6
#define FILE_HEIGHT_PERCENT 0.8

void directUpdate(Window_File *wFile);
void directReset(Window_File *wFile);

void file_init(Window_File *wFile,Modo modo);
void file_config_rect(Window_File *wFile,Settings *setting,int screen_w,int screen_h);
void file_draw(Window_File *wFile,Settings *setting,SDL_Renderer *renderer);
void file_run(Data *data);
void file_input(Data *data);
void file_free(Window_File **wFile);

#endif //FILEEX_H_INCLUDE