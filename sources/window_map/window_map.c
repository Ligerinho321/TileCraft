#include <window_map.h>

void map_init(Window_Map *window_map,Settings *setting,SDL_Renderer *renderer,Properties *properties){

    window_map->properties = properties;

    window_map->pilha = calloc(window_map->properties->size,sizeof(SDL_Point));

    create_chuncks(window_map);

    create_cross_hair(window_map,renderer);

    window_map->undo = NULL;
    window_map->redo = NULL;

    window_map->mode = MOD_PICKER;
    window_map->current_point.x = -1;
    window_map->current_point.y = -1;
    window_map->updated = true;
    window_map->grade_visible = true;

    window_map->picker.selected = false;
    window_map->picker.text = new_string("picker");

    window_map->eraser.selected = false;
    window_map->eraser.text = new_string("eraser");

    window_map->fill.selected = false;
    window_map->fill.text = new_string("fill");

    window_map->fill_rect.selected = false;
    window_map->fill_rect.text = new_string("fill rect");

    window_map->clear_rect.selected = false;
    window_map->clear_rect.text = new_string("clear rect");

    window_map->wTiles = calloc(1,sizeof(Window_Tiles));
    tiles_init(window_map->wTiles);

    window_map->wCamadas = calloc(1,sizeof(Window_Layers));
    layers_init(window_map->wCamadas,setting,window_map->properties);

    window_map->mapInfo = calloc(1,sizeof(Map_Informations));
    map_informations_init(window_map->mapInfo,window_map->properties);

}

void map_config_rect(Window_Map *window_map,Settings *setting){

    window_map->bar_rect.x = window_map->rect_reference.w*MAP_SUB_ELEMENTS_WIDTH+MAP_SPACING*2-1;
    window_map->bar_rect.y = window_map->rect_reference.y+MAP_SPACING;
    window_map->bar_rect.w = window_map->rect_reference.w*WIDTH_MAP_PERCENT-MAP_SPACING*3;
    window_map->bar_rect.h = window_map->rect_reference.h*MAP_HEIGHT_ELEMENT_PERCENT;

    window_map->map_rect.x = window_map->bar_rect.x;
    window_map->map_rect.y = window_map->bar_rect.y+window_map->bar_rect.h+MAP_SPACING-1;
    window_map->map_rect.w = window_map->bar_rect.w;
    window_map->map_rect.h = (window_map->rect_reference.y+window_map->rect_reference.h)-window_map->map_rect.y-MAP_SPACING;

    window_map->wCamadas->rect.w = window_map->rect_reference.w*MAP_SUB_ELEMENTS_WIDTH;
    window_map->wCamadas->rect.h = window_map->rect_reference.h*0.5f-MAP_SPACING*2;
    window_map->wCamadas->rect.x = window_map->rect_reference.x+MAP_SPACING;
    window_map->wCamadas->rect.y = window_map->rect_reference.y+MAP_SPACING;

    window_map->mapInfo->rect.x = window_map->rect_reference.x+MAP_SPACING;
    window_map->mapInfo->rect.y = window_map->wCamadas->rect.y+window_map->wCamadas->rect.h+MAP_SPACING;
    window_map->mapInfo->rect.w = window_map->rect_reference.w*MAP_SUB_ELEMENTS_WIDTH;
    window_map->mapInfo->rect.h = (window_map->rect_reference.y+window_map->rect_reference.h)-window_map->mapInfo->rect.y-MAP_SPACING;

    window_map->wTiles->rect.x = (window_map->rect_reference.x+window_map->rect_reference.w)-window_map->rect_reference.w*MAP_SUB_ELEMENTS_WIDTH-MAP_SPACING+1;
    window_map->wTiles->rect.y = window_map->rect_reference.y+MAP_SPACING;
    window_map->wTiles->rect.w = window_map->rect_reference.w*MAP_SUB_ELEMENTS_WIDTH;
    window_map->wTiles->rect.h = window_map->rect_reference.h-MAP_SPACING*2;

    window_map->properties->transX = -(window_map->map_rect.x+window_map->map_rect.w*0.5);
    window_map->properties->transY =  -(window_map->map_rect.y+window_map->map_rect.h*0.5);
    window_map->properties->scale = 1.0f;

    window_map->picker.rect.x = window_map->bar_rect.x+BAR_SPACING;
    window_map->picker.rect.y = window_map->bar_rect.y+BAR_SPACING;
    window_map->picker.rect.w = window_map->bar_rect.w*BAR_WIDTH_BUTTON_PERCENT;
    window_map->picker.rect.h = window_map->bar_rect.h-BAR_SPACING*2;

    window_map->eraser.rect.x = window_map->picker.rect.x+window_map->picker.rect.w+BAR_SPACING;
    window_map->eraser.rect.y = window_map->picker.rect.y;
    window_map->eraser.rect.w = window_map->bar_rect.w*BAR_WIDTH_BUTTON_PERCENT;
    window_map->eraser.rect.h = window_map->bar_rect.h-BAR_SPACING*2;

    window_map->fill.rect.x = window_map->eraser.rect.x+window_map->eraser.rect.w+BAR_SPACING;
    window_map->fill.rect.y = window_map->eraser.rect.y;
    window_map->fill.rect.w = window_map->bar_rect.w*BAR_WIDTH_BUTTON_PERCENT;
    window_map->fill.rect.h = window_map->bar_rect.h-BAR_SPACING*2;

    window_map->fill_rect.rect.x = window_map->fill.rect.x+window_map->fill.rect.w+BAR_SPACING;
    window_map->fill_rect.rect.y = window_map->fill.rect.y;
    window_map->fill_rect.rect.w = window_map->bar_rect.w*BAR_WIDTH_BUTTON_PERCENT; 
    window_map->fill_rect.rect.h = window_map->bar_rect.h-BAR_SPACING*2;

    window_map->clear_rect.rect.x = window_map->fill_rect.rect.x+window_map->fill_rect.rect.w+BAR_SPACING;
    window_map->clear_rect.rect.y = window_map->fill_rect.rect.y;
    window_map->clear_rect.rect.w = window_map->bar_rect.w*BAR_WIDTH_BUTTON_PERCENT;
    window_map->clear_rect.rect.h = window_map->bar_rect.h-BAR_SPACING*2;

    map_informations_config_rect(window_map->mapInfo,setting);
    layers_config_rect(window_map->wCamadas,setting);
    tiles_config_rect(window_map->wTiles,setting);
}

void map_draw(Window_Map *window_map,Data *data){
    const SDL_Color background_color_map = data->setting->background_color_map;
    const SDL_Color border_color = data->setting->border_color;
    const SDL_Color grade_color = data->setting->grade_color;

    const float transX = window_map->properties->transX;
    const float transY = window_map->properties->transY;
    const float scale = window_map->properties->scale;
    const int block_width = window_map->properties->width_block;
    const int block_height = window_map->properties->height_block;
    const int map_width = window_map->properties->width_map;
    const int map_height = window_map->properties->height_map;
    const int size = window_map->properties->size;

    SDL_RenderSetViewport(data->renderer,&window_map->map_rect);

    SDL_SetRenderDrawColor(data->renderer,background_color_map.r,background_color_map.g,background_color_map.b,background_color_map.a);
    SDL_RenderFillRect(data->renderer,NULL);

    SDL_Point ponto;
    SDL_Point ponto1,ponto2,ponto3,ponto4;
    Chunck chunck;
    Tag_Camadas *tags;
    SDL_Rect rect;
    Tile *tile_selected = (window_map->wTiles->tag_selected) ? window_map->wTiles->tag_selected->tile_selected : NULL;

    SDL_SetRenderDrawColor(data->renderer,grade_color.r,grade_color.g,grade_color.b,grade_color.a);

    for(int i=0; i<window_map->nChuncks; ++i){

        chunck = window_map->chuncks[i];
        
        convert_to_screen(chunck.start_x,chunck.start_y,&ponto1,window_map->properties,window_map->map_rect);
        convert_to_screen(chunck.end_x,chunck.start_y,&ponto2,window_map->properties,window_map->map_rect);
        convert_to_screen(chunck.end_x,chunck.end_y,&ponto3,window_map->properties,window_map->map_rect);
        convert_to_screen(chunck.start_x,chunck.end_y,&ponto4,window_map->properties,window_map->map_rect);

        if(ponto3.y >= 0 && ponto1.y < window_map->map_rect.h && ponto2.x >= 0 && ponto4.x < window_map->map_rect.w){

            tags = window_map->wCamadas->tags;
            while(tags != NULL){
                for(int y=chunck.start_y; y<chunck.end_y; ++y){
                    for(int x=chunck.start_x; x<chunck.end_x; ++x){
                        
                        Bloco bloco = tags->matriz[y][x];
                        if(window_map->mode == MOD_FILL && tile_selected && bloco.buffer == tile_selected->id_local && (!bloco.tile || bloco.buffer != bloco.tile->id_local)){
                            draw_bloco(window_map,data->renderer,x,y,tile_selected);
                        }
                        else if(bloco.tile){
                            draw_bloco(window_map,data->renderer,x,y,bloco.tile);
                        }
                    }
                }
                tags = tags->next;
            }

        }
    }

    switch(window_map->mode){
        case MOD_FILL:
           mod_fill_update(window_map,data);
           break;
        case MOD_FILL_RECT:
           mod_fill_shape_update(window_map,data);
           break;
        case MOD_CLEAR_RECT:
           mod_clear_rect_update(window_map,data);
           break;
    }

    if(window_map->grade_visible){
        for(int i=0; i<window_map->nChuncks; ++i){
            chunck = window_map->chuncks[i];
            
            convert_to_screen(chunck.start_x,chunck.start_y,&ponto1,window_map->properties,window_map->map_rect);
            convert_to_screen(chunck.end_x,chunck.start_y,&ponto2,window_map->properties,window_map->map_rect);
            convert_to_screen(chunck.end_x,chunck.end_y,&ponto3,window_map->properties,window_map->map_rect);
            convert_to_screen(chunck.start_x,chunck.end_y,&ponto4,window_map->properties,window_map->map_rect);
            
            if(ponto3.y >= 0 && ponto1.y < window_map->map_rect.h && ponto2.x >= 0 && ponto4.x < window_map->map_rect.w){
                
                for(int y=chunck.start_y; y<chunck.end_y; ++y){
                    for(int x=chunck.start_x; x<chunck.end_x; ++x){
                        
                        convert_to_screen(x,y,&ponto,window_map->properties,window_map->map_rect);
                        
                        if(window_map->properties->type == TYPE_ISOMETRIC && 
                          (ponto.y+window_map->properties->height_block*window_map->properties->scale >= 0 && ponto.y < window_map->map_rect.h) &&
                          (ponto.x+window_map->properties->width_block*window_map->properties->scale >= 0 && ponto.x-window_map->properties->width_block*window_map->properties->scale < window_map->map_rect.w)){
                            
                            //0-1
                            if(y == 0) SDL_RenderDrawLine(data->renderer,ponto.x,ponto.y,ponto.x+block_width*scale,ponto.y+block_height*0.5f*scale);
                            
                            //1-2
                            SDL_RenderDrawLine(data->renderer,
                                               ponto.x+block_width*scale,
                                               ponto.y+block_height*0.5f*scale,
                                               ponto.x,
                                               ponto.y+block_height*scale
                                              );
                            //2-3
                            SDL_RenderDrawLine(data->renderer,
                                               ponto.x,
                                               ponto.y+block_height*scale,
                                               ponto.x-block_width*scale,
                                               ponto.y+block_height*0.5f*scale
                                              );
                                              
                            //3-0
                            if(x == 0) SDL_RenderDrawLine(data->renderer,ponto.x-block_width*scale,ponto.y+block_height*0.5f*scale,ponto.x,ponto.y);
                        }
                        else if(window_map->properties->type == TYPE_ORTHOGONAL && 
                               (ponto.y+window_map->properties->height_block*window_map->properties->scale >= 0 && ponto.y < window_map->map_rect.h) &&
                               (ponto.x+window_map->properties->width_block*window_map->properties->scale >= 0 && ponto.x < window_map->map_rect.w)){
                                
                                //0-1 
                                if(y == 0) SDL_RenderDrawLine(data->renderer,ponto.x,ponto.y,ponto.x+block_width*scale,ponto.y);
                                
                                //1-2
                                SDL_RenderDrawLine(data->renderer,
                                                  ponto.x+block_width*scale,
                                                  ponto.y,
                                                  ponto.x+block_width*scale,
                                                  ponto.y+block_height*scale
                                                  );
                                //2-3
                                SDL_RenderDrawLine(data->renderer,
                                                  ponto.x+block_width*scale,
                                                  ponto.y+block_height*scale,
                                                  ponto.x,
                                                  ponto.y+block_height*scale
                                                  );
                                //3-0
                                if(x == 0) SDL_RenderDrawLine(data->renderer,ponto.x,ponto.y+block_height*scale,ponto.x,ponto.y);
                        }
                    }
                }
            }
        }
    }
    

    if(window_map->mode == MOD_PICKER && tile_selected){
        draw_bloco(window_map,data->renderer,window_map->properties->pos_x,window_map->properties->pos_y,tile_selected);
    }

    if(window_map->mode == MOD_PICKER || window_map->mode == MOD_ERASER){
        draw_cross_hair(window_map,data->renderer,window_map->properties->pos_x,window_map->properties->pos_y);
    }

    SDL_SetRenderDrawColor(data->renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(data->renderer,NULL);

    SDL_RenderSetViewport(data->renderer,NULL);
}

void map_input(Window_Map *window_map,Data *data){

    if(!data->window_file && !data->window_create && !data->window_new_tile_set && !data->window_project->modal_visible){

        if(data->event->type == SDL_KEYDOWN && SDL_GetModState() & KMOD_CTRL){
            if(window_map->undo && data->event->key.keysym.scancode == SDL_SCANCODE_Z){
                map_undo_redo_manager(window_map,&window_map->undo,TYPE_UNDO);
            }
            else if(window_map->redo && data->event->key.keysym.scancode == SDL_SCANCODE_Y){
                map_undo_redo_manager(window_map,&window_map->redo,TYPE_REDO);
            }
            else if(data->event->key.keysym.scancode == SDL_SCANCODE_G){
                window_map->grade_visible = !window_map->grade_visible;
            }
            else if(data->event->key.keysym.scancode == SDL_SCANCODE_R){
                window_map->properties->transX = -(window_map->map_rect.x+window_map->map_rect.w*0.5);
                window_map->properties->transY =  -(window_map->map_rect.y+window_map->map_rect.h*0.5);
                window_map->properties->scale = 1.0f;
            }
        }

        if(data->event->type == SDL_MOUSEBUTTONUP && data->event->button.button == SDL_BUTTON_LEFT){
            if(collidePoint(data->mouse,&window_map->picker.rect)){
                window_map->mode = MOD_PICKER;
            }
            else if(collidePoint(data->mouse,&window_map->eraser.rect)){
                window_map->mode = MOD_ERASER;
            }
            else if(collidePoint(data->mouse,&window_map->fill.rect)){
                window_map->mode = MOD_FILL;
            }
            else if(collidePoint(data->mouse,&window_map->fill_rect.rect)){
                window_map->mode = MOD_FILL_RECT;
                window_map->shape_buffer[0].x = window_map->shape_buffer[0].y = -1;
            }
            else if(collidePoint(data->mouse,&window_map->clear_rect.rect)){
                window_map->mode = MOD_CLEAR_RECT;
                window_map->shape_buffer[0].x = window_map->shape_buffer[0].y = -1;
            }
        }

        if(collidePoint(data->mouse,&window_map->map_rect)){
            screenToWorld(data->mouse->pos_x,data->mouse->pos_y,&data->mouse->pos_x_before,&data->mouse->pos_y_before,window_map->properties->transX,window_map->properties->transY,window_map->properties->scale);
            
            if(data->event->type == SDL_MOUSEWHEEL){
                if(data->event->wheel.preciseY > 0){
                    if(window_map->properties->width_block*(window_map->properties->scale+0.1f) < window_map->map_rect.w && window_map->properties->height_block*(window_map->properties->scale+0.1f) < window_map->map_rect.h){
                        window_map->properties->scale += 0.1f;
                    }
                }
                else{
                   if(window_map->properties->width_block*(window_map->properties->scale-0.1f) > 10 && window_map->properties->height_block*(window_map->properties->scale-0.1) > 10){
                      window_map->properties->scale -= 0.1f;
                   }
                }

            }

            screenToWorld(data->mouse->pos_x,data->mouse->pos_y,&data->mouse->pos_x_after,&data->mouse->pos_y_after,window_map->properties->transX,window_map->properties->transY,window_map->properties->scale);
            
            window_map->properties->transX += (data->mouse->pos_x_before - data->mouse->pos_x_after);
            window_map->properties->transY += (data->mouse->pos_y_before - data->mouse->pos_y_after);
            
            if(data->mouse->press_right){
                window_map->properties->transX -= (float)data->mouse->rel_x / window_map->properties->scale;
                window_map->properties->transY -= (float)data->mouse->rel_y / window_map->properties->scale;
            }
        }

        if(data->mouse->press_left && window_map->wCamadas->tag_atual && collidePoint(data->mouse,&window_map->map_rect) && collidePointMap(data->mouse,window_map->properties)){
            
            Tile *target_tile = window_map->wCamadas->tag_atual->matriz[window_map->properties->pos_y][window_map->properties->pos_x].tile;
            
            switch(window_map->mode){
                case MOD_PICKER:
                    if(window_map->wTiles->tag_selected && window_map->wTiles->tag_selected->tile_selected && (!target_tile || window_map->wTiles->tag_selected->tile_selected->id_local != target_tile->id_local)){

                        Tile *tile_selected = window_map->wTiles->tag_selected->tile_selected;
                        
                        if(window_map->wTiles->tag_selected->type == TYPE_GLOBAL){
                            window_map->wTiles->tag_selected->type = TYPE_LOCAL;
                            sort_tag_tiles(window_map->wTiles->tag_tiles);
                        }
                        
                        Bloco *bloco = &window_map->wCamadas->tag_atual->matriz[window_map->properties->pos_y][window_map->properties->pos_x];
                        bloco->tile = tile_selected;
                        bloco->tag_tile = window_map->wTiles->tag_selected;
                        bloco->buffer = tile_selected->id_local;

                        window_map->undo = push_stack(window_map->undo,
                                                push_element(NULL,window_map->properties->pos_x,window_map->properties->pos_y,window_map->wTiles->tag_selected,tile_selected),
                                                window_map->wCamadas->tag_atual,
                                                TYPE_ERASER
                                               );

                        window_map->redo = free_stack(window_map->redo);
                        window_map->updated = false;
                    }
                    break;
                case MOD_ERASER:
                    if(target_tile){
                        
                        Bloco *bloco = &window_map->wCamadas->tag_atual->matriz[window_map->properties->pos_y][window_map->properties->pos_x];
                        bloco->tile = NULL;
                        bloco->tag_tile = NULL;
                        bloco->buffer = -1;

                        window_map->undo = push_stack(window_map->undo,
                                                push_element(NULL,window_map->properties->pos_x,window_map->properties->pos_y,window_map->wTiles->tag_selected,target_tile),
                                                window_map->wCamadas->tag_atual,
                                                TYPE_ADD
                                               );
                        
                        window_map->redo = free_stack(window_map->redo);
                        window_map->updated = false;
                    }
                    break;
            }
        }
    }
}

void map_run(Window_Map *window_map,Data *data){
    map_input(window_map,data);

    bar_draw(window_map,data->setting,data->renderer);

    map_draw(window_map,data);

    if(collidePointMap(data->mouse,window_map->properties) && window_map->wCamadas->tag_atual && window_map->wCamadas->tag_atual->matriz[window_map->properties->pos_y][window_map->properties->pos_x].tile){
        window_map->properties->current_id =  window_map->wCamadas->tag_atual->matriz[window_map->properties->pos_y][window_map->properties->pos_x].tile->id_global;
    }
    else{
        window_map->properties->current_id = -1;
    }

    map_informations_run(window_map->mapInfo,data->setting,data->renderer,data->mouse);
    tiles_run(window_map->wTiles,window_map,window_map->wCamadas,data);
    layers_run(window_map->wCamadas,window_map,data);

    if(!window_map->undo) window_map->updated = true;
}

void map_free(Window_Map *window_map){

    tiles_free(&window_map->wTiles);
    layers_free(&window_map->wCamadas);
    map_informations_free(&window_map->mapInfo);

    free_stack(window_map->undo);
    free_stack(window_map->redo);

    free(window_map->properties->orientation);
    free(window_map->properties);
    free(window_map->path);
    free(window_map->path_export);

    free(window_map->picker.text);
    free(window_map->eraser.text);
    free(window_map->fill.text);
    free(window_map->fill_rect.text);
    free(window_map->clear_rect.text);
        
    free(window_map->chuncks);
    free(window_map->pilha);

    SDL_DestroyTexture(window_map->cross_hair_true_texture);
    SDL_DestroyTexture(window_map->cross_hair_false_texture);

    free(window_map);
}