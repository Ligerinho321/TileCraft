#include <window_settings.h>

void setting_init(Window_Settings *window_settings,Settings *setting){

    window_settings->open = true;

    init_switch_color(&window_settings->text_color_switch,&setting->text_color,setting);
    init_switch_color(&window_settings->background_color_switch,&setting->background_color,setting);
    init_switch_color(&window_settings->border_color_switch,&setting->border_color,setting);
    init_switch_color(&window_settings->background_color_map_switch,&setting->background_color_map,setting);
    init_switch_color(&window_settings->selection_color_switch,&setting->background_color_selected,setting);
    init_switch_color(&window_settings->grade_color_switch,&setting->grade_color,setting);
    init_switch_color(&window_settings->quit_color_switch,&setting->quit_color,setting);

    window_settings->settings_text = new_string("Settings");
    window_settings->text_color_text = new_string("Text color:");
    window_settings->background_color_text = new_string("Background color:");
    window_settings->border_color_text = new_string("Border color:");
    window_settings->background_color_map_text = new_string("Background map color:");
    window_settings->selection_color_text = new_string("Selection color:");
    window_settings->grade_color_text = new_string("Grade color");
    window_settings->quit_color_text = new_string("Quit color");

    window_settings->cancel.selected = false;
    window_settings->cancel.text = new_string("cancel");

    window_settings->ok.selected = false;
    window_settings->ok.text = new_string("ok");

    window_settings->apply.selected = false;
    window_settings->apply.text = new_string("apply");
}

void setting_config_rect(Window_Settings *window_settings,Settings *setting,int screen_w,int screen_h){
    //rect
    //----------------------------------------------------------------------------------------
    window_settings->rect = create_rect(0,0,screen_w,screen_h);

    window_settings->cancel.rect.x = (window_settings->rect.x+window_settings->rect.w)-window_settings->rect.w*SETTING_BUTTON_WIDTH_PERCENT-SPACING;
    window_settings->cancel.rect.y = (window_settings->rect.y+window_settings->rect.h)-window_settings->rect.h*SETTING_ELEMENT_HEIGHT_PERCENT-SPACING;
    window_settings->cancel.rect.w = window_settings->rect.w*SETTING_BUTTON_WIDTH_PERCENT;
    window_settings->cancel.rect.h = window_settings->rect.h*SETTING_ELEMENT_HEIGHT_PERCENT;
    
    window_settings->ok.rect.x = window_settings->cancel.rect.x - window_settings->cancel.rect.w - SPACING;
    window_settings->ok.rect.y = window_settings->cancel.rect.y;
    window_settings->ok.rect.w =  window_settings->cancel.rect.w;
    window_settings->ok.rect.h =  window_settings->cancel.rect.h;
    
    window_settings->apply.rect.x = window_settings->ok.rect.x - window_settings->ok.rect.w - SPACING;
    window_settings->apply.rect.y = window_settings->ok.rect.y;
    window_settings->apply.rect.w = window_settings->ok.rect.w;
    window_settings->apply.rect.h = window_settings->ok.rect.h;
    //-----------------------------------------------------------------------------------------

    //settings
    //------------------------------------------------------------------------------------------
    get_size_text(setting->font,window_settings->settings_text,&window_settings->settings.w,NULL);
    window_settings->settings.x = window_settings->rect.x+SPACING;
    window_settings->settings.y = window_settings->rect.y+SPACING;
    window_settings->settings.h = window_settings->rect.h*SETTING_ELEMENT_HEIGHT_PERCENT;
    //-------------------------------------------------------------------------------------------

    //text_color
    //---------------------------------------------------------------------------------------------------
    get_size_text(setting->font,window_settings->text_color_text,&window_settings->text_color.w,NULL);
    window_settings->text_color.x = window_settings->rect.x+SPACING;
    window_settings->text_color.y = window_settings->settings.y+window_settings->settings.h + SPACING;
    window_settings->text_color.h = window_settings->rect.h*SETTING_ELEMENT_HEIGHT_PERCENT;

    window_settings->text_color_switch.rect.x = window_settings->rect.x+SPACING;
    window_settings->text_color_switch.rect.y = window_settings->text_color.y+window_settings->text_color.h + SPACING;
    window_settings->text_color_switch.rect.w = window_settings->rect.w * SWITCH_COLOR_WIDTH_PERCENT;
    window_settings->text_color_switch.rect.h = window_settings->rect.h * SWITCH_COLOR_HEIGHT_PERCENT;

    config_switch_color(&window_settings->text_color_switch,setting);
    //----------------------------------------------------------------------------------------------------

    //background_color
    //----------------------------------------------------------------------------------------------------------------
    get_size_text(setting->font,window_settings->background_color_text,&window_settings->background_color.w,NULL);
    window_settings->background_color.x = window_settings->rect.x+SPACING;
    window_settings->background_color.y = window_settings->text_color_switch.rect.y+window_settings->text_color_switch.rect.h+SPACING;
    window_settings->background_color.h = window_settings->rect.h*SETTING_ELEMENT_HEIGHT_PERCENT;

    window_settings->background_color_switch.rect.x = window_settings->rect.x+SPACING;
    window_settings->background_color_switch.rect.y = window_settings->background_color.y+window_settings->background_color.h+SPACING;
    window_settings->background_color_switch.rect.w = window_settings->rect.w * SWITCH_COLOR_WIDTH_PERCENT;
    window_settings->background_color_switch.rect.h = window_settings->rect.h * SWITCH_COLOR_HEIGHT_PERCENT;

    config_switch_color(&window_settings->background_color_switch,setting);
    //------------------------------------------------------------------------------------------------------------------

    //border_color
    //--------------------------------------------------------------------------------------------------
    get_size_text(setting->font,window_settings->border_color_text,&window_settings->border_color.w,NULL);
    window_settings->border_color.x = window_settings->text_color_switch.rect.x+window_settings->text_color_switch.rect.w+SPACING;
    window_settings->border_color.y = window_settings->text_color.y;
    window_settings->border_color.h = window_settings->rect.h*SETTING_ELEMENT_HEIGHT_PERCENT;

    window_settings->border_color_switch.rect.x = window_settings->border_color.x;
    window_settings->border_color_switch.rect.y = window_settings->border_color.y+window_settings->border_color.h + SPACING;
    window_settings->border_color_switch.rect.w = window_settings->rect.w * SWITCH_COLOR_WIDTH_PERCENT;
    window_settings->border_color_switch.rect.h = window_settings->rect.h * SWITCH_COLOR_HEIGHT_PERCENT;

    config_switch_color(&window_settings->border_color_switch,setting);
    //------------------------------------------------------------------------------------------------------

    //background_color_map
    //---------------------------------------------------------------------------------------------------------------------------
    get_size_text(setting->font,window_settings->background_color_map_text,&window_settings->background_color_map.w,&window_settings->background_color_map.h);
    window_settings->background_color_map.x = window_settings->border_color_switch.rect.x;
    window_settings->background_color_map.y = window_settings->border_color_switch.rect.y+window_settings->border_color_switch.rect.h + SPACING;
    window_settings->background_color_map.h = window_settings->rect.h*SETTING_ELEMENT_HEIGHT_PERCENT;

    window_settings->background_color_map_switch.rect.x = window_settings->background_color_map.x;
    window_settings->background_color_map_switch.rect.y = window_settings->background_color_map.y+window_settings->background_color_map.h + SPACING;
    window_settings->background_color_map_switch.rect.w = window_settings->rect.w * SWITCH_COLOR_WIDTH_PERCENT;
    window_settings->background_color_map_switch.rect.h =  window_settings->rect.h * SWITCH_COLOR_HEIGHT_PERCENT;

    config_switch_color(&window_settings->background_color_map_switch,setting);
    //------------------------------------------------------------------------------------------------------------------------------

    //selection_color
    //--------------------------------------------------------------------------------------------
    get_size_text(setting->font,window_settings->selection_color_text,&window_settings->selection_color.w,NULL);
    window_settings->selection_color.x = window_settings->border_color_switch.rect.x+window_settings->border_color_switch.rect.w+SPACING;
    window_settings->selection_color.y = window_settings->border_color.y;
    window_settings->selection_color.h = window_settings->rect.h*SETTING_ELEMENT_HEIGHT_PERCENT;

    window_settings->selection_color_switch.rect.x = window_settings->selection_color.x;
    window_settings->selection_color_switch.rect.y = window_settings->selection_color.y+window_settings->selection_color.h+SPACING;
    window_settings->selection_color_switch.rect.w = window_settings->rect.w*SWITCH_COLOR_WIDTH_PERCENT;
    window_settings->selection_color_switch.rect.h = window_settings->rect.h*SWITCH_COLOR_HEIGHT_PERCENT;

    config_switch_color(&window_settings->selection_color_switch,setting);
    //--------------------------------------------------------------------------------------------------

    //grade_color
    //---------------------------------------------------------------------------------------------------------------------------------------------
    get_size_text(setting->font,window_settings->grade_color_text,&window_settings->grade_color.w,NULL);
    window_settings->grade_color.x = window_settings->selection_color.x;
    window_settings->grade_color.y = window_settings->selection_color_switch.rect.y+window_settings->selection_color_switch.rect.h+SPACING;
    window_settings->grade_color.h = window_settings->rect.h*SETTING_ELEMENT_HEIGHT_PERCENT;

    window_settings->grade_color_switch.rect.x = window_settings->grade_color.x;
    window_settings->grade_color_switch.rect.y = window_settings->grade_color.y+window_settings->grade_color.h+SPACING;
    window_settings->grade_color_switch.rect.w = window_settings->rect.w*SWITCH_COLOR_WIDTH_PERCENT;
    window_settings->grade_color_switch.rect.h = window_settings->rect.h*SWITCH_COLOR_HEIGHT_PERCENT;

    config_switch_color(&window_settings->grade_color_switch,setting);
    //-----------------------------------------------------------------------------------------------------------------------------------------------

    //quit_color
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    get_size_text(setting->font,window_settings->quit_color_text,&window_settings->quit_color.w,NULL);
    window_settings->quit_color.x = window_settings->selection_color_switch.rect.x+window_settings->selection_color_switch.rect.w+SPACING;
    window_settings->quit_color.y = window_settings->selection_color.y;
    window_settings->quit_color.h = window_settings->rect.h*SETTING_ELEMENT_HEIGHT_PERCENT;

    window_settings->quit_color_switch.rect.x = window_settings->quit_color.x;
    window_settings->quit_color_switch.rect.y = window_settings->quit_color.y+window_settings->quit_color.h+SPACING;
    window_settings->quit_color_switch.rect.w = window_settings->rect.w*SWITCH_COLOR_WIDTH_PERCENT;
    window_settings->quit_color_switch.rect.h = window_settings->rect.h*SWITCH_COLOR_HEIGHT_PERCENT;

    config_switch_color(&window_settings->quit_color_switch,setting);
    //------------------------------------------------------------------------------------------------------------------------------------------------------

}

void setting_draw(Window_Settings *window_settings,Settings *setting,SDL_Renderer *renderer){
    SDL_Color text_color = setting->text_color;
    SDL_Color border_color = setting->border_color;
    SDL_Color background_color = setting->background_color;

    SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
    SDL_RenderFillRect(renderer,&window_settings->rect);

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&window_settings->rect);

    //titulo
    draw_text(renderer,setting->font,window_settings->settings_text,text_color,window_settings->settings,true,true);

    //text_color
    draw_text(renderer,setting->font,window_settings->text_color_text,text_color,window_settings->text_color,true,true);
    draw_switch_color(&window_settings->text_color_switch,renderer,setting);

    //background_color
    draw_text(renderer,setting->font,window_settings->background_color_text,text_color,window_settings->background_color,true,true);
    draw_switch_color(&window_settings->background_color_switch,renderer,setting);

    //border_color
    draw_text(renderer,setting->font,window_settings->border_color_text,text_color,window_settings->border_color,true,true);
    draw_switch_color(&window_settings->border_color_switch,renderer,setting);

    //background_color_map
    draw_text(renderer,setting->font,window_settings->background_color_map_text,text_color,window_settings->background_color_map,true,true);
    draw_switch_color(&window_settings->background_color_map_switch,renderer,setting);

    //selection_color
    draw_text(renderer,setting->font,window_settings->selection_color_text,text_color,window_settings->selection_color,true,true);
    draw_switch_color(&window_settings->selection_color_switch,renderer,setting);

    //grade_color
    draw_text(renderer,setting->font,window_settings->grade_color_text,text_color,window_settings->grade_color,true,true);
    draw_switch_color(&window_settings->grade_color_switch,renderer,setting);

    //quit_color
    draw_text(renderer,setting->font,window_settings->quit_color_text,text_color,window_settings->quit_color,true,true);
    draw_switch_color(&window_settings->quit_color_switch,renderer,setting);

    draw_button(renderer,&window_settings->cancel,setting,setting->font,text_color,border_color);
    draw_button(renderer,&window_settings->ok,setting,setting->font,text_color,border_color);
    draw_button(renderer,&window_settings->apply,setting,setting->font,text_color,border_color);
}

void setting_input(Data *data){

    if(!data->window_file){

        Window_Settings *window_settings = data->window_settings;

        if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT){
            if(collidePointOffset(data->mouse,&window_settings->cancel.rect,0,0,&window_settings->rect)){
                load_settings(data->setting);
                window_settings->open = false;
            }
            else if(collidePointOffset(data->mouse,&window_settings->ok.rect,0,0,&window_settings->rect)){
                save_settings(data->setting);
                window_settings->open = false;
            }
            else if(collidePointOffset(data->mouse,&window_settings->apply.rect,0,0,&window_settings->rect)){
                save_settings(data->setting);
            }
        }
        
        if(collidePointOffset(data->mouse,&window_settings->cancel.rect,0,0,&window_settings->rect)){
            window_settings->cancel.selected = true;
        }
        else if(collidePointOffset(data->mouse,&window_settings->ok.rect,0,0,&window_settings->rect)){
            window_settings->ok.selected = true;
        }
        else if(collidePointOffset(data->mouse,&window_settings->apply.rect,0,0,&window_settings->rect)){
            window_settings->apply.selected = true;
        }
    }
}

void setting_run(Data *data){

    input_switch_color(&data->window_settings->text_color_switch,data->event,data->mouse,data->setting,&data->window_settings->rect);
    input_switch_color(&data->window_settings->background_color_switch,data->event,data->mouse,data->setting,&data->window_settings->rect);
    input_switch_color(&data->window_settings->border_color_switch,data->event,data->mouse,data->setting,&data->window_settings->rect);
    input_switch_color(&data->window_settings->background_color_map_switch,data->event,data->mouse,data->setting,&data->window_settings->rect);
    input_switch_color(&data->window_settings->selection_color_switch,data->event,data->mouse,data->setting,&data->window_settings->rect);
    input_switch_color(&data->window_settings->quit_color_switch,data->event,data->mouse,data->setting,&data->window_settings->rect);
    input_switch_color(&data->window_settings->grade_color_switch,data->event,data->mouse,data->setting,&data->window_settings->rect);
    
    setting_input(data);
    setting_draw(data->window_settings,data->setting,data->renderer);

    if(data->window_settings->open == false){
        setting_free(&data->window_settings);
    }
}

void setting_free(Window_Settings **window_settings){
    if(*window_settings != NULL){

        free((*window_settings)->cancel.text);
        free((*window_settings)->ok.text);
        free((*window_settings)->apply.text);
        free((*window_settings)->settings_text);
        free((*window_settings)->text_color_text);
        free((*window_settings)->background_color_text);
        free((*window_settings)->border_color_text);
        free((*window_settings)->background_color_map_text);
        free((*window_settings)->selection_color_text);
        free((*window_settings)->grade_color_text);
        free((*window_settings)->quit_color_text);

        free_switch_color(&(*window_settings)->text_color_switch);
        free_switch_color(&(*window_settings)->background_color_switch);
        free_switch_color(&(*window_settings)->border_color_switch);
        free_switch_color(&(*window_settings)->background_color_map_switch);
        free_switch_color(&(*window_settings)->selection_color_switch);
        free_switch_color(&(*window_settings)->grade_color_switch);
        free_switch_color(&(*window_settings)->quit_color_switch);

        free(*window_settings);

        *window_settings = NULL;
    }
}