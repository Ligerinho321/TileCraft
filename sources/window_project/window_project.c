#include <window_project.h>

void project_init(Window_Project *window_project,Settings *setting,SDL_Renderer *renderer,Tag *tag){

    window_project->open = true;

    window_project->tags = NULL;
    push_tag(window_project,setting,renderer,tag);

    window_project->not_save_text = new_string("*save");
    window_project->saved_text = new_string("save");
    
    window_project->open_file.selected = false;
    window_project->open_file.text = new_string("open");

    window_project->new_map.selected = false;
    window_project->new_map.text = new_string("new map");

    window_project->new_tile_set.selected = false;
    window_project->new_tile_set.text = new_string("new tile set");

    window_project->export.selected = false;
    window_project->export.text = new_string("export");

    window_project->save.selected = false;
    window_project->save.text = window_project->saved_text;

    window_project->close.selected = false;
    window_project->close.text = new_string("close");

    window_project->quit.selected = false;
    window_project->quit.text = new_string("quit");

    window_project->modal_text = new_string("There are saved changes. want to save now ?");

    window_project->modal_save.selected = false;
    window_project->modal_save.text = new_string("save");

    window_project->modal_discard.selected = false;
    window_project->modal_discard.text = new_string("discard");

    window_project->modal_cancel.selected = false;
    window_project->modal_cancel.text = new_string("cancel");

    window_project->modal_visible = false;
    window_project->closing = false;
    window_project->exiting = false;
}

void project_config_rect(Window_Project *window_project,Settings *setting,int screen_w,int screen_h){

    window_project->rect.x = 0;
    window_project->rect.y = 0;
    window_project->rect.w = screen_w;
    window_project->rect.h = screen_h;

    window_project->modal.w = window_project->rect.w*PROJECT_MODAL_WIDTH_PERCENT;
    window_project->modal.h = window_project->rect.h*PROJECT_MODAL_HEIGHT_PERCENT;
    window_project->modal.x = window_project->rect.w*0.5f - window_project->modal.w*0.5f;
    window_project->modal.y = window_project->rect.h*0.5f - window_project->modal.h*0.5f;

    window_project->modal_text_rect.x = window_project->modal.x;
    window_project->modal_text_rect.y = window_project->modal.y;
    window_project->modal_text_rect.w = window_project->modal.w;
    window_project->modal_text_rect.h = window_project->modal.h*0.5f;

    window_project->modal_cancel.rect.w = window_project->modal.w*PROJECT_MODAL_BUTTON_WIDTH_PERCENT;
    window_project->modal_cancel.rect.h = window_project->modal.h*PROJECT_MODAL_HEIGHT_ELEMENT_PERCENT;
    window_project->modal_cancel.rect.x = (window_project->modal.x+window_project->modal.w)-window_project->modal_cancel.rect.w-SPACING;
    window_project->modal_cancel.rect.y = (window_project->modal.y+window_project->modal.h)-window_project->modal_cancel.rect.h-SPACING;

    window_project->modal_discard.rect.w = window_project->modal.w*PROJECT_MODAL_BUTTON_WIDTH_PERCENT;
    window_project->modal_discard.rect.h = window_project->modal.h*PROJECT_MODAL_HEIGHT_ELEMENT_PERCENT;
    window_project->modal_discard.rect.x = window_project->modal_cancel.rect.x-window_project->modal_discard.rect.w-SPACING;
    window_project->modal_discard.rect.y = window_project->modal_cancel.rect.y;

    window_project->modal_save.rect.w = window_project->modal.w*PROJECT_MODAL_BUTTON_WIDTH_PERCENT;
    window_project->modal_save.rect.h = window_project->modal.h*PROJECT_MODAL_HEIGHT_ELEMENT_PERCENT;
    window_project->modal_save.rect.x = window_project->modal_discard.rect.x-window_project->modal_save.rect.w-SPACING;
    window_project->modal_save.rect.y = window_project->modal_discard.rect.y;

    window_project->bar_rect.x = 0;
    window_project->bar_rect.y = 0;
    window_project->bar_rect.w = window_project->rect.w;
    window_project->bar_rect.h = window_project->rect.h*PROJECT_BAR_HEIGHT_PERCENT;

    window_project->tag_rect.x = 0;
    window_project->tag_rect.y = window_project->bar_rect.y+window_project->bar_rect.h;
    window_project->tag_rect.w = screen_w;
    window_project->tag_rect.h = screen_h*PROJECT_BAR_HEIGHT_PERCENT;

    window_project->rect_item.x = 0;
    window_project->rect_item.y = window_project->tag_rect.y+window_project->tag_rect.h-1;
    window_project->rect_item.w = screen_w;
    window_project->rect_item.h = screen_h-window_project->rect_item.y;

    window_project->new_map.rect.x = PROJECT_BAR_SPACING;
    window_project->new_map.rect.y = window_project->bar_rect.y+PROJECT_BAR_SPACING;
    window_project->new_map.rect.w = window_project->bar_rect.w*PROJECT_BUTTON_WIDTH_PERCENT;
    window_project->new_map.rect.h = window_project->bar_rect.h-PROJECT_BAR_SPACING*2;

    window_project->new_tile_set.rect.x = window_project->new_map.rect.x+window_project->new_map.rect.w+PROJECT_BAR_SPACING;
    window_project->new_tile_set.rect.y = window_project->new_map.rect.y;
    window_project->new_tile_set.rect.w = window_project->new_map.rect.w;
    window_project->new_tile_set.rect.h = window_project->new_map.rect.h;

    window_project->open_file.rect.x = window_project->new_tile_set.rect.x+window_project->new_tile_set.rect.w+PROJECT_BAR_SPACING;
    window_project->open_file.rect.y = window_project->new_tile_set.rect.y;
    window_project->open_file.rect.w = window_project->new_tile_set.rect.w;
    window_project->open_file.rect.h = window_project->new_tile_set.rect.h;

    window_project->export.rect.x = window_project->open_file.rect.x+window_project->open_file.rect.w+PROJECT_BAR_SPACING;
    window_project->export.rect.y = window_project->open_file.rect.y;
    window_project->export.rect.w = window_project->open_file.rect.w;
    window_project->export.rect.h = window_project->open_file.rect.h;

    window_project->save.rect.x = window_project->export.rect.x+window_project->export.rect.w+PROJECT_BAR_SPACING;
    window_project->save.rect.y = window_project->export.rect.y;
    window_project->save.rect.w = window_project->export.rect.w;
    window_project->save.rect.h = window_project->export.rect.h;

    window_project->close.rect.x = window_project->save.rect.x+window_project->save.rect.w+PROJECT_BAR_SPACING;
    window_project->close.rect.y = window_project->save.rect.y;
    window_project->close.rect.w = window_project->save.rect.w;
    window_project->close.rect.h = window_project->save.rect.h;

    window_project->quit.rect.x = window_project->close.rect.x+window_project->close.rect.w+PROJECT_BAR_SPACING;
    window_project->quit.rect.y = window_project->close.rect.y;
    window_project->quit.rect.w = window_project->close.rect.w;
    window_project->quit.rect.h = window_project->close.rect.h;

    config_tag_rect(window_project,setting,TYPE_GLOBAL);
}

void project_run(Data *data){

    if(data->window_project->tag_atual->type == TYPE_MAP && !((Window_Map*)data->window_project->tag_atual->element)->updated){
        data->window_project->save.text = data->window_project->not_save_text;
    }
    else if(data->window_project->tag_atual->type == TYPE_TILE_SET && !((Window_Edit*)data->window_project->tag_atual->element)->updated){
        data->window_project->save.text = data->window_project->not_save_text;
    }
    else{
        data->window_project->save.text = data->window_project->saved_text;
    }

    if(!data->window_file) check_this_is_not_save(data->window_project);

    project_draw(data->window_project,data->setting,data->renderer);
    
    update_tags(data);

    draw_modal(data->window_project,data->renderer,data->setting);

    project_input(data);
    
    if(data->window_project->open == false){
        if(!data->window_project->tag_atual || data->window_project->closing){

            data->window_menu = calloc(1,sizeof(Window_Menu));
            menu_init(data->window_menu);
            menu_config_rect(data->window_menu,data->setting,data->screen_w,data->screen_h);
        }
        project_free(&data->window_project);
    }
}

void project_draw(Window_Project *window_project,Settings *setting,SDL_Renderer *renderer){
    SDL_Color background_color = setting->background_color;
    SDL_Color background_color_selected = setting->background_color_selected;
    SDL_Color border_color = setting->border_color;
    SDL_Color text_color = setting->text_color;
    TTF_Font *font = setting->fontlower;
    SDL_Color off_color = {255,255,255,100};

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&window_project->rect_item);
    SDL_RenderDrawRect(renderer,&window_project->bar_rect);

    //new map
    draw_button(renderer,&window_project->new_map,setting,font,text_color,border_color);
    //new block set
    draw_button(renderer,&window_project->new_tile_set,setting,font,text_color,border_color);
    //open file
    draw_button(renderer,&window_project->open_file,setting,font,text_color,border_color);

    //export
    if(window_project->tag_atual->type == TYPE_MAP){
        draw_button(renderer,&window_project->export,setting,font,text_color,border_color);
    }
    else{
        draw_button(renderer,&window_project->export,setting,font,off_color,off_color);
    }

    //save
    draw_button(renderer,&window_project->save,setting,font,text_color,border_color);
    //close
    draw_button(renderer,&window_project->close,setting,font,text_color,border_color);
    //quit
    draw_button(renderer,&window_project->quit,setting,font,text_color,border_color);

    Tag *tag = window_project->tags;
   
    while(tag != NULL){
        if(tag != window_project->tag_atual){

            if(tag->selected){
                SDL_SetRenderDrawColor(renderer,background_color_selected.r,background_color_selected.g,background_color_selected.b,background_color_selected.a);
                SDL_RenderFillRect(renderer,&tag->current_rect);
            }
            else{
                SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
                SDL_RenderFillRect(renderer,&tag->current_rect);
            }
            
            tag->selected = false;
            SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
            SDL_RenderDrawRect(renderer,&tag->current_rect);
        }
        else{

            SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
            SDL_RenderFillRect(renderer,&tag->current_rect);

            SDL_Rect rect = tag->current_rect;
          
            SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);

            SDL_Point ponto1 = {rect.x,rect.y};
            SDL_Point ponto2 = {rect.x+rect.w,rect.y};
            SDL_RenderDrawLine(renderer,ponto1.x,ponto1.y,ponto2.x,ponto2.y);
            
            SDL_Point ponto3 = {rect.x,rect.y};
            SDL_Point ponto4 = {rect.x,rect.y+rect.h-1};
            SDL_RenderDrawLine(renderer,ponto3.x,ponto3.y,ponto4.x,ponto4.y);
            
            SDL_Point ponto5 = {rect.x+rect.w,rect.y};
            SDL_Point ponto6 = {rect.x+rect.w,rect.y+rect.h-1};
            SDL_RenderDrawLine(renderer,ponto5.x,ponto5.y,ponto6.x,ponto6.y);

        }

        draw_text(renderer,font,tag->name,text_color,tag->textRect,true,true);

        draw_button_quit(renderer,&tag->button_quit,setting);

        tag = tag->next;
    }
}

void project_input(Data *data){

    if(data->window_create || data->window_file || data->window_new_tile_set) return;

    Window_Project *window_project = data->window_project;

    if(data->window_project->modal_visible){

        if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT){
            
            if(collidePoint(data->mouse,&window_project->modal_cancel.rect)){
                cancel_exiting_closing(window_project);
                window_project->modal_visible = false;
            }
            else if(collidePoint(data->mouse,&window_project->modal_discard.rect)){
                
                if(!window_project->closing && !window_project->exiting){
                    remove_tag(window_project,window_project->tag_atual);
                    config_tag_rect(window_project,data->setting,TYPE_LOCAL);
                }

                window_project->modal_visible = false;

            }
            else if(collidePoint(data->mouse,&window_project->modal_save.rect)){

                save_element(data);

                if(!window_project->closing && !window_project->exiting){
                    remove_tag(window_project,window_project->tag_atual);
                    config_tag_rect(window_project,data->setting,TYPE_LOCAL);
                }

                window_project->modal_visible = false;
            }
        }

        if(collidePoint(data->mouse,&window_project->modal_cancel.rect)){
            window_project->modal_cancel.selected = true;
        }
        else if(collidePoint(data->mouse,&window_project->modal_discard.rect)){
            window_project->modal_discard.selected = true;
        }
        else if(collidePoint(data->mouse,&window_project->modal_save.rect)){
            window_project->modal_save.selected = true;
        }

    }
    else if(data->event->type == SDL_QUIT){
        window_project->exiting = true;
    }
    else{

        if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT){
            
            if(collidePoint(data->mouse,&window_project->tag_rect)){
                Tag *tag = window_project->tags;
                while(tag != NULL){
                    if(collidePoint(data->mouse,&tag->current_rect)){
                        
                        if(collidePoint(data->mouse,&tag->button_quit.rect)){

                            if((tag->type == TYPE_MAP && !((Window_Map*)tag->element)->updated) ||
                               (tag->type == TYPE_TILE_SET && !((Window_Edit*)tag->element)->updated)){

                                window_project->tag_atual = tag;
                                window_project->modal_visible = true;
                            }
                            else{
                                remove_tag(window_project,tag);
                                config_tag_rect(window_project,data->setting,TYPE_LOCAL);
                            }

                            break;
                        }
                        else{
                            tag->on = true;
                            window_project->tag_atual = tag;
                            tag->mouseDiff = data->mouse->pos_x - tag->current_rect.x;
                            break;
                        }
                    }
                    tag = tag->next;
                }
            }
            else if(collidePoint(data->mouse,&window_project->export.rect) && window_project->tag_atual->type == TYPE_MAP){
                 
                 Window_Map *window_map = (Window_Map*)window_project->tag_atual->element;
                 if(window_map->wCamadas->tags){
                    if(!window_map->path_export){
                        data->window_file = calloc(1,sizeof(Window_File));
                        file_init(data->window_file,MOD_EXPORT);
                        file_config_rect(data->window_file,data->setting,data->screen_w,data->screen_h);
                    }
                    else{
                        export_map(window_map);
                    }
                 }
            }
            else if(collidePoint(data->mouse,&window_project->open_file.rect)){

                data->window_file = calloc(1,sizeof(Window_File));
                file_init(data->window_file,MOD_SELECTION_FILE);
                file_config_rect(data->window_file,data->setting,data->screen_w,data->screen_h);

            }
            else if(collidePoint(data->mouse,&window_project->save.rect)){
                save_element(data);
            }
            else if(collidePoint(data->mouse,&window_project->close.rect) && !data->window_menu){
                window_project->closing = true;
            }
            else if(collidePoint(data->mouse,&window_project->new_map.rect)){

                data->window_create = calloc(1,sizeof(Window_Create));
                create_init(data->window_create);
                create_config_rect(data->window_create,data->setting,data->screen_w,data->screen_h);
            }
            else if(collidePoint(data->mouse,&window_project->new_tile_set.rect)){

               data->window_new_tile_set = calloc(1,sizeof(Window_New_Tile_Set));
               new_tile_init(data->window_new_tile_set);
               new_tile_config_rect(data->window_new_tile_set,data->setting,data->screen_w,data->screen_h);
               
            }
            else if(collidePoint(data->mouse,&window_project->quit.rect)){
                window_project->exiting = true;
            }
        }
        else if(data->event->type == SDL_MOUSEBUTTONUP && data->event->button.button == SDL_BUTTON_LEFT){
            Tag *current = window_project->tags;
            while(current != NULL){
                current->on = false;
                current->current_rect = current->rect;
                current = current->next;
            }
        }
        else if(data->event->type == SDL_KEYDOWN){
            if(data->event->key.keysym.scancode == SDL_SCANCODE_S && SDL_GetModState() & KMOD_CTRL){
                save_element(data);
            }
        }

        if(collidePoint(data->mouse,&window_project->open_file.rect)){
            window_project->open_file.selected = true;
        }
        else if(collidePoint(data->mouse,&window_project->close.rect)){
            window_project->close.selected = true;
        }
        else if(collidePoint(data->mouse,&window_project->new_map.rect)){
            window_project->new_map.selected = true;
        }
        else if(collidePoint(data->mouse,&window_project->new_tile_set.rect)){
            window_project->new_tile_set.selected = true;
        }
        else if(collidePoint(data->mouse,&window_project->export.rect) && window_project->tag_atual->type == TYPE_MAP){
            window_project->export.selected = true;
        }
        else if(collidePoint(data->mouse,&window_project->save.rect)){
            window_project->save.selected = true;
        }
        else if(collidePoint(data->mouse,&window_project->quit.rect)){
            window_project->quit.selected = true;
        }
        else if(collidePoint(data->mouse,&window_project->tag_rect)){
            Tag *tag = window_project->tags;
            while(tag != NULL){
                if(collidePoint(data->mouse,&tag->current_rect)){
                    tag->selected = true;
                    if(collidePoint(data->mouse,&tag->button_quit.rect)){
                        tag->button_quit.selected = true;
                    }
                }
                tag = tag->next;
            }
        }
    }
}

void project_free(Window_Project **window_project){
    if(*window_project != NULL){

        free((*window_project)->open_file.text);
        free((*window_project)->new_map.text);
        free((*window_project)->new_tile_set.text);
        free((*window_project)->export.text);
        free((*window_project)->close.text);
        free((*window_project)->quit.text);

        free((*window_project)->saved_text);
        free((*window_project)->not_save_text);

        free((*window_project)->modal_text);
        free((*window_project)->modal_save.text);
        free((*window_project)->modal_discard.text);
        free((*window_project)->modal_cancel.text);

        free_tags(*window_project);
        free(*window_project);
        *window_project = NULL;
    }
}