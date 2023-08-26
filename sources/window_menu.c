#include <window_menu.h>

void menu_init(Window_Menu *window_menu){

    window_menu->open = true;

    window_menu->tile_editor_text = new_string("TileCraft");

    window_menu->open_file.selected = false;
    window_menu->open_file.text = new_string("Open file");

    window_menu->new_map.selected = false;
    window_menu->new_map.text = new_string("New map");

    window_menu->new_tile_set.selected = false;
    window_menu->new_tile_set.text = new_string("New tile set");

    window_menu->settings.selected = false;
    window_menu->settings.text = new_string("Settings");

    window_menu->quit.selected = false;
    window_menu->quit.text = new_string("Quit");

    window_menu->f11_text = new_string("F11 - FULLSCREEN");
    window_menu->esc_text = new_string("ESC - EXIT FULLSCREEN");
    window_menu->ctrl_z_text = new_string("CTRL+Z - UNDO");
    window_menu->ctrl_y_text = new_string("CTRL+Y - REDO");
    window_menu->ctrl_g_text = new_string("CTRL+G - ON/OFF GRADE");
    window_menu->ctrl_r_text = new_string("CTRL+R - RESET POS/SCALE MAP");
}

void menu_config_rect(Window_Menu *window_menu,Settings *setting,int screen_w,int screen_h){;

    window_menu->rect = create_rect(0,0,screen_w,screen_h);

    get_size_text(setting->font,window_menu->tile_editor_text,&window_menu->tile_editor.w,&window_menu->tile_editor.h);
    window_menu->tile_editor.x = SPACING;
    window_menu->tile_editor.y = SPACING;
    window_menu->tile_editor.h = window_menu->rect.h*MENU_HEIGHT_ELEMENT_PERCENT;

    window_menu->open_file.rect.w = window_menu->rect.w*MENU_BUTTON_WIDTH_PERCENT;
    window_menu->open_file.rect.h = window_menu->rect.h*MENU_HEIGHT_ELEMENT_PERCENT;
    window_menu->open_file.rect.x = SPACING;
    window_menu->open_file.rect.y = window_menu->tile_editor.y+window_menu->tile_editor.h+SPACING;

    window_menu->new_map.rect.w = window_menu->rect.w*MENU_BUTTON_WIDTH_PERCENT;
    window_menu->new_map.rect.h = window_menu->rect.h*MENU_HEIGHT_ELEMENT_PERCENT;
    window_menu->new_map.rect.x = SPACING;
    window_menu->new_map.rect.y = window_menu->open_file.rect.y+window_menu->open_file.rect.h+SPACING;

    window_menu->new_tile_set.rect.w = window_menu->rect.w*MENU_BUTTON_WIDTH_PERCENT;
    window_menu->new_tile_set.rect.h = window_menu->rect.h*MENU_HEIGHT_ELEMENT_PERCENT;
    window_menu->new_tile_set.rect.x = SPACING;
    window_menu->new_tile_set.rect.y = window_menu->new_map.rect.y+window_menu->new_map.rect.h+SPACING;

    window_menu->settings.rect.w = window_menu->rect.w*MENU_BUTTON_WIDTH_PERCENT;
    window_menu->settings.rect.h = window_menu->rect.h*MENU_HEIGHT_ELEMENT_PERCENT;
    window_menu->settings.rect.x = SPACING;
    window_menu->settings.rect.y =  window_menu->new_tile_set.rect.y+window_menu->new_tile_set.rect.h+SPACING;

    window_menu->quit.rect.w = window_menu->rect.w*MENU_BUTTON_WIDTH_PERCENT;
    window_menu->quit.rect.h = window_menu->rect.h*MENU_HEIGHT_ELEMENT_PERCENT;
    window_menu->quit.rect.x = SPACING;
    window_menu->quit.rect.y = window_menu->settings.rect.y+window_menu->settings.rect.h+SPACING;

    get_size_text(setting->font,window_menu->f11_text,&window_menu->f11.w,&window_menu->f11.h);
    get_size_text(setting->font,window_menu->esc_text,&window_menu->esc.w,&window_menu->esc.h);
    get_size_text(setting->font,window_menu->ctrl_z_text,&window_menu->ctrl_z.w,&window_menu->ctrl_z.h);
    get_size_text(setting->font,window_menu->ctrl_y_text,&window_menu->ctrl_y.w,&window_menu->ctrl_y.h);
    get_size_text(setting->font,window_menu->ctrl_g_text,&window_menu->ctrl_g.w,&window_menu->ctrl_g.h);
    get_size_text(setting->font,window_menu->ctrl_r_text,&window_menu->ctrl_r.w,&window_menu->ctrl_r.h);

    window_menu->f11.x = (window_menu->rect.x+window_menu->rect.w)-window_menu->ctrl_r.w-SPACING;
    window_menu->f11.y = window_menu->rect.y+SPACING;

    window_menu->esc.x = window_menu->f11.x;
    window_menu->esc.y = window_menu->f11.y+window_menu->f11.h+SPACING;

    window_menu->ctrl_z.x = window_menu->f11.x;
    window_menu->ctrl_z.y = window_menu->esc.y+window_menu->esc.h+SPACING;

    window_menu->ctrl_y.x = window_menu->f11.x;
    window_menu->ctrl_y.y = window_menu->ctrl_z.y+window_menu->ctrl_z.h+SPACING;

    window_menu->ctrl_g.x = window_menu->f11.x;
    window_menu->ctrl_g.y = window_menu->ctrl_y.y+window_menu->ctrl_y.h+SPACING;

    window_menu->ctrl_r.x = window_menu->f11.x;
    window_menu->ctrl_r.y = window_menu->ctrl_g.y+window_menu->ctrl_g.h+SPACING;

    get_size_text(setting->font,VERSION,&window_menu->version_rect.w,&window_menu->version_rect.h);
    window_menu->version_rect.x = (window_menu->rect.x+window_menu->rect.w)-window_menu->version_rect.w-SPACING;
    window_menu->version_rect.y = (window_menu->rect.y+window_menu->rect.h)-window_menu->version_rect.h-SPACING;
}

void menu_run(Data *data){

    menu_input(data);
    menu_draw(data->window_menu,data->setting,data->renderer);

    if(data->window_menu->open == false){
        menu_free(&data->window_menu);
    }
}

void menu_draw(Window_Menu *window_menu,Settings *setting,SDL_Renderer *renderer){
    SDL_Color text_color = setting->text_color;
    SDL_Color background_color = setting->background_color;
    SDL_Color border_color = setting->border_color;

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&window_menu->rect);

    draw_text(renderer,setting->font,window_menu->tile_editor_text,text_color,window_menu->tile_editor,true,true);

    draw_button(renderer,&window_menu->open_file,setting,setting->font,text_color,border_color);

    draw_button(renderer,&window_menu->new_map,setting,setting->font,text_color,border_color);

    draw_button(renderer,&window_menu->new_tile_set,setting,setting->font,text_color,border_color);

    draw_button(renderer,&window_menu->settings,setting,setting->font,text_color,border_color);

    draw_button(renderer,&window_menu->quit,setting,setting->font,text_color,border_color);

    draw_text(renderer,setting->font,window_menu->f11_text,text_color,window_menu->f11,true,true);

    draw_text(renderer,setting->font,window_menu->esc_text,text_color,window_menu->esc,true,true);

    draw_text(renderer,setting->font,window_menu->ctrl_z_text,text_color,window_menu->ctrl_z,true,true);

    draw_text(renderer,setting->font,window_menu->ctrl_y_text,text_color,window_menu->ctrl_y,true,true);

    draw_text(renderer,setting->font,window_menu->ctrl_g_text,text_color,window_menu->ctrl_g,true,true);

    draw_text(renderer,setting->font,window_menu->ctrl_r_text,text_color,window_menu->ctrl_r,true,true);

    draw_text(renderer,setting->font,VERSION,text_color,window_menu->version_rect,true,true);
}

void menu_input(Data *data){
    if(!data->window_create && !data->window_file && !data->window_settings && !data->window_new_tile_set){

        Window_Menu *window_menu = data->window_menu;

        if(data->event->type == SDL_QUIT){
            window_menu->open = false;
        }
        else if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT){
            if(collidePoint(data->mouse,&window_menu->new_tile_set.rect)){

                data->window_new_tile_set = calloc(1,sizeof(Window_New_Tile_Set));
                new_tile_init(data->window_new_tile_set);
                new_tile_config_rect(data->window_new_tile_set,data->setting,data->screen_w,data->screen_h);

            }
            else if(collidePoint(data->mouse,&window_menu->open_file.rect)){
                
                data->window_file = calloc(1,sizeof(Window_File));
                file_init(data->window_file,MOD_SELECTION_FILE);
                file_config_rect(data->window_file,data->setting,data->screen_w,data->screen_h);

            }
            else if(collidePoint(data->mouse,&window_menu->new_map.rect)){
                
                data->window_create = calloc(1,sizeof(Window_Create));
                create_init(data->window_create);
                create_config_rect(data->window_create,data->setting,data->screen_w,data->screen_h);

            }
            else if(collidePoint(data->mouse,&window_menu->settings.rect)){
                
                data->window_settings = calloc(1,sizeof(Window_Settings));
                setting_init(data->window_settings,data->setting);
                setting_config_rect(data->window_settings,data->setting,data->screen_w,data->screen_h);

            }
            else if(collidePoint(data->mouse,&window_menu->quit.rect)){
                window_menu->open = false;
            }
        }

        if(collidePoint(data->mouse,&window_menu->new_tile_set.rect)){
            window_menu->new_tile_set.selected = true;
        }
        else if(collidePoint(data->mouse,&window_menu->open_file.rect)){
            window_menu->open_file.selected = true;
        }
        else if(collidePoint(data->mouse,&window_menu->new_map.rect)){
            window_menu->new_map.selected = true;
        }
        else if(collidePoint(data->mouse,&window_menu->settings.rect)){
            window_menu->settings.selected = true;
        }
        else if(collidePoint(data->mouse,&window_menu->quit.rect)){
            window_menu->quit.selected = true;
        }
    }
}

void menu_free(Window_Menu **window_menu){
    if(*window_menu != NULL){

        free((*window_menu)->tile_editor_text);
        free((*window_menu)->open_file.text);
        free((*window_menu)->new_map.text);
        free((*window_menu)->new_tile_set.text);
        free((*window_menu)->settings.text);
        free((*window_menu)->quit.text);

        free((*window_menu)->f11_text);
        free((*window_menu)->esc_text);
        free((*window_menu)->ctrl_z_text);
        free((*window_menu)->ctrl_y_text);
        free((*window_menu)->ctrl_g_text);
        free((*window_menu)->ctrl_r_text);

        free(*window_menu);
        *window_menu = NULL;
    }
}