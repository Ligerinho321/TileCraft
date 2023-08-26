#include <window_tiles.h>

void remove_tag_tile(Window_Tiles *wTiles,char *path){

    Tag_Tiles *tag_tile = wTiles->tag_tiles;
    Tag_Tiles *anterior = NULL;

    while(tag_tile != NULL){
        if(!strcmp(tag_tile->path,path)){
            break;
        }
        anterior = tag_tile;
        tag_tile = tag_tile->next;
    }

    if(tag_tile != NULL && tag_tile->type == TYPE_GLOBAL){
        if(anterior == NULL){
            wTiles->tag_tiles = tag_tile->next;
        }else{
            anterior->next = tag_tile->next;
        }

        if(!strcmp(tag_tile->path,wTiles->tag_selected->path)){
            if(anterior){
                wTiles->tag_selected = anterior;
            }
            else if(tag_tile->next){
                wTiles->tag_selected = tag_tile->next;
            }
            else{
                wTiles->tag_selected = NULL;
            }
        }
        free(tag_tile->name);
        free(tag_tile->path);
        free_tiles(&tag_tile->tiles);
        free(tag_tile);
    }
}

void free_tag_tiles(Window_Tiles *wTiles){
    Tag_Tiles *tag_tiles = wTiles->tag_tiles;
    Tag_Tiles *aux;

    while(tag_tiles != NULL){
        aux = tag_tiles->next;

        free_tiles(&tag_tiles->tiles);
        tag_tiles->tiles = NULL;

        free(tag_tiles->name);
        free(tag_tiles->path);

        free(tag_tiles);
        tag_tiles = aux;
    }
    wTiles->tag_tiles = NULL;
}

void config_tags_rect(Window_Tiles *wTiles,Settings *setting){
    Tag_Tiles *tag_tile = wTiles->tag_tiles;
    Tag_Tiles *previous = NULL;

    wTiles->horizontal_scroll_bar_tags.lenght = 0;

    while(tag_tile){
        tag_tile->rect.x = 0;
        tag_tile->rect.y = (previous) ? previous->rect.y+previous->rect.h : 0;
        tag_tile->rect.w = wTiles->tags_rect_reference.w;
        tag_tile->rect.h = wTiles->rect.h*TILE_TAG_HEIGHT_ELEMENT_PERCENT;

        int width;
        get_size_text(setting->fontlower,tag_tile->name,&width,NULL);
        wTiles->horizontal_scroll_bar_tags.lenght = (width+4 > wTiles->horizontal_scroll_bar_tags.lenght) ? width+4 : wTiles->horizontal_scroll_bar_tags.lenght;

        previous = tag_tile;
        tag_tile = tag_tile->next;
    }

    wTiles->vertical_scroll_bar_tags.lenght = (previous) ? previous->rect.y+previous->rect.h : 0;

    calcSize(&wTiles->vertical_scroll_bar_tags,TYPE_VERTICAL);
    calcSize(&wTiles->horizontal_scroll_bar_tags,TYPE_HORIZONTAL);

    if(wTiles->horizontal_scroll_bar_tags.subRect.w > wTiles->horizontal_scroll_bar_tags.rect.w-4 || wTiles->horizontal_scroll_bar_tags.subRect.w <= 0){
        wTiles->horizontal_scroll_bar_tags.visible = false;
        wTiles->horizontal_scroll_bar_tags.offset = 0;
        wTiles->horizontal_scroll_bar_tags.subRect.x = wTiles->horizontal_scroll_bar_tags.rect.x+SCROLL_BAR_SPACING;
        wTiles->tags_rect.h = wTiles->tags_rect_reference.h;
        wTiles->vertical_scroll_bar_tags.rect.h = wTiles->tags_rect.h;
    }
    else{
        wTiles->horizontal_scroll_bar_tags.visible = true;
        wTiles->tags_rect.h = wTiles->horizontal_scroll_bar_tags.rect.y-wTiles->tags_rect.y-1;
        wTiles->vertical_scroll_bar_tags.rect.h = wTiles->tags_rect.h;

    }

    if(wTiles->vertical_scroll_bar_tags.subRect.h > wTiles->vertical_scroll_bar_tags.rect.h-4 || wTiles->vertical_scroll_bar_tags.subRect.h <= 0){
        wTiles->vertical_scroll_bar_tags.visible = false;
        wTiles->vertical_scroll_bar_tags.offset = 0;
        wTiles->vertical_scroll_bar_tags.subRect.y = wTiles->vertical_scroll_bar_tags.rect.y+SCROLL_BAR_SPACING;
        wTiles->tags_rect.w = wTiles->tags_rect_reference.w;
        wTiles->horizontal_scroll_bar_tags.rect.w = wTiles->tags_rect_reference.w;
    }
    else{
        wTiles->vertical_scroll_bar_tags.visible = true;
        wTiles->tags_rect.w = wTiles->vertical_scroll_bar_tags.rect.x-wTiles->tags_rect.x-1;
        wTiles->horizontal_scroll_bar_tags.rect.w = wTiles->tags_rect.w;
    }

    calcSize(&wTiles->vertical_scroll_bar_tags,TYPE_VERTICAL);
    calcSize(&wTiles->horizontal_scroll_bar_tags,TYPE_HORIZONTAL);
}

void config_tiles_rect(Window_Tiles *wTiles){
    calcSize(&wTiles->vertical_scroll_bar_tiles,TYPE_VERTICAL);
    calcSize(&wTiles->horizontal_scroll_bar_tiles,TYPE_HORIZONTAL);

    if(wTiles->horizontal_scroll_bar_tiles.subRect.w > wTiles->horizontal_scroll_bar_tiles.rect.w-4 || wTiles->horizontal_scroll_bar_tiles.subRect.w <= 0){
        wTiles->horizontal_scroll_bar_tiles.visible = false;
        wTiles->horizontal_scroll_bar_tiles.offset = 0;
        wTiles->horizontal_scroll_bar_tiles.subRect.x = wTiles->horizontal_scroll_bar_tiles.rect.x+SCROLL_BAR_SPACING;
        wTiles->tiles_rect.h = wTiles->tiles_rect_reference.h;
        wTiles->vertical_scroll_bar_tiles.rect.h = wTiles->tiles_rect_reference.h;
    }
    else{
        wTiles->horizontal_scroll_bar_tiles.visible = true;
        wTiles->tiles_rect.h = wTiles->horizontal_scroll_bar_tiles.rect.y-wTiles->tiles_rect.y-1;
        wTiles->vertical_scroll_bar_tiles.rect.h = wTiles->tiles_rect.h;
    }

    if(wTiles->vertical_scroll_bar_tiles.subRect.h > wTiles->vertical_scroll_bar_tiles.rect.h-4 || wTiles->vertical_scroll_bar_tiles.subRect.h <= 0){
        wTiles->vertical_scroll_bar_tiles.visible = false;
        wTiles->vertical_scroll_bar_tiles.offset = 0;
        wTiles->vertical_scroll_bar_tiles.subRect.y = wTiles->vertical_scroll_bar_tiles.rect.y+SCROLL_BAR_SPACING;
        wTiles->tiles_rect.w = wTiles->tiles_rect_reference.w;
        wTiles->horizontal_scroll_bar_tiles.rect.w = wTiles->tiles_rect_reference.w;
        wTiles->tags_rect.w = wTiles->tiles_rect.w;
    }
    else{
        wTiles->vertical_scroll_bar_tiles.visible = true;
        wTiles->tiles_rect.w =  wTiles->vertical_scroll_bar_tiles.rect.x-wTiles->tiles_rect.x-1;
        wTiles->horizontal_scroll_bar_tiles.rect.w = wTiles->tiles_rect.w;
        wTiles->tags_rect.w = wTiles->tiles_rect.w;
    }

    calcSize(&wTiles->vertical_scroll_bar_tiles,TYPE_VERTICAL);
    calcSize(&wTiles->horizontal_scroll_bar_tiles,TYPE_HORIZONTAL);
}

void tiles_init(Window_Tiles *wTiles){
    wTiles->tag_tiles = NULL;
    wTiles->tag_selected = NULL;

    wTiles->titulo_text = new_string("Tiles");

    wTiles->edit.selected = false;
    wTiles->edit.text = new_string("edit");

    wTiles->new_tile_set.selected = false;
    wTiles->new_tile_set.text = new_string("new tile set");

    wTiles->delete.selected = false;
    wTiles->delete.text = new_string("delete");

    wTiles->vertical_scroll_bar_tiles.on_bar = false;
    wTiles->vertical_scroll_bar_tiles.mouseDiff = 0;
    wTiles->vertical_scroll_bar_tiles.selected = false;
    wTiles->vertical_scroll_bar_tiles.visible = false;

    wTiles->horizontal_scroll_bar_tiles.on_bar = false;
    wTiles->horizontal_scroll_bar_tiles.mouseDiff = 0;
    wTiles->horizontal_scroll_bar_tiles.selected = false;
    wTiles->vertical_scroll_bar_tiles.visible = false;

    wTiles->vertical_scroll_bar_tags.on_bar = false;
    wTiles->vertical_scroll_bar_tags.mouseDiff = 0;
    wTiles->vertical_scroll_bar_tags.selected = false;
    wTiles->vertical_scroll_bar_tags.visible = false;

    wTiles->horizontal_scroll_bar_tags.on_bar = false;
    wTiles->horizontal_scroll_bar_tags.mouseDiff = 0;
    wTiles->horizontal_scroll_bar_tags.selected = false;
    wTiles->vertical_scroll_bar_tags.visible = false;
}

void tiles_config_rect(Window_Tiles *wTiles,Settings *setting){

    wTiles->new_tile_set.rect.w = wTiles->rect.w*0.6;
    wTiles->new_tile_set.rect.h = wTiles->rect.h*TILE_HEIGHT_ELEMENT_PERCENT;
    wTiles->new_tile_set.rect.x = (wTiles->rect.x+wTiles->rect.w*0.5)-wTiles->new_tile_set.rect.w*0.5;
    wTiles->new_tile_set.rect.y = (wTiles->rect.y+wTiles->rect.h*0.5)-wTiles->new_tile_set.rect.h*0.5;

    get_size_text(setting->fontlower,wTiles->titulo_text,&wTiles->titulo_rect.w,&wTiles->titulo_rect.h);
    wTiles->titulo_rect.x = wTiles->rect.x+TILE_SPACING;
    wTiles->titulo_rect.y = wTiles->rect.y+TILE_SPACING;
    wTiles->titulo_rect.h = wTiles->rect.h*TILE_HEIGHT_ELEMENT_PERCENT;

    wTiles->edit.rect.w = wTiles->rect.w*TILE_BUTTON_WIDTH_PERCENT;
    wTiles->edit.rect.h = wTiles->rect.h*TILE_HEIGHT_ELEMENT_PERCENT;
    wTiles->edit.rect.x = (wTiles->rect.x+wTiles->rect.w)-wTiles->edit.rect.w-TILE_SPACING;
    wTiles->edit.rect.y = wTiles->rect.y+TILE_SPACING;

    wTiles->delete.rect.w = wTiles->rect.w*TILE_BUTTON_WIDTH_PERCENT;
    wTiles->delete.rect.h = wTiles->rect.h*TILE_HEIGHT_ELEMENT_PERCENT;
    wTiles->delete.rect.x = wTiles->edit.rect.x-wTiles->delete.rect.w-TILE_SPACING;
    wTiles->delete.rect.y = wTiles->rect.y+TILE_SPACING;

    wTiles->tags_rect_reference.x = wTiles->rect.x+TILE_SPACING;
    wTiles->tags_rect_reference.y = wTiles->titulo_rect.y+wTiles->titulo_rect.h+TILE_SPACING;
    wTiles->tags_rect_reference.w = wTiles->rect.w-TILE_SPACING*2;
    wTiles->tags_rect_reference.h = wTiles->rect.h*TILE_TAG_RECT_HEIGHT_PERCENT;

    wTiles->tiles_rect_reference.x = wTiles->rect.x+TILE_SPACING;
    wTiles->tiles_rect_reference.y = wTiles->tags_rect_reference.y+wTiles->tags_rect_reference.h+TILE_SPACING;
    wTiles->tiles_rect_reference.w = wTiles->rect.w-TILE_SPACING*2;
    wTiles->tiles_rect_reference.h = (wTiles->rect.y+wTiles->rect.h)-wTiles->tiles_rect_reference.y-TILE_SPACING;


    wTiles->tags_rect.x = wTiles->rect.x+TILE_SPACING;
    wTiles->tags_rect.y = wTiles->titulo_rect.y+wTiles->titulo_rect.h+TILE_SPACING;
    wTiles->tags_rect.w = wTiles->rect.w-TILE_SPACING*2;
    wTiles->tags_rect.h = wTiles->rect.h*TILE_HEIGHT_ELEMENT_PERCENT;

    wTiles->vertical_scroll_bar_tags.rect.x = (wTiles->tags_rect_reference.x+wTiles->tags_rect_reference.w)-wTiles->rect.w*BLOCOS_SCROLL_BAR_SIZE_PERCENT;
    wTiles->vertical_scroll_bar_tags.rect.y = wTiles->tags_rect_reference.y;
    wTiles->vertical_scroll_bar_tags.rect.w = wTiles->rect.w*BLOCOS_SCROLL_BAR_SIZE_PERCENT;

    wTiles->vertical_scroll_bar_tags.subRect.x = wTiles->vertical_scroll_bar_tags.rect.x+SCROLL_BAR_SPACING;
    wTiles->vertical_scroll_bar_tags.subRect.y = wTiles->vertical_scroll_bar_tags.rect.y+SCROLL_BAR_SPACING;
    wTiles->vertical_scroll_bar_tags.subRect.w = wTiles->vertical_scroll_bar_tags.rect.w-SCROLL_BAR_SPACING*2;

    wTiles->horizontal_scroll_bar_tags.rect.x = wTiles->tags_rect_reference.x;
    wTiles->horizontal_scroll_bar_tags.rect.y = (wTiles->tags_rect_reference.y+wTiles->tags_rect_reference.h)-wTiles->rect.w*BLOCOS_SCROLL_BAR_SIZE_PERCENT;
    wTiles->horizontal_scroll_bar_tags.rect.h = wTiles->rect.w*BLOCOS_SCROLL_BAR_SIZE_PERCENT;

    wTiles->horizontal_scroll_bar_tags.subRect.x = wTiles->horizontal_scroll_bar_tags.rect.x+SCROLL_BAR_SPACING;
    wTiles->horizontal_scroll_bar_tags.subRect.y = wTiles->horizontal_scroll_bar_tags.rect.y+SCROLL_BAR_SPACING;
    wTiles->horizontal_scroll_bar_tags.subRect.h = wTiles->horizontal_scroll_bar_tags.rect.h-SCROLL_BAR_SPACING*2;


    wTiles->tiles_rect.x = wTiles->tiles_rect_reference.x;
    wTiles->tiles_rect.y = wTiles->tiles_rect_reference.y;

    wTiles->vertical_scroll_bar_tiles.rect.x = (wTiles->tiles_rect_reference.x+wTiles->tiles_rect_reference.w)-wTiles->rect.w*BLOCOS_SCROLL_BAR_SIZE_PERCENT;
    wTiles->vertical_scroll_bar_tiles.rect.y = wTiles->tiles_rect_reference.y;
    wTiles->vertical_scroll_bar_tiles.rect.w = wTiles->rect.w*BLOCOS_SCROLL_BAR_SIZE_PERCENT;
    wTiles->vertical_scroll_bar_tiles.rect.h = wTiles->rect.h-wTiles->vertical_scroll_bar_tiles.rect.y-wTiles->rect.w*BLOCOS_SCROLL_BAR_SIZE_PERCENT-TILE_SPACING;

    wTiles->vertical_scroll_bar_tiles.subRect.x = wTiles->vertical_scroll_bar_tiles.rect.x+SCROLL_BAR_SPACING;
    wTiles->vertical_scroll_bar_tiles.subRect.y = wTiles->vertical_scroll_bar_tiles.rect.y+SCROLL_BAR_SPACING;
    wTiles->vertical_scroll_bar_tiles.subRect.w = wTiles->vertical_scroll_bar_tiles.rect.w-SCROLL_BAR_SPACING*2;

    wTiles->horizontal_scroll_bar_tiles.rect.w = wTiles->tiles_rect_reference.w-wTiles->vertical_scroll_bar_tiles.rect.w-TILE_SPACING;
    wTiles->horizontal_scroll_bar_tiles.rect.h = wTiles->rect.w*BLOCOS_SCROLL_BAR_SIZE_PERCENT;
    wTiles->horizontal_scroll_bar_tiles.rect.x = wTiles->rect.x+TILE_SPACING;
    wTiles->horizontal_scroll_bar_tiles.rect.y = (wTiles->tiles_rect_reference.y+wTiles->tiles_rect_reference.h)-wTiles->horizontal_scroll_bar_tiles.rect.h;

    wTiles->horizontal_scroll_bar_tiles.subRect.x = wTiles->horizontal_scroll_bar_tiles.rect.x+SCROLL_BAR_SPACING;
    wTiles->horizontal_scroll_bar_tiles.subRect.y = wTiles->horizontal_scroll_bar_tiles.rect.y+SCROLL_BAR_SPACING;
    wTiles->horizontal_scroll_bar_tiles.subRect.h = wTiles->horizontal_scroll_bar_tiles.rect.h-SCROLL_BAR_SPACING*2;

    wTiles->max_width = wTiles->vertical_scroll_bar_tiles.rect.x-wTiles->tiles_rect.x-1;
}

void tiles_run(Window_Tiles *wTiles,Window_Map *wMap,Window_Layers *wLayers,Data *data){
    tiles_input(wTiles,wMap,wLayers,data);
    if(wTiles->tag_selected){
        update_tiles(wTiles->tag_selected->tiles,&wTiles->vertical_scroll_bar_tiles,&wTiles->horizontal_scroll_bar_tiles,wTiles->tag_selected->scale,wTiles->tag_selected->number_of_columns,wTiles->max_width);
        config_tiles_rect(wTiles);
        config_tags_rect(wTiles,data->setting);

        ScrollBarUpdate(&wTiles->vertical_scroll_bar_tags,TYPE_VERTICAL,data->mouse);
        ScrollBarUpdate(&wTiles->horizontal_scroll_bar_tags,TYPE_HORIZONTAL,data->mouse);
        
        ScrollBarUpdate(&wTiles->vertical_scroll_bar_tiles,TYPE_VERTICAL,data->mouse);
        ScrollBarUpdate(&wTiles->horizontal_scroll_bar_tiles,TYPE_HORIZONTAL,data->mouse);
    }
    tiles_draw(wTiles,data->setting,data->renderer);
}

void tiles_draw(Window_Tiles *wTiles,Settings *setting,SDL_Renderer *renderer){
    SDL_Color text_color = setting->text_color;
    SDL_Color background_color = setting->background_color;
    SDL_Color border_color = setting->border_color;
    SDL_Color bg_color_selected = setting->background_color_selected;
    SDL_Color off_color = {255,255,255,50};

    SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
    SDL_RenderFillRect(renderer,&wTiles->rect);

    draw_text(renderer,setting->fontlower,wTiles->titulo_text,text_color,wTiles->titulo_rect,true,true);

    if(wTiles->tag_tiles){

        if(wTiles->tag_selected->type == TYPE_LOCAL){
            draw_button(renderer,&wTiles->delete,setting,setting->fontlower,text_color,border_color);
        }
        else{
            draw_button(renderer,&wTiles->delete,setting,setting->fontlower,off_color,off_color);
        }

        draw_button(renderer,&wTiles->edit,setting,setting->fontlower,text_color,border_color);


        SDL_RenderSetViewport(renderer,&wTiles->tiles_rect);

        SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
        SDL_RenderFillRect(renderer,NULL);

        Tile *tile = wTiles->tag_selected->tiles;
        
        while(tile != NULL){
            SDL_Rect rect = {
                tile->rect.x+wTiles->horizontal_scroll_bar_tiles.offset,
                tile->rect.y+wTiles->vertical_scroll_bar_tiles.offset,
                tile->rect.w,
                tile->rect.h
            };

            SDL_RenderCopy(renderer,tile->texture,NULL,&rect);

            if(wTiles->tag_selected->tile_selected && tile == wTiles->tag_selected->tile_selected){

                SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE,rect.w,rect.h,32,0xff,0xff00,0xff0000,0xff000000);
                SDL_FillRect(surface,NULL,SDL_MapRGBA(surface->format,border_color.r,border_color.g,border_color.b,50));
                
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surface);
                SDL_RenderCopy(renderer,texture,NULL,&rect);

                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
            }
            tile = tile->next;
        }

        SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.g,border_color.a);
        SDL_RenderDrawRect(renderer,NULL);

        SDL_RenderSetViewport(renderer,&wTiles->tags_rect);

        Tag_Tiles *tag_tile = wTiles->tag_tiles;
        SDL_Rect rect;
        while(tag_tile != NULL){

            rect.x = tag_tile->rect.x+wTiles->horizontal_scroll_bar_tags.offset;
            rect.y = tag_tile->rect.y+wTiles->vertical_scroll_bar_tags.offset;
            rect.w = wTiles->tags_rect.w+wTiles->horizontal_scroll_bar_tags.lenght;
            rect.h = tag_tile->rect.h;

            if(tag_tile == wTiles->tag_selected){
                SDL_SetRenderDrawColor(renderer,bg_color_selected.r,bg_color_selected.g,bg_color_selected.b,bg_color_selected.a);
                SDL_RenderFillRect(renderer,&rect);
            }

            draw_text(renderer,setting->fontlower,tag_tile->name,text_color,rect,false,true);

            tag_tile = tag_tile->next;
        }

        SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
        SDL_RenderDrawRect(renderer,NULL);

        SDL_RenderSetViewport(renderer,NULL);

        draw_scroll_bar(&wTiles->vertical_scroll_bar_tags,setting,renderer);
        draw_scroll_bar(&wTiles->horizontal_scroll_bar_tags,setting,renderer);

        draw_scroll_bar(&wTiles->vertical_scroll_bar_tiles,setting,renderer);
        draw_scroll_bar(&wTiles->horizontal_scroll_bar_tiles,setting,renderer);
    }
    else{
        draw_button(renderer,&wTiles->new_tile_set,setting,setting->fontlower,text_color,border_color);
    }

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&wTiles->rect);

}

void tiles_input(Window_Tiles *wTiles,Window_Map *wMap,Window_Layers *wLayers,Data *data){
    
    if(data->window_new_tile_set || data->window_file || data->window_create || data->window_project->modal_visible) return;

    if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT){

        if(wTiles->tag_tiles){

            if(collidePoint(data->mouse,&wTiles->tags_rect)){
                Tag_Tiles *tag_tile = wTiles->tag_tiles;
                while(tag_tile != NULL){
                    if(collidePointOffset(data->mouse,&tag_tile->rect,0,wTiles->vertical_scroll_bar_tags.offset,&wTiles->tags_rect)){
                        wTiles->tag_selected = tag_tile;
                    }
                    tag_tile = tag_tile->next;
                }
            }
            else if(collidePoint(data->mouse,&wTiles->tiles_rect)){
                Tile *tile = wTiles->tag_selected->tiles;
                while(tile != NULL){
                    if(collidePointOffset(data->mouse,&tile->rect,wTiles->horizontal_scroll_bar_tiles.offset,wTiles->vertical_scroll_bar_tiles.offset,&wTiles->tiles_rect)){
                        wTiles->tag_selected->tile_selected = tile;
                    }
                    tile = tile->next;
                }
            }
            else if(collidePoint(data->mouse,&wTiles->vertical_scroll_bar_tiles.subRect) && wTiles->vertical_scroll_bar_tiles.visible){
                wTiles->vertical_scroll_bar_tiles.on_bar = true;
                wTiles->vertical_scroll_bar_tiles.mouseDiff = data->mouse->pos_y - wTiles->vertical_scroll_bar_tiles.subRect.y;
            }
            else if(collidePoint(data->mouse,&wTiles->horizontal_scroll_bar_tiles.subRect) && wTiles->horizontal_scroll_bar_tiles.visible){
                wTiles->horizontal_scroll_bar_tiles.on_bar = true;
                wTiles->horizontal_scroll_bar_tiles.mouseDiff = data->mouse->pos_x - wTiles->horizontal_scroll_bar_tiles.subRect.x;
            }
            else if(collidePoint(data->mouse,&wTiles->vertical_scroll_bar_tags.subRect) && wTiles->vertical_scroll_bar_tags.visible){
                wTiles->vertical_scroll_bar_tags.on_bar = true;
                wTiles->vertical_scroll_bar_tags.mouseDiff = data->mouse->pos_y - wTiles->vertical_scroll_bar_tags.subRect.y;
            }
            else if(collidePoint(data->mouse,&wTiles->horizontal_scroll_bar_tags.subRect) && wTiles->horizontal_scroll_bar_tags.visible){
                wTiles->horizontal_scroll_bar_tags.on_bar = true;
                wTiles->horizontal_scroll_bar_tags.mouseDiff = data->mouse->pos_x - wTiles->horizontal_scroll_bar_tags.subRect.x;
            }
            else if(collidePoint(data->mouse,&wTiles->delete.rect) && wTiles->tag_selected->type == TYPE_LOCAL){
                wTiles->tag_selected->type = TYPE_GLOBAL;
                remove_from_stack(&wMap->redo,NULL,wTiles->tag_selected->path);
                remove_from_stack(&wMap->undo,NULL,wTiles->tag_selected->path);
                remove_block_in_layers(wLayers,NULL,wTiles->tag_selected->path);
            }
            else if(collidePoint(data->mouse,&wTiles->edit.rect)){
                select_tag_by_path(data,wTiles->tag_selected->path);
            }
        }
        else{
            if(collidePoint(data->mouse,&wTiles->new_tile_set.rect)){

                data->window_new_tile_set = calloc(1,sizeof(Window_New_Tile_Set));
                new_tile_init(data->window_new_tile_set);
                new_tile_config_rect(data->window_new_tile_set,data->setting,data->screen_w,data->screen_h);

            }
        }
    }
    else if(data->event->type == SDL_MOUSEBUTTONUP && data->event->button.button == SDL_BUTTON_LEFT){
        wTiles->vertical_scroll_bar_tiles.on_bar = false;
        wTiles->horizontal_scroll_bar_tiles.on_bar = false;
        wTiles->vertical_scroll_bar_tags.on_bar = false;
        wTiles->horizontal_scroll_bar_tags.on_bar = false;
    }
    if(data->event->type == SDL_MOUSEWHEEL && wTiles->tag_tiles && collidePoint(data->mouse,&wTiles->tiles_rect)){
        wTiles->tag_selected->scale += data->event->wheel.preciseY*0.1f;
        if(wTiles->tag_selected->scale > 10.0f){
            wTiles->tag_selected->scale = 10.0f;
        }
        else if(wTiles->tag_selected->scale <= 0){
            wTiles->tag_selected->scale = 0.1f;
        }
    }

    if(collidePoint(data->mouse,&wTiles->rect)){
        if(wTiles->tag_tiles){
            if(collidePoint(data->mouse,&wTiles->vertical_scroll_bar_tiles.subRect) && wTiles->vertical_scroll_bar_tiles.visible){
                wTiles->vertical_scroll_bar_tiles.selected = true;
            }
            else if(collidePoint(data->mouse,&wTiles->horizontal_scroll_bar_tiles.subRect) && wTiles->horizontal_scroll_bar_tiles.visible){
                wTiles->horizontal_scroll_bar_tiles.selected = true;
            }
            else if(collidePoint(data->mouse,&wTiles->vertical_scroll_bar_tags.subRect) && wTiles->vertical_scroll_bar_tags.visible){
                wTiles->vertical_scroll_bar_tags.selected = true;
            }
            else if(collidePoint(data->mouse,&wTiles->horizontal_scroll_bar_tags.subRect) && wTiles->horizontal_scroll_bar_tags.visible){
                wTiles->horizontal_scroll_bar_tags.selected = true;
            }
            else if(collidePoint(data->mouse,&wTiles->edit.rect)){
                wTiles->edit.selected = true;
            }
            else if(collidePoint(data->mouse,&wTiles->delete.rect) && wTiles->tag_selected->type == TYPE_LOCAL){
                wTiles->delete.selected = true;
            }
        }
        else{
            if(collidePoint(data->mouse,&wTiles->new_tile_set.rect)){
                wTiles->new_tile_set.selected = true;
            }
        }
    }
}

void tiles_free(Window_Tiles **wTiles){
    if(*wTiles != NULL){

        free((*wTiles)->edit.text);
        free((*wTiles)->new_tile_set.text);
        free((*wTiles)->delete.text);
        free((*wTiles)->titulo_text);
        
        free_tag_tiles(*wTiles);
        free(*wTiles);
        *wTiles = NULL;
    }
}