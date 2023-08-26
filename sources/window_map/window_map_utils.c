#include <window_map_utils.h>

void remove_tag_camada_from_stack(Map_Undo_Redo_Action **stack,Tag_Camadas *tag_camada){
    Map_Undo_Redo_Action *aux = *stack;
    Map_Undo_Redo_Action *previous = NULL;

    while(aux){
        if(aux->tag_camada == tag_camada){

            if(aux == *stack){
                *stack = aux->previous;
            }

            if(previous){
                previous->previous = aux->previous;
            }

            Map_Undo_Redo_Action *buffer_stack = aux->previous;

            while(aux->elements){
                Element *buffer_element = aux->elements->previous;
                free(aux->elements);
                aux->elements = buffer_element;
            }

            free(aux);
            aux = buffer_stack;
        }
        else{
            previous = aux;
            aux = aux->previous;
        }
    }
}

void remove_from_stack(Map_Undo_Redo_Action **stack,char *path_tile,char *path_tag_tile){

    if(!(*stack)) return;

    size_t nElements;
    size_t nChecked;

    Map_Undo_Redo_Action *current_stack = *stack;
    Map_Undo_Redo_Action *previous_stack = NULL;

    Element *current_element;
    Element *previous_element;

    while(current_stack){
        current_element = current_stack->elements;
        previous_element = NULL;
        nElements = 0;
        nChecked = 0;

        while(current_element){
            ++nElements;
            if((path_tile && current_element->tile->path && !strcmp(current_element->tile->path,path_tile)) ||
               (path_tag_tile && current_element->tag_tile->path && !strcmp(current_element->tag_tile->path,path_tag_tile))){
                
                ++nChecked;

                if(current_element == current_stack->elements){
                    current_stack->elements = current_element->previous;
                }

                if(previous_element){
                    previous_element->previous = current_element->previous;
                }
                
                Element *buffer_element = current_element->previous;
                free(current_element);
                current_element = buffer_element;
            }
            else{
                previous_element = current_element;
                current_element = current_element->previous;
            }
        }

        if(nChecked == nElements){
            Map_Undo_Redo_Action *buffer_stack = current_stack->previous;

            if(*stack == current_stack) *stack = buffer_stack;

            free(current_stack);
            current_stack = buffer_stack;

            if(previous_stack) previous_stack->previous = current_stack;

        }
        else{
            previous_stack = current_stack;
            current_stack = current_stack->previous;
        }
    }
}

Element* push_element(Element *elements,size_t x,size_t y,Tag_Tiles *tag_tile,Tile *tile){
    Element *new = calloc(1,sizeof(Element));
    new->x = x;
    new->y = y;
    new->tile = tile;
    new->tag_tile = tag_tile;
    new->previous = (elements) ? elements : NULL;

    return new;
}

Map_Undo_Redo_Action* push_stack(Map_Undo_Redo_Action *stack,Element *elements,Tag_Camadas *tag_camada,Tipo type){
    Map_Undo_Redo_Action *new = calloc(1,sizeof(Map_Undo_Redo_Action));
    new->elements = elements;
    new->type = type;
    new->tag_camada = tag_camada;
    new->previous = (stack) ? stack : NULL;

    return new;
}

Map_Undo_Redo_Action* free_stack(Map_Undo_Redo_Action *stack){
    while(stack != NULL){
        Map_Undo_Redo_Action *buffer_stack = stack->previous;

        while(stack->elements){
            Element *buffer_element = stack->elements->previous;
            free(stack->elements);
            stack->elements = buffer_element;
        }

        free(stack);
        stack = buffer_stack;
    }
    return NULL;
}

void map_undo_redo_manager(Window_Map *wMap,Map_Undo_Redo_Action **stack,Tipo type){

    Bloco **matriz = (*stack)->tag_camada->matriz;
    Element *element = (*stack)->elements;
        
    if(type == TYPE_UNDO){
        wMap->redo = push_stack(wMap->redo,
                                (*stack)->elements,
                                (*stack)->tag_camada,
                                ((*stack)->type == TYPE_ERASER) ? TYPE_ADD : TYPE_ERASER
                               );
    }
    else if(type == TYPE_REDO){

        wMap->updated = false;

        wMap->undo = push_stack(wMap->undo,
                                (*stack)->elements,
                                (*stack)->tag_camada,
                                ((*stack)->type == TYPE_ERASER) ? TYPE_ADD : TYPE_ERASER
                               );
    }
        
    while(element != NULL){
        if((*stack)->type == TYPE_ERASER){
            matriz[element->y][element->x].tile = NULL;
            matriz[element->y][element->x].tag_tile = NULL;
            matriz[element->y][element->x].buffer = -1;
        }
        else if((*stack)->type == TYPE_ADD){
            matriz[element->y][element->x].tile = element->tile;
            matriz[element->y][element->x].tag_tile = element->tag_tile;
            matriz[element->y][element->x].buffer = (matriz[element->y][element->x].tile) ? matriz[element->y][element->x].tile->id_local : -1;
        }
            
        element = element->previous;
    }

    Map_Undo_Redo_Action *buffer_stack = (*stack)->previous;
    free(*stack);
    *stack = buffer_stack;
}

void clear_buffer_matriz(Window_Map *wMap){
    if(wMap->wCamadas->tag_atual){
        int y = 0;
        int x = 0;
        Bloco **matriz = wMap->wCamadas->tag_atual->matriz;
        
        for(int i=0; i<wMap->properties->size; ++i){
            matriz[y][x].buffer = (matriz[y][x].tile) ? matriz[y][x].tile->id_local : -1;
            
            ++x;
            if(x>=wMap->properties->width_map){
                x=0;
                ++y;
            }
        }
    }
}

void fill_matriz(Window_Map *wMap){
    
    if(wMap->wTiles->tag_selected->type == TYPE_GLOBAL){
        wMap->wTiles->tag_selected->type = TYPE_LOCAL;
        sort_tag_tiles(wMap->wTiles->tag_tiles);
    }

    Bloco **matriz = wMap->wCamadas->tag_atual->matriz;
    Tile *tile = wMap->wTiles->tag_selected->tile_selected;
                            
    int y = 0;
    int x = 0;
    Element *elements = NULL;
    
    for(int i=0; i<wMap->properties->size; ++i){
        if(matriz[y][x].buffer == tile->id_local && (!matriz[y][x].tile || matriz[y][x].tile->id_local != matriz[y][x].buffer)){
            matriz[y][x].tile = tile;
            matriz[y][x].tag_tile = wMap->wTiles->tag_selected;
            elements = push_element(elements,x,y,wMap->wTiles->tag_selected,tile);
        }
        matriz[y][x].buffer = (matriz[y][x].tile) ? matriz[y][x].tile->id_local : -1;
                                
        ++x;
        if(x>=wMap->properties->width_map){
            x=0;
            ++y;
        }
    }

    wMap->undo = push_stack(wMap->undo,elements,wMap->wCamadas->tag_atual,TYPE_ERASER);
    wMap->redo = free_stack(wMap->redo);
    wMap->updated = false;
}

void create_cross_hair(Window_Map *wMap,SDL_Renderer *renderer){
    const int block_width = wMap->properties->width_block;
    const int block_height = wMap->properties->height_block;

    SDL_Color fill_color_false = {255, 0, 0,100};
    SDL_Color border_color_false = {200,0,0,100};

    SDL_Color fill_color_true = {26, 163, 255, 100};
    SDL_Color border_color_true = {0, 0, 200,100};

    SDL_Point pontos[4];
    SDL_Surface *surface;

    if(wMap->properties->type == TYPE_ISOMETRIC){
        pontos[0].x = block_width,     pontos[0].y = 0;
        pontos[1].x = block_width*2,   pontos[1].y = block_height*0.5f;
        pontos[2].x = block_width,     pontos[2].y = block_height;
        pontos[3].x = 0,               pontos[3].y = block_height*0.5f;

        surface = SDL_CreateRGBSurface(SDL_SWSURFACE,block_width*2,block_height,32,0xff,0xff00,0xff0000,0xff000000);
    }
    else if(wMap->properties->type == TYPE_ORTHOGONAL){
        pontos[0].x = 0,           pontos[0].y = 0;
        pontos[1].x = block_width, pontos[1].y = 0;
        pontos[2].x = block_width, pontos[2].y = block_height;
        pontos[3].x = 0,           pontos[3].y = block_height;

        surface = SDL_CreateRGBSurface(SDL_SWSURFACE,block_width,block_height,32,0xff,0xff00,0xff0000,0xff000000);
    }

    fillPolygon(surface,fill_color_true,pontos,4);
    drawLines(surface,fill_color_true,pontos,4,true);
    wMap->cross_hair_true_texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_SetTextureScaleMode(wMap->cross_hair_true_texture,SDL_ScaleModeBest);

    
    fillPolygon(surface,fill_color_false,pontos,4);
    drawLines(surface,fill_color_false,pontos,4,true);
    wMap->cross_hair_false_texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_SetTextureScaleMode(wMap->cross_hair_false_texture,SDL_ScaleModeBest);

    SDL_FreeSurface(surface);
}

void create_chuncks(Window_Map *wMap){
    wMap->nChuncks = 0;
    int width = ceil((float)wMap->properties->width_map/(float)CHUNCK_WIDTH);
    int height = ceil((float)wMap->properties->height_map/(float)CHUNCK_HEIGHT);
    wMap->chuncks = calloc(width*height,sizeof(Chunck));

    for(int y=0; y<wMap->properties->height_map; y+=CHUNCK_HEIGHT){
        for(int x=0; x<wMap->properties->width_map; x+=CHUNCK_WIDTH){
            Chunck chunck;

            chunck.start_x = x;
            chunck.start_y = y;

            if(chunck.start_x+CHUNCK_WIDTH > wMap->properties->width_map){
                chunck.end_x = wMap->properties->width_map;
            }
            else{
                chunck.end_x = chunck.start_x+CHUNCK_WIDTH;
            }

            if(chunck.start_y+CHUNCK_HEIGHT > wMap->properties->height_map){
                chunck.end_y = wMap->properties->height_map;
            }
            else{
                chunck.end_y = chunck.start_y+CHUNCK_HEIGHT;
            }

            wMap->chuncks[wMap->nChuncks] = chunck;
            ++wMap->nChuncks;
        }
    }

}

void push(SDL_Point *pilha,int x,int y,int *top){
    pilha[*top].x = x;
    pilha[*top].y = y;
    (*top)++;
}

void draw_bloco(Window_Map *wMap,SDL_Renderer *renderer,int x,int y,Tile *tile){

    SDL_Point ponto;
    SDL_Rect rect;

    convert_to_screen(x,y,&ponto,wMap->properties,wMap->map_rect);

    if(wMap->properties->type == TYPE_ISOMETRIC){
        rect.x = ponto.x-tile->largura*0.5f*wMap->properties->scale;
        rect.y = (ponto.y-tile->altura*wMap->properties->scale)+wMap->properties->height_block*wMap->properties->scale;
        rect.w = tile->largura*wMap->properties->scale;
        rect.h = tile->altura*wMap->properties->scale;
    }
    else if(wMap->properties->type == TYPE_ORTHOGONAL){
        rect.x = ponto.x;
        rect.y = (ponto.y-tile->altura*wMap->properties->scale)+wMap->properties->height_block*wMap->properties->scale;
        rect.w = tile->largura*wMap->properties->scale;
        rect.h = tile->altura*wMap->properties->scale;
    }

    if(rect.x+rect.w >= 0 && rect.x < wMap->map_rect.w && rect.y+rect.h >= 0 && rect.y < wMap->map_rect.h){
        SDL_RenderCopy(renderer,tile->texture,NULL,&rect);
    }
}

void draw_cross_hair(Window_Map *wMap,SDL_Renderer *renderer,int x,int y){
    SDL_Point ponto;
    SDL_Rect rect;

    convert_to_screen(x,y,&ponto,wMap->properties,wMap->map_rect);

    if(wMap->properties->type == TYPE_ISOMETRIC){
        rect.x = ponto.x-wMap->properties->width_block*wMap->properties->scale;
        rect.y = ponto.y;
        rect.w = wMap->properties->width_block*2*wMap->properties->scale;
        rect.h = wMap->properties->height_block*wMap->properties->scale;
    }
    else if(wMap->properties->type == TYPE_ORTHOGONAL){
        rect.x = ponto.x;
        rect.y = ponto.y;
        rect.w = wMap->properties->width_block*wMap->properties->scale;
        rect.h = wMap->properties->height_block*wMap->properties->scale;
    }

    if(x >= 0 && y >= 0 && x < wMap->properties->width_map && y < wMap->properties->height_map){
        SDL_RenderCopy(renderer,wMap->cross_hair_true_texture,NULL,&rect);
    }
    else{
        SDL_RenderCopy(renderer,wMap->cross_hair_false_texture,NULL,&rect);
    }
}

void bar_draw(Window_Map *wMap,Settings *setting,SDL_Renderer *renderer){

    SDL_Color border_color = setting->border_color;
    SDL_Color text_color = setting->text_color;

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&wMap->bar_rect);

    switch(wMap->mode){
        case MOD_PICKER:
           wMap->picker.selected = true;
           break;
        case MOD_ERASER:
           wMap->eraser.selected = true;
           break;
        case MOD_FILL:
           wMap->fill.selected = true;
           break;
        case MOD_FILL_RECT:
           wMap->fill_rect.selected = true;
           break;
        case MOD_CLEAR_RECT:
           wMap->clear_rect.selected = true;
           break;
    }

    draw_button(renderer,&wMap->picker,setting,setting->fontlower,text_color,border_color);
    draw_button(renderer,&wMap->eraser,setting,setting->fontlower,text_color,border_color);
    draw_button(renderer,&wMap->fill,setting,setting->fontlower,text_color,border_color);
    draw_button(renderer,&wMap->fill_rect,setting,setting->fontlower,text_color,border_color);
    draw_button(renderer,&wMap->clear_rect,setting,setting->fontlower,text_color,border_color);

}


void mod_fill_update(Window_Map *wMap,Data *data){
    if(!wMap->wCamadas->tag_atual || !wMap->wTiles->tag_selected || !wMap->wTiles->tag_selected->tile_selected ||
       data->window_file || data->window_create || data->window_new_tile_set || data->window_project->modal_visible) return;

    clear_buffer_matriz(wMap);

    if(collidePoint(data->mouse,&wMap->map_rect) && collidePointMap(data->mouse,wMap->properties)){

        Bloco **matriz = wMap->wCamadas->tag_atual->matriz;

        if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT){
            fill_matriz(wMap);
        }
        else if(!matriz[wMap->properties->pos_y][wMap->properties->pos_x].tile || matriz[wMap->properties->pos_y][wMap->properties->pos_x].tile->id_local != wMap->wTiles->tag_selected->tile_selected->id_local){
            
            SDL_Point ponto;
            int target_id = (matriz[wMap->properties->pos_y][wMap->properties->pos_x].tile) ? matriz[wMap->properties->pos_y][wMap->properties->pos_x].tile->id_local : -1;
            int top = 0;
            SDL_Point pos;
            SDL_Rect rect;

            push(wMap->pilha,wMap->properties->pos_x,wMap->properties->pos_y,&top);

            while(top > 0){
                ponto.x = wMap->pilha[top-1].x;
                ponto.y = wMap->pilha[top-1].y;

                --top;

                if(matriz[ponto.y][ponto.x].buffer == target_id){

                    matriz[ponto.y][ponto.x].buffer = wMap->wTiles->tag_selected->tile_selected->id_local;

                    draw_cross_hair(wMap,data->renderer,ponto.x,ponto.y);

                    if(ponto.x+1 < wMap->properties->width_map && matriz[ponto.y][ponto.x+1].buffer == target_id){
                        push(wMap->pilha,ponto.x+1,ponto.y,&top);
                    }
                    if(ponto.x-1 >= 0 && matriz[ponto.y][ponto.x-1].buffer == target_id){
                        push(wMap->pilha,ponto.x-1,ponto.y,&top);
                    }
                    if(ponto.y+1 < wMap->properties->height_map && matriz[ponto.y+1][ponto.x].buffer == target_id){
                        push(wMap->pilha,ponto.x,ponto.y+1,&top);
                    }
                    if(ponto.y-1 >= 0 && matriz[ponto.y-1][ponto.x].buffer == target_id){
                        push(wMap->pilha,ponto.x,ponto.y-1,&top);
                    }
                }
            }
        }
    }
}

void mod_fill_shape_update(Window_Map *wMap,Data *data){

    if(!wMap->wCamadas->tag_atual || !wMap->wTiles->tag_selected || !wMap->wTiles->tag_selected->tile_selected ||
       data->window_file || data->window_create || data->window_new_tile_set || data->window_project->modal_visible) return;

    if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT && collidePoint(data->mouse,&wMap->map_rect) && collidePointMap(data->mouse,wMap->properties)){
        wMap->shape_buffer[0].x = wMap->shape_buffer[1].x = wMap->properties->pos_x;
        wMap->shape_buffer[0].y = wMap->shape_buffer[1].y = wMap->properties->pos_y;
    }
    else if(data->event->type == SDL_MOUSEBUTTONUP && data->event->button.button == SDL_BUTTON_LEFT && wMap->shape_buffer[0].x >= 0){
        fill_matriz(wMap);
        wMap->shape_buffer[0].x = wMap->shape_buffer[0].y = -1;
    }

    if(data->mouse->press_left && wMap->shape_buffer[0].x >= 0){

        wMap->shape_buffer[1].x = wMap->properties->pos_x;
        wMap->shape_buffer[1].y = wMap->properties->pos_y;

        clear_buffer_matriz(wMap);

        SDL_Point vertex1 = wMap->shape_buffer[0];
        SDL_Point vertex2 = wMap->shape_buffer[1];
        int start_x;
        int start_y;
        int end_x,end_y;
        int inc_x,inc_y;
                        
        if(vertex2.x > vertex1.x){
            start_x = vertex1.x;
            end_x = vertex2.x;
        }
        else{
            start_x = vertex2.x;
            end_x = vertex1.x;
        }
                        
        if(vertex2.y > vertex1.y){
            start_y = vertex1.y;
            end_y = vertex2.y;
        }
        else{
            start_y = vertex2.y;
            end_y = vertex1.y;
        }
                        
        for(int y=start_y; y<=end_y; ++y){
            for(int x=start_x; x<=end_x; ++x){

                draw_bloco(wMap,data->renderer,x,y,wMap->wTiles->tag_selected->tile_selected);
                
                if(x >= 0 && y >= 0 && x < wMap->properties->width_map && y < wMap->properties->height_map){
                    wMap->wCamadas->tag_atual->matriz[y][x].buffer = wMap->wTiles->tag_selected->tile_selected->id_local;
                }
            }
        }

        for(int y=start_y; y<=end_y; ++y){
            for(int x=start_x; x<=end_x; ++x){
                draw_cross_hair(wMap,data->renderer,x,y);
            }
        }
    }
}

void mod_clear_rect_update(Window_Map *wMap,Data *data){
    if(!wMap->wCamadas->tag_atual || !wMap->wTiles->tag_selected ||
       data->window_file || data->window_create || data->window_new_tile_set || data->window_project->modal_visible) return;

    if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT && collidePoint(data->mouse,&wMap->map_rect) && collidePointMap(data->mouse,wMap->properties)){
        wMap->shape_buffer[0].x = wMap->shape_buffer[1].x = wMap->properties->pos_x;
        wMap->shape_buffer[0].y = wMap->shape_buffer[1].y = wMap->properties->pos_y;
    }
    else if(data->event->type == SDL_MOUSEBUTTONUP && data->event->button.button == SDL_BUTTON_LEFT){
        int y = 0;
        int x = 0;
        Bloco **matriz = wMap->wCamadas->tag_atual->matriz;
        Element *elements = NULL;
        
        for(int i=0; i<wMap->properties->size; ++i){
            if(matriz[y][x].buffer == -2){
                elements = push_element(elements,x,y,get_tag_tile(wMap->wTiles->tag_tiles,matriz[y][x].tile),matriz[y][x].tile);
                matriz[y][x].tile = NULL;
                matriz[y][x].tag_tile = NULL;
            }
            matriz[y][x].buffer = (matriz[y][x].tile) ? matriz[y][x].tile->id_local : -1;
            
            ++x;
            if(x>=wMap->properties->width_map){
                x=0;
                ++y;
            }
        }

        if(elements){
            wMap->undo = push_stack(wMap->undo,elements,wMap->wCamadas->tag_atual,TYPE_ADD);
            wMap->redo = free_stack(wMap->redo);
            wMap->updated = false;
        }

        wMap->shape_buffer[0].x = wMap->shape_buffer[0].y = -1;
    }

    if(data->mouse->press_left && wMap->shape_buffer[0].x >= 0){

        wMap->shape_buffer[1].x = wMap->properties->pos_x;
        wMap->shape_buffer[1].y = wMap->properties->pos_y;

        clear_buffer_matriz(wMap);

        SDL_Point vertex1 = wMap->shape_buffer[0];
        SDL_Point vertex2 = wMap->shape_buffer[1];
        int start_x;
        int start_y;
        int end_x,end_y;
        int inc_x,inc_y;
                        
        if(vertex2.x > vertex1.x){
            start_x = vertex1.x;
            end_x = vertex2.x;
        }
        else{
            start_x = vertex2.x;
            end_x = vertex1.x;
        }
                        
        if(vertex2.y > vertex1.y){
            start_y = vertex1.y;
            end_y = vertex2.y;
        }
        else{
            start_y = vertex2.y;
            end_y = vertex1.y;
        }
                        
        for(int y=start_y; y<=end_y; ++y){
            for(int x=start_x; x<=end_x; ++x){
                
                draw_cross_hair(wMap,data->renderer,x,y);

                if(x >= 0 && y >= 0 && x < wMap->properties->width_map && y < wMap->properties->height_map){
                    if(wMap->wCamadas->tag_atual->matriz[y][x].tile) wMap->wCamadas->tag_atual->matriz[y][x].buffer = -2;
                }
            }
        }
    }
}
