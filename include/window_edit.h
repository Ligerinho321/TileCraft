#ifndef WINDOW_EDIT_H_INCLUDE
#define WINDOW_EDIT_H_INCLUDE

#include <utils.h>
#include <window_file.h>
#include <window_edit_informations.h>

#define EDIT_SCROLL_BAR_SIZE_PERCENT 0.020
#define EDIT_SPACING 3
#define INFO_WIDTH_PERCENT 0.2
#define EDIT_CONFIRM_WIDTH_PERCENT 0.3
#define EDIT_CONFIRM_HEIGHT_PERCENT 0.15
#define EDIT_WIDGET_WIDTH_PERCENT 0.05
#define EDIT_WIDGET_HEIGHT_PERCENT 0.1
#define EDIT_BUTTON_WIDTH_PERCENT 0.2
#define EDIT_BUTTON_HEIGHT_PERCENT 0.3

void edit_init(Window_Edit *window_edit,SDL_Renderer *renderer,Tipo type);
void edit_config_rect(Window_Edit *window_edit,Settings *setting);
void edit_run(Window_Edit *window_edit,Data *data);
void edit_draw(Window_Edit *window_edit,Settings *setting,SDL_Renderer *renderer);
void edit_input(Window_Edit *window_edit,Data *data);
void edit_free(Window_Edit *window_edit);

Edit_Undo_Redo_Action* edit_push_stack(Edit_Undo_Redo_Action *stack,char *path_tile,Tipo type);
Edit_Undo_Redo_Action* edit_free_stack(Edit_Undo_Redo_Action *stack);
void edit_undo_redo_manager(Window_Edit *window_edit,Data *data,Edit_Undo_Redo_Action **stack,Tipo type);


#endif