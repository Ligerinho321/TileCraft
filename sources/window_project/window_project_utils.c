#include <window_project_utils.h>

void removeBlocosMatriz(Window_Project *window_project,char *path_tile){
    Tag *tag = window_project->tags;

    while(tag != NULL){
        if(tag->type == TYPE_MAP && ((Window_Map*)tag->element)->wCamadas->tags){
            remove_block_in_layers(((Window_Map*)tag->element)->wCamadas,path_tile,NULL);
        }
        tag = tag->next;
    }
}

Tag* check_tag_by_path(Window_Project *window_project,char *path_tag){
    Tag *tag = window_project->tags;

    while(tag != NULL){
        if(tag->path && !strcmp(tag->path,path_tag)) return tag;
        tag = tag->next;
    }
    return NULL;
}

int check_tag_tile_path(void *element,char *path){

    Tag_Tiles *tag_tiles = ((Window_Map*)element)->wTiles->tag_tiles;

    while(tag_tiles != NULL){
        if(!strcmp(tag_tiles->path,path)) return 1;
        tag_tiles = tag_tiles->next;
    }

    return 0;
}

void select_tag_by_path(Data *data,char *path_tag){
    Tag *tag = check_tag_by_path(data->window_project,path_tag);
    if(tag){
        data->window_project->tag_atual = tag;
    }
    else{
        char type[MAX_STRING];
        memset(type,'\0',MAX_STRING);

        char path_buffer[MAX_STRING];
        memset(path_buffer,'\0',MAX_STRING);
        strncpy(path_buffer,path_tag,MAX_STRING-1);

        char *name = basename(path_buffer);

        FILE *file = fopen(path_tag,"r");
        fgets(type,MAX_STRING-1,file);
        trim(type);
        fclose(file);

        Window_Edit *window_edit = calloc(1,sizeof(Window_Edit));
        window_edit->path = new_string(path_tag);

        edit_init(window_edit,data->renderer,((strstr(type,"tile_set_based_on_an_image")) ? TYPE_AN_IMAGE : TYPE_MULTIPLE_IMAGES));

        Tag *tag = calloc(1,sizeof(Tag));
        tag->name = new_string(name);
        tag->path = new_string(path_tag);
        tag->type = TYPE_TILE_SET;
        tag->element = (void*)window_edit;

        push_tag(data->window_project,data->setting,data->renderer,tag);
    }
}

void config_tag_rect(Window_Project *window_project,Settings *setting,int type){
    Tag *tag = window_project->tags;
    while(tag != NULL){

        get_size_text(setting->fontlower,tag->name,&tag->textRect.w,&tag->textRect.h);
        tag->textRect.w += SPACING;
        
        tag->rect.w = tag->textRect.w+PROJECT_BUTTON_QUIT_SIZE;
        tag->rect.h = window_project->tag_rect.h-PROJECT_BAR_SPACING;
        tag->rect.x = (tag->previous == NULL) ? 0 : tag->previous->rect.x+tag->previous->rect.w+PROJECT_BAR_SPACING;
        tag->rect.y = (window_project->tag_rect.y+window_project->tag_rect.h)-tag->rect.h;
        tag->current_rect = tag->rect;

        tag->textRect.x = tag->rect.x;
        tag->textRect.y = tag->rect.y;
        tag->textRect.h = tag->rect.h;

        tag->button_quit.rect.w = PROJECT_BUTTON_QUIT_SIZE-SPACING;
        tag->button_quit.rect.h = PROJECT_BUTTON_QUIT_SIZE-SPACING;
        tag->button_quit.rect.x = (tag->rect.x+tag->rect.w)-(PROJECT_BUTTON_QUIT_SIZE*0.5)-(tag->button_quit.rect.w*0.5);
        tag->button_quit.rect.y = (tag->rect.y+tag->rect.h*0.5)-tag->button_quit.rect.h*0.5;

        if(tag->type == TYPE_TILE_SET && type == TYPE_GLOBAL){
            ((Window_Edit*)tag->element)->rect_reference = window_project->rect_item;
            edit_config_rect((Window_Edit*)tag->element,setting);
        }
        else if(tag->type == TYPE_MAP && type == TYPE_GLOBAL){
            ((Window_Map*)tag->element)->rect_reference = window_project->rect_item;
            map_config_rect((Window_Map*)tag->element,setting);
        }

        tag = tag->next;
    }
}

void update_tags(Data *data){
    Tag *tag = data->window_project->tags;

    while(tag != NULL){

        if(tag->on){
            tag->current_rect.x = data->mouse->pos_x - tag->mouseDiff;

            if(tag->previous != NULL){
                SDL_Rect previousRect = tag->previous->rect;
                SDL_Rect rect = tag->current_rect;

                int diff = (rect.x+rect.w)-(previousRect.x+previousRect.w);

                if(diff < rect.w*0.5){
                    Tag *previous = tag->previous;

                    if(data->window_project->tags == previous){
                        data->window_project->tags = tag;
                    }

                    if(previous->previous) previous->previous->next = tag;
                    if(tag->next) tag->next->previous = previous;

                    tag->previous = previous->previous;
                    previous->previous = tag;
                    previous->next = tag->next;
                    tag->next = previous;

                    config_tag_rect(data->window_project,data->setting,TYPE_LOCAL);
                }
            }
            if(tag->next != NULL){
                SDL_Rect nextRect = tag->next->rect;
                SDL_Rect rect = tag->current_rect;

                int diff = (nextRect.x+nextRect.w)-(rect.x+rect.w);

                if(diff < rect.w*0.5){

                    Tag *next = tag->next;

                    if(data->window_project->tags == tag){
                        data->window_project->tags = next;
                    }

                    if(tag->previous) tag->previous->next = next;
                    if(next->next) next->next->previous = tag;

                    next->previous = tag->previous;
                    tag->previous = next;
                    tag->next = next->next;
                    next->next = tag;

                    config_tag_rect(data->window_project,data->setting,TYPE_LOCAL);
                }
            }
        }
        
        tag->textRect.x = tag->current_rect.x;
        tag->button_quit.rect.x = (tag->current_rect.x+tag->rect.w)-(30*0.5)-(tag->button_quit.rect.w*0.5);
        tag = tag->next;
    }

    if(data->window_project->tag_atual && data->window_project->tag_atual->type == TYPE_TILE_SET){
        edit_run((Window_Edit*)data->window_project->tag_atual->element,data);
    }
    else if(data->window_project->tag_atual && data->window_project->tag_atual->type == TYPE_MAP){
        map_run((Window_Map*)data->window_project->tag_atual->element,data);
    }
}

void reload_tag_tile(Window_Project *window_project,SDL_Renderer *renderer,char *path_tag_tile,char *path_tile,Tipo type){
    Tag *tag = window_project->tags;
    while(tag != NULL){
        if(tag->type == TYPE_MAP){

            Window_Tiles *wTiles = ((Window_Map*)tag->element)->wTiles;
            Tag_Tiles *tag_tiles = wTiles->tag_tiles;
            
            while(tag_tiles != NULL){
                if(!strcmp(tag_tiles->path,path_tag_tile)){

                    if(type == TYPE_APPEND){
                        
                        append_tile(&tag_tiles->tiles,path_tile,renderer);
                    }
                    else if(type == TYPE_REMOVE){

                        remove_from_stack(&((Window_Map*)tag->element)->undo,path_tile,NULL);
                        remove_from_stack(&((Window_Map*)tag->element)->redo,path_tile,NULL);

                        removeBlocosMatriz(window_project,path_tile);
                        remove_tile(&tag_tiles->tiles,path_tile);
                        
                    }
                    sort_tag_tiles(tag_tiles);
                    
                    tag_tiles->tile_selected = NULL;
                    break;
                }
                tag_tiles = tag_tiles->next;
            }
        }
        tag = tag->next;
    }
}

void remove_tag(Window_Project *window_project,Tag *target_tag){

    Tag *next = target_tag->next;
    Tag *previous = target_tag->previous;

    if(next != NULL){
        next->previous = previous;
    }

    if(previous != NULL){
        previous->next = next;

    }else{
        window_project->tags = next;
    }

    if(target_tag == window_project->tag_atual){
        if(target_tag->previous){
            window_project->tag_atual = target_tag->previous;
        }
        else if(target_tag->next){
            window_project->tag_atual = window_project->tags;
        }
        else{
            window_project->tag_atual = NULL;
            window_project->open = false;
            window_project->tags = NULL;
        }
    }

    if(target_tag->type == TYPE_TILE_SET){
        Tag *aux = window_project->tags;
        while(aux != NULL){
            if(aux->type == TYPE_MAP){
                remove_tag_tile(((Window_Map*)aux->element)->wTiles,((Window_Edit*)target_tag->element)->path);
            }
            aux = aux->next;
        }
        edit_free((Window_Edit*)target_tag->element);
    }
    else{
        map_free((Window_Map*)target_tag->element);
    }

    free(target_tag->path);
    free(target_tag->name);
    free(target_tag);
}

void push_tag(Window_Project *window_project,Settings *setting,SDL_Renderer *renderer,Tag *tag){

    Tag *aux = window_project->tags;

    tag->previous = NULL;
    tag->next = NULL;
    tag->on = false;
    tag->selected = false;
    tag->checked = false;

    tag->button_quit.selected = false;
    tag->button_quit.text = NULL;

    if(tag->type == TYPE_TILE_SET && window_project->tags){
        while(aux != NULL){
            if(aux->type == TYPE_MAP && !check_tag_tile_path(aux->element,tag->path)){
                append_tag_tile(((Window_Map*)aux->element)->wTiles,renderer,tag->path,TYPE_GLOBAL);
            }
            aux = aux->next;
        }
    }
    else if(tag->type == TYPE_MAP && window_project->tags){
        while(aux != NULL){
            if(aux->type == TYPE_TILE_SET && !check_tag_tile_path(tag->element,aux->path)){
                append_tag_tile(((Window_Map*)tag->element)->wTiles,renderer,aux->path,TYPE_GLOBAL);
            }
            aux = aux->next;
        }
    }

    aux = window_project->tags;

    if(window_project->tags == NULL){
        window_project->tags = tag;
    }
    else{
        while(aux->next != NULL){
            aux = aux->next;
        }
        tag->previous = aux;
        tag->next = NULL;

        aux->next = tag;
    }

    window_project->tag_atual = tag;

    config_tag_rect(window_project,setting,TYPE_GLOBAL);
}

void free_tags(Window_Project *window_project){
    Tag *aux;
    Tag *tag = window_project->tags;
    while(tag != NULL){
        aux = tag;
        free(tag->button_quit.text);
        free(tag->name);
        free(tag->path);
        if(tag->type == TYPE_MAP){
            map_free((Window_Map*)tag->element);
        }
        else{
            edit_free((Window_Edit*)tag->element);
        }
        tag = tag->next;
        free(aux);
    }
    window_project->tags = NULL;
}

void draw_modal(Window_Project *window_project,SDL_Renderer *renderer,Settings *setting){
    if(window_project->modal_visible){
        SDL_Color border_color = setting->border_color;
        SDL_Color background_color = setting->background_color;
        SDL_Color text_color = setting->text_color;
            
        SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
        SDL_RenderFillRect(renderer,&window_project->modal);
            
        draw_text(renderer,setting->fontlower,window_project->modal_text,text_color,window_project->modal_text_rect,true,true);
            
        draw_button(renderer,&window_project->modal_cancel,setting,setting->fontlower,text_color,border_color);
        draw_button(renderer,&window_project->modal_discard,setting,setting->fontlower,text_color,border_color);
        draw_button(renderer,&window_project->modal_save,setting,setting->fontlower,text_color,border_color);
            
        SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
        SDL_RenderDrawRect(renderer,&window_project->modal);
    }
}

void check_this_is_not_save(Window_Project *window_project){
    if((window_project->closing || window_project->exiting) && !window_project->modal_visible){
        Tag *tag = window_project->tags;
        while(tag){
            if(!tag->checked && ((tag->type == TYPE_MAP && !((Window_Map*)tag->element)->updated) || (tag->type == TYPE_TILE_SET && !((Window_Edit*)tag->element)->updated))){
                tag->checked = true;
                window_project->modal_visible = true;
                window_project->tag_atual = tag;
                return;
            }
            tag = tag->next;
        }

        window_project->open = false;
    }
}

void save_element(Data *data){
    
    if(data->window_project->tag_atual->type == TYPE_MAP){
                    
        if(!data->window_project->tag_atual->path){
                        
            data->window_file = calloc(1,sizeof(Window_File));
            file_init(data->window_file,MOD_SAVE_MAP);
            file_config_rect(data->window_file,data->setting,data->screen_w,data->screen_h);
        }
        else{
            save_map((Window_Map*)data->window_project->tag_atual->element);
        }

    }
    else if(data->window_project->tag_atual->type == TYPE_TILE_SET && ((Window_Edit*)data->window_project->tag_atual->element)->type == TYPE_MULTIPLE_IMAGES){
                    
        Window_Edit *window_edit = (Window_Edit*)data->window_project->tag_atual->element;
        save_tile_set_based_on_multiple_images(window_edit->tiles,window_edit->path);
        window_edit->updated = true;
    }

}

void cancel_exiting_closing(Window_Project *window_project){
    window_project->exiting = false;
    window_project->closing = false;
    Tag *tag = window_project->tags;
    while(tag){
        tag->checked = false;
        tag = tag->next;
    }
}