#include <window_layers.h>


void remove_block_in_layers(Window_Layers *window_layers,char *path_tile,char *path_tag_tile){
    Tag_Camadas *tagCamadas = window_layers->tags;

    while(tagCamadas != NULL){
        for(int y=0; y<window_layers->propriedades->height_map; ++y){
            for(int x=0; x<window_layers->propriedades->width_map; ++x){
                Bloco *bloco = &tagCamadas->matriz[y][x];

                if((path_tile && bloco->tile && bloco->tile->path && !strcmp(bloco->tile->path,path_tile)) ||
                   (path_tag_tile && bloco->tag_tile && bloco->tag_tile->path && !strcmp(bloco->tag_tile->path,path_tag_tile))){
                    
                    bloco->tile = NULL;
                    bloco->tag_tile = NULL;
                    bloco->buffer = -1;
                }

            }
        }
        tagCamadas = tagCamadas->next;
    }
}

void layers_update_rect(Window_Layers *window_layers,Settings *setting){
    Tag_Camadas *tag = window_layers->tags;
    Tag_Camadas *anterior = NULL;

    window_layers->horiScrollBar.lenght = 0;

    while(tag != NULL){
        tag->input_name.rect.x = 0;
        tag->input_name.rect.y = (anterior) ? anterior->input_name.rect.y+anterior->input_name.rect.h : 0;
        tag->input_name.rect.w = window_layers->subRect.w;
        tag->input_name.rect.h = window_layers->subRect.h*0.15;
        if(tag->input_name.buffer){
            get_size_text(setting->fontlower,tag->input_name.buffer,&tag->width,NULL);
        }
        else{
            tag->width = 0;
        }

        window_layers->horiScrollBar.lenght = (tag->width+LAYER_SPACING > window_layers->horiScrollBar.lenght) ? tag->width+LAYER_SPACING : window_layers->horiScrollBar.lenght;
        anterior = tag;
        tag = tag->next;
    }

    window_layers->vertScrollBar.lenght = (anterior) ? anterior->input_name.rect.y+anterior->input_name.rect.h : 0;

    calcSize(&window_layers->vertScrollBar,TYPE_VERTICAL);
    calcSize(&window_layers->horiScrollBar,TYPE_HORIZONTAL);

    if(window_layers->vertScrollBar.subRect.h >= window_layers->vertScrollBar.rect.h-4 || window_layers->vertScrollBar.subRect.h <= 0){
        window_layers->vertScrollBar.visible = false;
        window_layers->vertScrollBar.offset = 0;
        window_layers->subRect.w = window_layers->rect.w-LAYER_SPACING*2;
        window_layers->horiScrollBar.rect.w = window_layers->subRect.w;
    }
    else{
        window_layers->vertScrollBar.visible = true;
        window_layers->subRect.w = window_layers->vertScrollBar.rect.x-window_layers->subRect.x-1;
        window_layers->horiScrollBar.rect.w = window_layers->subRect.w;
    }

    if(window_layers->horiScrollBar.subRect.w >= window_layers->horiScrollBar.rect.w-4 || window_layers->horiScrollBar.subRect.w <= 0){
        window_layers->horiScrollBar.visible = false;
        window_layers->horiScrollBar.offset = 0;
        window_layers->subRect.h = (window_layers->rect.y+window_layers->rect.h)-window_layers->subRect.y-LAYER_SPACING;
        window_layers->vertScrollBar.rect.h = window_layers->subRect.h;
    }
    else{
        window_layers->horiScrollBar.visible = true;
        window_layers->subRect.h = window_layers->horiScrollBar.rect.y-window_layers->subRect.y-1;
        window_layers->vertScrollBar.rect.h = window_layers->subRect.h;
    }

    calcSize(&window_layers->vertScrollBar,TYPE_VERTICAL);
    calcSize(&window_layers->horiScrollBar,TYPE_HORIZONTAL);
}

void layers_manager_input(Window_Layers *window_layers,Settings *setting,SDL_Event *event,Mouse *mouse){

    Tag_Camadas *tag_camada = window_layers->tags;
    while(tag_camada){
        if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT){
            if(collidePoint(mouse,&window_layers->subRect) && collidePointOffset(mouse,&tag_camada->input_name.rect,0,window_layers->vertScrollBar.offset,&window_layers->subRect)){
                window_layers->tag_atual = tag_camada;
                
                if(tag_camada->clicked && SDL_GetTicks() - tag_camada->time <= COLLDOWN_DOUBLE_CLICK){
                    tag_camada->input_name.on = true;
                    tag_camada->clicked = false;
                }
                else{
                    tag_camada->clicked = true;
                }
                tag_camada->time = SDL_GetTicks();

            }
            else{
                tag_camada->input_name.on = false;

                if(lenght(tag_camada->input_name.buffer)){
                    free(tag_camada->input_name.text);
                    tag_camada->input_name.text = new_string(tag_camada->input_name.buffer);
                }
                else{
                    tag_camada->input_name.buffer = new_string(tag_camada->input_name.text);
                }
            }
        }
        else if(event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_RETURN){
            tag_camada->input_name.on = false;

            if(lenght(tag_camada->input_name.buffer)){
                free(tag_camada->input_name.text);
                tag_camada->input_name.text = new_string(tag_camada->input_name.buffer);
            }
            else{
                tag_camada->input_name.buffer = new_string(tag_camada->input_name.text);
            }
        }
        else if(tag_camada->input_name.on){

            if(event->type == SDL_TEXTINPUT){
                tag_camada->input_name.buffer = append(tag_camada->input_name.buffer,event->text.text[0],MAX_STRING);
                tag_camada->input_name.visible = true;
                tag_camada->input_name.time = SDL_GetTicks();
            }
            else if(event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE){
                tag_camada->input_name.buffer = pop(tag_camada->input_name.buffer);
                tag_camada->input_name.visible = true;
                tag_camada->input_name.time = SDL_GetTicks();
            }

            if(tag_camada->input_name.buffer){
                layers_update_rect(window_layers,setting);

                float displacement_x = ((float)tag_camada->width) / ((float)window_layers->horiScrollBar.lenght);
                int new_pos_x = displacement_x * window_layers->horiScrollBar.rect.w;
                window_layers->horiScrollBar.subRect.x = window_layers->horiScrollBar.rect.x + (new_pos_x - window_layers->horiScrollBar.subRect.w*0.5f);

                float displacement_y = ((float)tag_camada->input_name.rect.y+tag_camada->input_name.rect.h) / ((float)window_layers->vertScrollBar.lenght);
                int new_pos_y = displacement_y * window_layers->vertScrollBar.rect.h;
                window_layers->vertScrollBar.subRect.y = window_layers->vertScrollBar.rect.y + (new_pos_y - window_layers->vertScrollBar.subRect.h*0.5f);

            }
        }
        tag_camada = tag_camada->next;
    }
}

void switch_tag_layer(Window_Layers *window_layers,int type){

    Tag_Camadas *atual = window_layers->tags;

    while((atual != window_layers->tag_atual && atual != NULL)){
        atual = atual->next;
    }

    if(atual != NULL){
        if(type == TYPE_UP && atual->previous != NULL){

            Tag_Camadas *previous = atual->previous;

            if(window_layers->tags == previous){
                window_layers->tags = atual;
            }

            if(atual->next) atual->next->previous = previous;
            if(previous->previous) previous->previous->next = atual;

            previous->next = atual->next;
            atual->next = previous;
            atual->previous = previous->previous;
            previous->previous = atual;
        }
        else if(type == TYPE_DOWN && atual->next != NULL){

            Tag_Camadas *next = atual->next;

            if(window_layers->tags == atual){
                window_layers->tags = next;
            }

            if(atual->previous) atual->previous->next = next;
            if(next->next) next->next->previous = atual;

            next->previous = atual->previous;
            atual->previous = next;
            atual->next = next->next;
            next->next = atual;
        }
    }
}

void create_tag_layer(Window_Layers *window_layers,Settings *setting){

    Tag_Camadas *tags = window_layers->tags;

    Tag_Camadas *tag = calloc(1,sizeof(Tag_Camadas));
    tag->clicked = false;
    tag->time = 0;
    tag->next = NULL;
    tag->previous = NULL;

    tag->matriz = calloc(window_layers->propriedades->height_map,sizeof(Bloco*));

    for(int y=0; y<window_layers->propriedades->height_map; y++){
        tag->matriz[y] = calloc(window_layers->propriedades->width_map,sizeof(Bloco));
        for(int x=0; x<window_layers->propriedades->width_map; x++){
            Bloco *bloco = &tag->matriz[y][x];
            bloco->x = x;
            bloco->y = y;
            bloco->tile = NULL;
            bloco->tag_tile = NULL;
            bloco->buffer = 0;
        }
    }

    char name_buffer[MAX_STRING];
    memset(name_buffer,'\0',MAX_STRING);

    if(!tags){
        tag->id = 1;
        sprintf(name_buffer,"layer_%d",tag->id);
        init_input(&tag->input_name,name_buffer,TYPE_TEXT);

        window_layers->tags = tag;
        window_layers->tag_atual = tag;
    }
    else{
        while(tags->next != NULL){
            tags = tags->next;
        }

        tag->id = tags->id+1;
        sprintf(name_buffer,"layer_%d",tag->id);
        init_input(&tag->input_name,name_buffer,TYPE_TEXT);

        tag->previous = tags;
        tags->next = tag;
    }
}

void remove_tag_layer(Window_Layers *window_layers,Window_Map *wMap){
    Tag_Camadas *tag = window_layers->tags;

    while((tag != window_layers->tag_atual && tag != NULL)){
        tag = tag->next;
    }

    if(tag != NULL){
        if(tag->previous != NULL){
            tag->previous->next = tag->next;
            if(tag->next) tag->next->previous = tag->previous;
            window_layers->tag_atual = tag->previous;
        }
        else if(tag->next){
            window_layers->tags = tag->next;
            if(tag->next) tag->next->previous = NULL;
            window_layers->tag_atual = tag->next;
        }
        else{
            window_layers->tag_atual = NULL;
            window_layers->tags = NULL;
        }

        remove_tag_camada_from_stack(&wMap->redo,tag);
        remove_tag_camada_from_stack(&wMap->undo,tag);

        for(int i=0; i<window_layers->propriedades->height_map; ++i){
            free(tag->matriz[i]);
        }
        free_input(&tag->input_name);
        free(tag->matriz);

        free(tag);
    }
}


void layers_init(Window_Layers *window_layers,Settings *setting,Properties *propriedades){
    window_layers->propriedades = propriedades;
    window_layers->tags = NULL;
    window_layers->tag_atual = NULL;

    window_layers->layers_text = new_string("Layers");

    window_layers->vertScrollBar.lenght = 0;
    window_layers->vertScrollBar.mouseDiff = 0;
    window_layers->vertScrollBar.offset = 0;
    window_layers->vertScrollBar.on_bar = false;
    window_layers->vertScrollBar.visible = false;

    window_layers->horiScrollBar.lenght = 0;
    window_layers->horiScrollBar.mouseDiff = 0;
    window_layers->horiScrollBar.offset = 0;
    window_layers->horiScrollBar.on_bar = false;
    window_layers->horiScrollBar.visible = false;

    window_layers->up_selected = false;
    window_layers->down_selected = false;
    window_layers->add_selected = false;
    window_layers->eraser_selected = false;
}

void layers_config_rect(Window_Layers *window_layers,Settings *setting){

    get_size_text(setting->fontlower,window_layers->layers_text,&window_layers->layers.w,NULL);
    window_layers->layers.x = window_layers->rect.x+LAYER_SPACING;
    window_layers->layers.y = window_layers->rect.y+LAYER_SPACING;
    window_layers->layers.h = window_layers->rect.h*LAYER_HEIGHT_ELEMENT_PERCENT;

    window_layers->down.w = window_layers->rect.w*LAYER_BUTTON_WIDTH_PERCENT;
    window_layers->down.h = window_layers->rect.h*LAYER_HEIGHT_ELEMENT_PERCENT;
    window_layers->down.x = (window_layers->rect.x+window_layers->rect.w)-window_layers->down.w-LAYER_SPACING;
    window_layers->down.y = window_layers->rect.y+LAYER_SPACING;

    SDL_Vertex vertex1 = {{window_layers->down.x+window_layers->down.w*0.2f,window_layers->down.y+window_layers->down.h*0.2f},setting->border_color,{1,1}};
    SDL_Vertex vertex2 = {{window_layers->down.x+window_layers->down.w*0.5f,window_layers->down.y+window_layers->down.h*0.8f},setting->border_color,{1,1}};
    SDL_Vertex vertex3 = {{window_layers->down.x+window_layers->down.w*0.8f,window_layers->down.y+window_layers->down.h*0.2f},setting->border_color,{1,1}};

    window_layers->down_vertices[0] = vertex1;
    window_layers->down_vertices[1] = vertex2;
    window_layers->down_vertices[2] = vertex3;

    window_layers->up.w = window_layers->rect.w*LAYER_BUTTON_WIDTH_PERCENT;
    window_layers->up.h = window_layers->rect.h*LAYER_HEIGHT_ELEMENT_PERCENT;
    window_layers->up.x = window_layers->down.x-window_layers->up.w-LAYER_SPACING;
    window_layers->up.y = window_layers->rect.y+LAYER_SPACING;

    SDL_Vertex vertex4 = {{window_layers->up.x+window_layers->up.w*0.5f,window_layers->up.y+window_layers->up.h*0.2f},setting->border_color,{1,1}};
    SDL_Vertex vertex5 = {{window_layers->up.x+window_layers->up.w*0.2f,window_layers->up.y+window_layers->up.h*0.8f},setting->border_color,{1,1}};
    SDL_Vertex vertex6 = {{window_layers->up.x+window_layers->up.w*0.8f,window_layers->up.y+window_layers->up.h*0.8f},setting->border_color,{1,1}};

    window_layers->up_vertices[0] = vertex4;
    window_layers->up_vertices[1] = vertex5;
    window_layers->up_vertices[2] = vertex6;

    window_layers->eraser.w = window_layers->rect.w*LAYER_BUTTON_WIDTH_PERCENT;
    window_layers->eraser.h = window_layers->rect.h*LAYER_HEIGHT_ELEMENT_PERCENT;
    window_layers->eraser.x = window_layers->up.x-window_layers->eraser.w-LAYER_SPACING;
    window_layers->eraser.y = window_layers->rect.y+LAYER_SPACING;

    window_layers->eraser_rect_1.w = window_layers->eraser.w*0.6f;
    window_layers->eraser_rect_1.h = window_layers->eraser.h*0.2f;
    window_layers->eraser_rect_1.x = window_layers->eraser.x+window_layers->eraser.w*0.2f;
    window_layers->eraser_rect_1.y = (window_layers->eraser.y+window_layers->eraser.h*0.5f)-window_layers->eraser_rect_1.h*0.5f;

    window_layers->add.w = window_layers->rect.w*LAYER_BUTTON_WIDTH_PERCENT;
    window_layers->add.h = window_layers->rect.h*LAYER_HEIGHT_ELEMENT_PERCENT;
    window_layers->add.x = window_layers->eraser.x-window_layers->add.w-LAYER_SPACING;
    window_layers->add.y = window_layers->rect.y+LAYER_SPACING;

    window_layers->add_rect_1.w = window_layers->add.w*0.6f;
    window_layers->add_rect_1.h = window_layers->add.h*0.2f;
    window_layers->add_rect_1.x = window_layers->add.x+window_layers->add.w*0.2f;
    window_layers->add_rect_1.y = (window_layers->add.y+window_layers->add.h*0.5f)-window_layers->add_rect_1.h*0.5f;

    window_layers->add_rect_2.w = window_layers->add.w*0.2f;
    window_layers->add_rect_2.h = window_layers->add.h*0.6f;
    window_layers->add_rect_2.x = (window_layers->add.x+window_layers->add.w*0.5f)-window_layers->add_rect_2.w*0.5f;
    window_layers->add_rect_2.y = window_layers->add.y+window_layers->add.h*0.2f;

    window_layers->subRect.x = window_layers->rect.x+LAYER_SPACING;
    window_layers->subRect.y = window_layers->layers.y+window_layers->layers.h+LAYER_SPACING;

    window_layers->vertScrollBar.rect.w = window_layers->rect.w*LAYER_SIZE_SCROLL_BAR_PERCENT;
    window_layers->vertScrollBar.rect.x = (window_layers->rect.x+window_layers->rect.w)-window_layers->vertScrollBar.rect.w-LAYER_SPACING;
    window_layers->vertScrollBar.rect.y = window_layers->subRect.y;

    window_layers->vertScrollBar.subRect.x = window_layers->vertScrollBar.rect.x+SCROLL_BAR_SPACING;
    window_layers->vertScrollBar.subRect.y = window_layers->vertScrollBar.rect.y+SCROLL_BAR_SPACING;
    window_layers->vertScrollBar.subRect.w = window_layers->vertScrollBar.rect.w-SCROLL_BAR_SPACING*2;

    window_layers->horiScrollBar.rect.h = window_layers->rect.w*LAYER_SIZE_SCROLL_BAR_PERCENT;
    window_layers->horiScrollBar.rect.x = window_layers->rect.x+LAYER_SPACING;
    window_layers->horiScrollBar.rect.y = (window_layers->rect.y+window_layers->rect.h)-window_layers->horiScrollBar.rect.h-LAYER_SPACING;

    window_layers->horiScrollBar.subRect.x = window_layers->horiScrollBar.rect.x+SCROLL_BAR_SPACING;
    window_layers->horiScrollBar.subRect.y = window_layers->horiScrollBar.rect.y+SCROLL_BAR_SPACING;
    window_layers->horiScrollBar.subRect.h = window_layers->horiScrollBar.rect.h-SCROLL_BAR_SPACING*2;

}

void layers_run(Window_Layers *window_layers,Window_Map *wMap,Data *data){
    layers_manager_input(window_layers,data->setting,data->event,data->mouse);
    layers_input(window_layers,wMap,data);

    ScrollBarUpdate(&window_layers->vertScrollBar,TYPE_VERTICAL,data->mouse);
    ScrollBarUpdate(&window_layers->horiScrollBar,TYPE_HORIZONTAL,data->mouse);

    layers_update_rect(window_layers,data->setting);
    layers_draw(window_layers,data->setting,data->renderer);
}

void layers_draw(Window_Layers *window_layers,Settings *setting,SDL_Renderer *renderer){
    SDL_Color text_color = setting->text_color;
    SDL_Color background_color = setting->background_color;
    SDL_Color border_color = setting->border_color;
    SDL_Color bg_color_selected = setting->background_color_selected;
    TTF_Font *font = setting->fontlower;
    Tag_Camadas *tag = window_layers->tags;

    draw_text(renderer,font,window_layers->layers_text,text_color,window_layers->layers,true,true);

    SDL_SetRenderDrawColor(renderer,bg_color_selected.r,bg_color_selected.g,bg_color_selected.b,bg_color_selected.a);
    
    if(window_layers->add_selected){
        SDL_RenderFillRect(renderer,&window_layers->add);
        window_layers->add_selected = false;
    }
    else if(window_layers->eraser_selected){
        SDL_RenderFillRect(renderer,&window_layers->eraser);
        window_layers->eraser_selected = false;
    }
    else if(window_layers->up_selected){
        SDL_RenderFillRect(renderer,&window_layers->up);
        window_layers->up_selected = false;
    }
    else if(window_layers->down_selected){
        SDL_RenderFillRect(renderer,&window_layers->down);
        window_layers->down_selected = false;
    }
    
    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);

    //down
    SDL_RenderDrawRect(renderer,&window_layers->down);
    SDL_RenderGeometry(renderer,NULL,window_layers->down_vertices,3,NULL,0);

    //up
    SDL_RenderDrawRect(renderer,&window_layers->up);
    SDL_RenderGeometry(renderer,NULL,window_layers->up_vertices,3,NULL,0);

    //eraser
    SDL_RenderDrawRect(renderer,&window_layers->eraser);
    SDL_RenderFillRect(renderer,&window_layers->eraser_rect_1);

    //add
    SDL_RenderDrawRect(renderer,&window_layers->add);
    SDL_RenderFillRect(renderer,&window_layers->add_rect_1);
    SDL_RenderFillRect(renderer,&window_layers->add_rect_2);

    SDL_RenderSetViewport(renderer,&window_layers->subRect);
    
    while(tag != NULL){

        if(tag->input_name.on){
            if(SDL_GetTicks() - tag->input_name.time >= COLLDOWN_INPUT){
                tag->input_name.time = SDL_GetTicks();
                tag->input_name.visible = !tag->input_name.visible;
            }
        }
        else{
            tag->input_name.visible = false;
        }

        SDL_Rect rect = {
            tag->input_name.rect.x+window_layers->horiScrollBar.offset,
            tag->input_name.rect.y+window_layers->vertScrollBar.offset,
            tag->input_name.rect.w+window_layers->horiScrollBar.lenght,
            tag->input_name.rect.h
        };

        if(window_layers->tag_atual && tag == window_layers->tag_atual){
            SDL_SetRenderDrawColor(renderer,bg_color_selected.r,bg_color_selected.g,bg_color_selected.b,bg_color_selected.a);
            SDL_RenderFillRect(renderer,&rect);
        }
        if(lenght(tag->input_name.buffer)){
            draw_text(renderer,setting->fontlower,tag->input_name.buffer,text_color,rect,false,true);
        }

        if(tag->input_name.visible){
            SDL_Point ponto1;
            SDL_Point ponto2;
            if(tag->input_name.buffer){
                ponto1.x = window_layers->horiScrollBar.offset+tag->width+2;
                ponto1.y = rect.y+rect.h*0.1;
                ponto2.x = window_layers->horiScrollBar.offset+tag->width+2;
                ponto2.y = rect.y+rect.h*0.9;
            }
            else{
                ponto1.x = 2;
                ponto1.y = rect.y+rect.h*0.1;
                ponto2.x = 2;
                ponto2.y = rect.y+rect.h*0.9;
            }

            SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
            SDL_RenderDrawLine(renderer,ponto1.x,ponto1.y,ponto2.x,ponto2.y);
        }
        tag = tag->next;
    }

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,NULL);

    SDL_RenderSetViewport(renderer,NULL);

    //vertical scroll bar
    draw_scroll_bar(&window_layers->vertScrollBar,setting,renderer);

    //horizontal scroll bar
    draw_scroll_bar(&window_layers->horiScrollBar,setting,renderer);

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&window_layers->rect);
}

void layers_input(Window_Layers *window_layers,Window_Map *wMap,Data *data){

    if(data->window_create || data->window_file || data->window_new_tile_set || data->window_project->modal_visible) return;

    if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT){

        if(collidePoint(data->mouse,&window_layers->vertScrollBar.subRect) && window_layers->vertScrollBar.visible){
            window_layers->vertScrollBar.on_bar = true;
            window_layers->vertScrollBar.mouseDiff = data->mouse->pos_y - window_layers->vertScrollBar.subRect.y;
        }
        else if(collidePoint(data->mouse,&window_layers->horiScrollBar.subRect) && window_layers->horiScrollBar.visible){
            window_layers->horiScrollBar.on_bar = true;
            window_layers->horiScrollBar.mouseDiff = data->mouse->pos_x - window_layers->horiScrollBar.subRect.x;
        }
        else if(collidePoint(data->mouse,&window_layers->add)){
            create_tag_layer(window_layers,data->setting);
        }
        else if(collidePoint(data->mouse,&window_layers->eraser) && window_layers->tag_atual){
            remove_tag_layer(window_layers,wMap);
        }
        else if(collidePoint(data->mouse,&window_layers->up) && window_layers->tag_atual){
            switch_tag_layer(window_layers,TYPE_UP);
        }
        else if(collidePoint(data->mouse,&window_layers->down) && window_layers->tag_atual){
            switch_tag_layer(window_layers,TYPE_DOWN);
        }
    }
    else if(data->event->type == SDL_MOUSEBUTTONUP && data->event->button.button == SDL_BUTTON_LEFT){
        window_layers->vertScrollBar.on_bar = false;
        window_layers->horiScrollBar.on_bar = false;
    }
    else if(collidePoint(data->mouse,&window_layers->rect)){
        if(collidePoint(data->mouse,&window_layers->vertScrollBar.subRect)){
            window_layers->vertScrollBar.selected = true;
        }
        else if(collidePoint(data->mouse,&window_layers->horiScrollBar.subRect)){
            window_layers->horiScrollBar.selected = true;
        } 
    }
        
    if(collidePoint(data->mouse,&window_layers->vertScrollBar.subRect) && window_layers->vertScrollBar.visible){
        window_layers->vertScrollBar.selected = true;
    }
    else if(collidePoint(data->mouse,&window_layers->horiScrollBar.subRect) && window_layers->horiScrollBar.visible){
        window_layers->horiScrollBar.selected = true;
    }
    else if(collidePoint(data->mouse,&window_layers->add)){
        window_layers->add_selected = true;
    }
    else if(collidePoint(data->mouse,&window_layers->eraser)){
        window_layers->eraser_selected = true;
    }
    else if(collidePoint(data->mouse,&window_layers->up)){
        window_layers->up_selected = true;
    }
    else if(collidePoint(data->mouse,&window_layers->down)){
        window_layers->down_selected = true;
    }
}

void layers_free(Window_Layers **window_layers){
    Tag_Camadas *tag = (*window_layers)->tags;
    Tag_Camadas *aux;

    while(tag != NULL){
        aux = tag->next;
        for(int i=0; i<(*window_layers)->propriedades->height_map; i++){
            free(tag->matriz[i]);
        }
        free(tag->matriz);
        free_input(&tag->input_name);
        free(tag);
        tag = aux;
    }
    
    (*window_layers)->tags = NULL;

    free((*window_layers)->layers_text);
    free(*window_layers);
    *window_layers = NULL;
}
