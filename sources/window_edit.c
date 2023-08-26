#include <window_edit.h>

Edit_Undo_Redo_Action* edit_push_stack(Edit_Undo_Redo_Action *stack,char *path_tile,Tipo type){
    Edit_Undo_Redo_Action *new = calloc(1,sizeof(Edit_Undo_Redo_Action));
    new->path_tile = new_string(path_tile);
    new->type = type;
    new->previous = (stack) ? stack : NULL;

    return new;
}

Edit_Undo_Redo_Action* edit_free_stack(Edit_Undo_Redo_Action *stack){
    while(stack){
        Edit_Undo_Redo_Action *buffer_stack = stack->previous;
        free(stack->path_tile);
        free(stack);
        stack = buffer_stack;
    }
    return NULL;
}

void edit_undo_redo_manager(Window_Edit *window_edit,Data *data,Edit_Undo_Redo_Action **stack,Tipo type){
    
    if(type == TYPE_UNDO){
        window_edit->redo = edit_push_stack(window_edit->redo,(*stack)->path_tile,(((*stack)->type == TYPE_ADD) ? TYPE_ERASER : TYPE_ADD));
    }
    else if(type == TYPE_REDO){
        window_edit->updated = false;

        window_edit->undo = edit_push_stack(window_edit->undo,(*stack)->path_tile,(((*stack)->type == TYPE_ADD) ? TYPE_ERASER : TYPE_ADD));
    }

    if((*stack)->type == TYPE_ADD){
        reload_tag_tile(data->window_project,data->renderer,window_edit->path,(*stack)->path_tile,TYPE_APPEND);
        append_tile(&window_edit->tiles,(*stack)->path_tile,data->renderer);
        sort_tiles(window_edit->tiles,&window_edit->size,0);
    }
    else if((*stack)->type == TYPE_ERASER){
        reload_tag_tile(data->window_project,data->renderer,window_edit->path,(*stack)->path_tile,TYPE_REMOVE);
        remove_tile(&window_edit->tiles,(*stack)->path_tile);
        sort_tiles(window_edit->tiles,&window_edit->size,0);
        window_edit->tile_selected = NULL;
    }

    Edit_Undo_Redo_Action *buffer_stack = (*stack)->previous;
    free(*stack);
    *stack = buffer_stack;
}



void widgets_config_rect(Window_Edit *window_edit){

    //confirm
    window_edit->wConfirm.rect.w = window_edit->blocos_rect.w*EDIT_CONFIRM_WIDTH_PERCENT;
    window_edit->wConfirm.rect.h = window_edit->blocos_rect.h*EDIT_CONFIRM_HEIGHT_PERCENT;
    window_edit->wConfirm.rect.x = (window_edit->blocos_rect.x+window_edit->blocos_rect.w*0.5)-window_edit->wConfirm.rect.w*0.5;
    window_edit->wConfirm.rect.y = (window_edit->blocos_rect.y+window_edit->blocos_rect.h*0.5)-window_edit->wConfirm.rect.h*0.5;

    window_edit->wConfirm.textRect.w = window_edit->wConfirm.rect.w;
    window_edit->wConfirm.textRect.h = window_edit->wConfirm.rect.h*0.5;
    window_edit->wConfirm.textRect.x = window_edit->wConfirm.rect.x;
    window_edit->wConfirm.textRect.y = window_edit->wConfirm.rect.y;

    window_edit->wConfirm.cancelar.rect.w = window_edit->wConfirm.rect.w*EDIT_BUTTON_WIDTH_PERCENT;
    window_edit->wConfirm.cancelar.rect.h = window_edit->wConfirm.rect.h*EDIT_BUTTON_HEIGHT_PERCENT;
    window_edit->wConfirm.cancelar.rect.x = (window_edit->wConfirm.rect.x+window_edit->wConfirm.rect.w)-window_edit->wConfirm.cancelar.rect.w-SPACING;
    window_edit->wConfirm.cancelar.rect.y = (window_edit->wConfirm.rect.y+window_edit->wConfirm.rect.h)-window_edit->wConfirm.cancelar.rect.h-SPACING;

    window_edit->wConfirm.confirmar.rect.w = window_edit->wConfirm.cancelar.rect.w;
    window_edit->wConfirm.confirmar.rect.h = window_edit->wConfirm.cancelar.rect.h;
    window_edit->wConfirm.confirmar.rect.x = window_edit->wConfirm.cancelar.rect.x-window_edit->wConfirm.confirmar.rect.w-SPACING;
    window_edit->wConfirm.confirmar.rect.y = window_edit->wConfirm.cancelar.rect.y;

    //widget
    window_edit->widget.rect.x = (window_edit->blocos_rect.x+window_edit->blocos_rect.w)-window_edit->blocos_rect.w*EDIT_WIDGET_WIDTH_PERCENT-EDIT_SPACING;
    window_edit->widget.rect.y = (window_edit->blocos_rect.y+window_edit->blocos_rect.h)-window_edit->blocos_rect.w*EDIT_WIDGET_HEIGHT_PERCENT-EDIT_SPACING;
    window_edit->widget.rect.w = window_edit->blocos_rect.w*EDIT_WIDGET_WIDTH_PERCENT;
    window_edit->widget.rect.h = window_edit->blocos_rect.w*EDIT_WIDGET_HEIGHT_PERCENT;

    window_edit->widget.adicionar.x = window_edit->widget.rect.x;
    window_edit->widget.adicionar.y = window_edit->widget.rect.y;
    window_edit->widget.adicionar.w = window_edit->widget.rect.w;
    window_edit->widget.adicionar.h = window_edit->widget.rect.h*0.5+1;

    window_edit->widget.remove.x = window_edit->widget.rect.x;
    window_edit->widget.remove.y = window_edit->widget.rect.y+window_edit->widget.rect.h*0.5;
    window_edit->widget.remove.w = window_edit->widget.rect.w;
    window_edit->widget.remove.h = window_edit->widget.rect.h*0.5;

    window_edit->widget.rect1.w = window_edit->widget.adicionar.w*0.2;
    window_edit->widget.rect1.h = window_edit->widget.adicionar.h*0.6;
    window_edit->widget.rect1.x = (window_edit->widget.adicionar.x+window_edit->widget.adicionar.w*0.5)-window_edit->widget.rect1.w*0.5;
    window_edit->widget.rect1.y = window_edit->widget.adicionar.y+window_edit->widget.adicionar.h*0.2;

    window_edit->widget.rect2.w = window_edit->widget.adicionar.w*0.6;
    window_edit->widget.rect2.h = window_edit->widget.adicionar.h*0.2;
    window_edit->widget.rect2.x = window_edit->widget.adicionar.x+window_edit->widget.adicionar.w*0.2;
    window_edit->widget.rect2.y = (window_edit->widget.adicionar.y+window_edit->widget.adicionar.h*0.5)-window_edit->widget.rect2.h*0.5;

    window_edit->widget.rect3.w = window_edit->widget.remove.w*0.6;
    window_edit->widget.rect3.h = window_edit->widget.remove.h*0.2;
    window_edit->widget.rect3.x = window_edit->widget.remove.x+window_edit->widget.remove.w*0.2;
    window_edit->widget.rect3.y = (window_edit->widget.remove.y+window_edit->widget.remove.h*0.5)-window_edit->widget.rect3.h*0.5;
}

void widgets_draw(Window_Edit *window_edit,Settings *setting,SDL_Renderer *renderer){

    SDL_Color background_color = setting->background_color;
    SDL_Color background_color_selected = setting->background_color_selected;
    SDL_Color text_color = setting->text_color;
    SDL_Color border_color = setting->border_color;
    SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
    SDL_RenderFillRect(renderer,&window_edit->widget.rect);

    SDL_SetRenderDrawColor(renderer,background_color_selected.r,background_color_selected.g,background_color_selected.b,background_color_selected.a);
    if(window_edit->widget.adiSelected) SDL_RenderFillRect(renderer,&window_edit->widget.adicionar);
    if(window_edit->widget.remSelected) SDL_RenderFillRect(renderer,&window_edit->widget.remove);
    
    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&window_edit->widget.adicionar);
    SDL_RenderFillRect(renderer,&window_edit->widget.rect1);
    SDL_RenderFillRect(renderer,&window_edit->widget.rect2);

    SDL_RenderDrawRect(renderer,&window_edit->widget.remove);
    SDL_RenderFillRect(renderer,&window_edit->widget.rect3);

    window_edit->widget.adiSelected = false;
    window_edit->widget.remSelected = false;

    if(window_edit->wConfirm.visible){
        SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
        SDL_RenderFillRect(renderer,&window_edit->wConfirm.rect);

        SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
        SDL_RenderDrawRect(renderer,&window_edit->wConfirm.rect);

        draw_text(renderer,setting->fontlower,"Sure you want to delete this ?",text_color,window_edit->wConfirm.textRect,true,true);

        draw_button(renderer,&window_edit->wConfirm.cancelar,setting,setting->fontlower,text_color,border_color);
        draw_button(renderer,&window_edit->wConfirm.confirmar,setting,setting->fontlower,text_color,border_color);
    }
}

void config_blocos_rect(Window_Edit *window_edit){

    size_t scroll_bar_size = window_edit->rect_reference.w*EDIT_SCROLL_BAR_SIZE_PERCENT;

    calcSize(&window_edit->vertical_scroll_bar,TYPE_VERTICAL);
    calcSize(&window_edit->horizontal_scroll_bar,TYPE_HORIZONTAL);

    if(window_edit->horizontal_scroll_bar.subRect.w > window_edit->horizontal_scroll_bar.rect.w-4 || window_edit->horizontal_scroll_bar.subRect.w <= 0){
        window_edit->horizontal_scroll_bar.visible = false;
        window_edit->horizontal_scroll_bar.offset = 0;
        window_edit->horizontal_scroll_bar.subRect.x = window_edit->horizontal_scroll_bar.rect.x+SCROLL_BAR_SPACING;
        window_edit->blocos_rect.h = window_edit->rect_reference.h-EDIT_SPACING*2;
        window_edit->vertical_scroll_bar.rect.h = window_edit->blocos_rect.h;
    }
    else{
        window_edit->horizontal_scroll_bar.visible = true;
        window_edit->blocos_rect.h = window_edit->horizontal_scroll_bar.rect.y-window_edit->blocos_rect.y-EDIT_SPACING+1;
        window_edit->vertical_scroll_bar.rect.h = window_edit->blocos_rect.h;
    }

    if(window_edit->vertical_scroll_bar.subRect.h > window_edit->vertical_scroll_bar.rect.h-4 || window_edit->vertical_scroll_bar.subRect.h <= 0){
        window_edit->vertical_scroll_bar.visible = false;
        window_edit->vertical_scroll_bar.offset = 0;
        window_edit->vertical_scroll_bar.subRect.y = window_edit->vertical_scroll_bar.rect.y+SCROLL_BAR_SPACING;
        window_edit->blocos_rect.w = (window_edit->rect_reference.x+window_edit->rect_reference.w)-window_edit->blocos_rect.x-EDIT_SPACING;
        window_edit->horizontal_scroll_bar.rect.w = window_edit->blocos_rect.w;
    }
    else{
        window_edit->vertical_scroll_bar.visible = true;
        window_edit->blocos_rect.w = window_edit->vertical_scroll_bar.rect.x-window_edit->blocos_rect.x-EDIT_SPACING+1;
        window_edit->horizontal_scroll_bar.rect.w = window_edit->blocos_rect.w;
    }

    calcSize(&window_edit->vertical_scroll_bar,TYPE_VERTICAL);
    calcSize(&window_edit->horizontal_scroll_bar,TYPE_HORIZONTAL);
    widgets_config_rect(window_edit);
}

void edit_init(Window_Edit *window_edit,SDL_Renderer *renderer,Tipo type){

    window_edit->tiles = NULL;

    window_edit->scale = 1.0f;
    window_edit->size = 0;
    window_edit->type = type;
    window_edit->updated = true;
    window_edit->number_of_columns = 0;

    window_edit->tile_selected = NULL;

    window_edit->editInfo = calloc(1,sizeof(Edit_Informations));
    edit_info_init(window_edit->editInfo);

    window_edit->widget.adiSelected = false;
    window_edit->widget.remSelected = false;

    window_edit->wConfirm.visible = false;

    window_edit->wConfirm.cancelar.selected = false;
    window_edit->wConfirm.cancelar.text = new_string("No");

    window_edit->wConfirm.confirmar.selected = false;
    window_edit->wConfirm.confirmar.text = new_string("Yes");

    if(window_edit->type == TYPE_AN_IMAGE){
        window_edit->tiles = load_tile_set_based_on_an_image(window_edit->path,renderer,&window_edit->number_of_columns);
    }
    else if(window_edit->type == TYPE_MULTIPLE_IMAGES){
        load_tile_set_based_on_multiple_images(&window_edit->tiles,window_edit->path,renderer);
    }
    sort_tiles(window_edit->tiles,&window_edit->size,0);
}

void edit_config_rect(Window_Edit *window_edit,Settings *setting){

    window_edit->editInfo->rect.x = window_edit->rect_reference.x+EDIT_SPACING;
    window_edit->editInfo->rect.y = window_edit->rect_reference.y+EDIT_SPACING;
    window_edit->editInfo->rect.w = window_edit->rect_reference.w*INFO_WIDTH_PERCENT;
    window_edit->editInfo->rect.h = window_edit->rect_reference.h-EDIT_SPACING*2;

    window_edit->blocos_rect.x = window_edit->editInfo->rect.x+window_edit->editInfo->rect.w+EDIT_SPACING-1;;
    window_edit->blocos_rect.y = window_edit->rect_reference.y+EDIT_SPACING;
    window_edit->blocos_rect.w = (window_edit->rect_reference.x+window_edit->rect_reference.w)-window_edit->blocos_rect.x-EDIT_SPACING;
    window_edit->blocos_rect.h = window_edit->rect_reference.h-EDIT_SPACING*2;

    size_t scroll_bar_size = window_edit->blocos_rect.w*EDIT_SCROLL_BAR_SIZE_PERCENT;

    window_edit->horizontal_scroll_bar.rect.x = window_edit->blocos_rect.x;
    window_edit->horizontal_scroll_bar.rect.y = (window_edit->rect_reference.y+window_edit->rect_reference.h)-scroll_bar_size-EDIT_SPACING;
    window_edit->horizontal_scroll_bar.rect.h = scroll_bar_size;

    window_edit->horizontal_scroll_bar.subRect.x = window_edit->horizontal_scroll_bar.rect.x+SCROLL_BAR_SPACING;
    window_edit->horizontal_scroll_bar.subRect.y = window_edit->horizontal_scroll_bar.rect.y+SCROLL_BAR_SPACING;
    window_edit->horizontal_scroll_bar.subRect.h = window_edit->horizontal_scroll_bar.rect.h-SCROLL_BAR_SPACING*2;


    window_edit->vertical_scroll_bar.rect.x = (window_edit->rect_reference.x+window_edit->rect_reference.w)-scroll_bar_size-EDIT_SPACING;
    window_edit->vertical_scroll_bar.rect.y = window_edit->blocos_rect.y;
    window_edit->vertical_scroll_bar.rect.w = scroll_bar_size;

    window_edit->vertical_scroll_bar.subRect.x = window_edit->vertical_scroll_bar.rect.x+SCROLL_BAR_SPACING;
    window_edit->vertical_scroll_bar.subRect.y = window_edit->vertical_scroll_bar.rect.y+SCROLL_BAR_SPACING;
    window_edit->vertical_scroll_bar.subRect.w = window_edit->vertical_scroll_bar.rect.w-SCROLL_BAR_SPACING*2;

    window_edit->max_width = window_edit->vertical_scroll_bar.rect.x-window_edit->blocos_rect.x-EDIT_SPACING+1;

    edit_info_config_rect(window_edit->editInfo,setting);
}

void edit_run(Window_Edit *window_edit,Data *data){
    edit_input(window_edit,data);
    update_tiles(window_edit->tiles,&window_edit->vertical_scroll_bar,&window_edit->horizontal_scroll_bar,window_edit->scale,window_edit->number_of_columns,window_edit->max_width);
    config_blocos_rect(window_edit);
    ScrollBarUpdate(&window_edit->vertical_scroll_bar,TYPE_VERTICAL,data->mouse);
    ScrollBarUpdate(&window_edit->horizontal_scroll_bar,TYPE_HORIZONTAL,data->mouse);

    edit_draw(window_edit,data->setting,data->renderer);
    edit_info_run(window_edit->editInfo,data->renderer,data->setting,data->mouse,data->event,window_edit->tile_selected,window_edit->size);

    if(!window_edit->undo) window_edit->updated = true;
}

void edit_draw(Window_Edit *window_edit,Settings *setting,SDL_Renderer *renderer){
    SDL_Color border_color = setting->border_color;
    SDL_Color background_color = setting->background_color;

    //draw blocos rect
    SDL_RenderSetViewport(renderer,&window_edit->blocos_rect);
    
    SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
    SDL_RenderFillRect(renderer,NULL);

    Tile *tile = window_edit->tiles;
    while(tile != NULL){

        SDL_Rect rect = {
            tile->rect.x+window_edit->horizontal_scroll_bar.offset,
            tile->rect.y+window_edit->vertical_scroll_bar.offset,
            tile->rect.w,
            tile->rect.h
        };
        SDL_RenderCopy(renderer,tile->texture,NULL,&rect);

        if(window_edit->tile_selected && tile == window_edit->tile_selected){

            SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE,rect.w,rect.h,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);
            SDL_FillRect(surface,NULL,SDL_MapRGBA(surface->format,border_color.r,border_color.g,border_color.b,50));
            
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surface);
            SDL_RenderCopy(renderer,texture,NULL,&rect);
            
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        tile = tile->next;
    }

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,NULL);

    SDL_RenderSetViewport(renderer,NULL);

    draw_scroll_bar(&window_edit->vertical_scroll_bar,setting,renderer);
    draw_scroll_bar(&window_edit->horizontal_scroll_bar,setting,renderer);

    if(window_edit->type == TYPE_MULTIPLE_IMAGES) widgets_draw(window_edit,setting,renderer);
}

void edit_input(Window_Edit *window_edit,Data *data){

    if(!data->window_file && !data->window_create && !data->window_new_tile_set){

        if(data->event->type == SDL_KEYDOWN){
            if(window_edit->undo && data->event->key.keysym.scancode == SDL_SCANCODE_Z && SDL_GetModState() & KMOD_CTRL){
                edit_undo_redo_manager(window_edit,data,&window_edit->undo,TYPE_UNDO);
            }
            else if(window_edit->redo && data->event->key.keysym.scancode == SDL_SCANCODE_Y && SDL_GetModState() & KMOD_CTRL){
                edit_undo_redo_manager(window_edit,data,&window_edit->redo,TYPE_REDO);
            }
        }
        else if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT){

            if(window_edit->type == TYPE_MULTIPLE_IMAGES){

                if(collidePoint(data->mouse,&window_edit->widget.adicionar)){
                    
                    data->window_file = calloc(1,sizeof(Window_File));
                    file_init(data->window_file,MOD_SELECTION_IMG_TILE);
                    file_config_rect(data->window_file,data->setting,data->screen_w,data->screen_h);
                    data->window_file->window_edit = window_edit;

                } 
                else if(collidePoint(data->mouse,&window_edit->widget.remove) && window_edit->tile_selected != NULL){
                    window_edit->wConfirm.visible = true;
                }
                else if(collidePoint(data->mouse,&window_edit->wConfirm.cancelar.rect) && window_edit->wConfirm.visible){
                    window_edit->wConfirm.visible = false;
                }
                else if(collidePoint(data->mouse,&window_edit->wConfirm.confirmar.rect) && window_edit->wConfirm.visible){
                    char *path_tile = new_string(window_edit->tile_selected->path);
                    
                    reload_tag_tile(data->window_project,data->renderer,window_edit->path,path_tile,TYPE_REMOVE);

                    remove_tile(&window_edit->tiles,window_edit->tile_selected->path);

                    sort_tiles(window_edit->tiles,&window_edit->size,0);

                    window_edit->updated = false;
                    window_edit->undo = edit_push_stack(window_edit->undo,path_tile,TYPE_ADD);
                    window_edit->redo = edit_free_stack(window_edit->redo);
                    
                    window_edit->tile_selected = NULL;
                    window_edit->wConfirm.visible = false;
                    free(path_tile);
                }
            }

            if(collidePoint(data->mouse,&window_edit->vertical_scroll_bar.subRect) && window_edit->vertical_scroll_bar.visible){
                window_edit->vertical_scroll_bar.on_bar = true;
                window_edit->vertical_scroll_bar.mouseDiff = data->mouse->pos_y - window_edit->vertical_scroll_bar.subRect.y;
            }
            else if(collidePoint(data->mouse,&window_edit->horizontal_scroll_bar.subRect) && window_edit->horizontal_scroll_bar.visible){
                window_edit->horizontal_scroll_bar.on_bar = true;
                window_edit->horizontal_scroll_bar.mouseDiff = data->mouse->pos_x - window_edit->horizontal_scroll_bar.subRect.x;
            }
            else{
                Tile *tile = window_edit->tiles;
                while(tile != NULL){
                    if(collidePointOffset(data->mouse,&tile->rect,window_edit->horizontal_scroll_bar.offset,window_edit->vertical_scroll_bar.offset,&window_edit->blocos_rect)){
                        window_edit->tile_selected = tile;
                    }
                    tile = tile->next;
                }
            }
        }
        else if(data->event->type == SDL_MOUSEBUTTONUP && data->event->button.button == SDL_BUTTON_LEFT){
            window_edit->vertical_scroll_bar.on_bar = false;
            window_edit->horizontal_scroll_bar.on_bar = false;
        }
        else if(data->event->type == SDL_MOUSEWHEEL && collidePoint(data->mouse,&window_edit->blocos_rect)){
            window_edit->scale += data->event->wheel.preciseY*0.2f;
            if(window_edit->scale > 10.0f){
                window_edit->scale = 10.0f;
            }
            else if(window_edit->scale <= 0){
                window_edit->scale = 0.1f;
            }
        }

        if(collidePoint(data->mouse,&window_edit->vertical_scroll_bar.subRect) && window_edit->vertical_scroll_bar.visible){
            window_edit->vertical_scroll_bar.selected = true;
        }
        else if(collidePoint(data->mouse,&window_edit->horizontal_scroll_bar.subRect) && window_edit->horizontal_scroll_bar.visible){
            window_edit->horizontal_scroll_bar.selected = true;
        }

        if(window_edit->type == TYPE_MULTIPLE_IMAGES){
            if(collidePoint(data->mouse,&window_edit->widget.adicionar)){
                window_edit->widget.adiSelected = true;
            }
            else if(collidePoint(data->mouse,&window_edit->widget.remove)){
                window_edit->widget.remSelected = true;
            }
            else if(collidePoint(data->mouse,&window_edit->wConfirm.cancelar.rect)){
                window_edit->wConfirm.cancelar.selected = true;
            }
            else if(collidePoint(data->mouse,&window_edit->wConfirm.confirmar.rect)){
                window_edit->wConfirm.confirmar.selected = true;
            }
        }
    }
}

void edit_free(Window_Edit *window_edit){

    edit_free_stack(window_edit->undo);
    edit_free_stack(window_edit->redo);

    free_tiles(&window_edit->tiles);
    edit_info_free(&window_edit->editInfo);

    free((window_edit)->path);
    free((window_edit)->wConfirm.cancelar.text);
    free((window_edit)->wConfirm.confirmar.text);

    free(window_edit);
}