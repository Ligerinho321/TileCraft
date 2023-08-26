#include <window_create_map.h>

void create_manager_input(Input *input,SDL_Event *event,Mouse *mouse){
    if(collidePoint(mouse,&input->rect) && !input->on){
        input->selected = true;
    }
    else{
        input->selected = false;
    }

    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT){
        if(collidePoint(mouse,&input->rect)){
            input->on = true;
            input->time = SDL_GetTicks();
            input->visible = true;
        }
        else{
            if(input->buffer == NULL){
                input->buffer = new_string(input->text);
            }
            input->on = false;
        }
    }

    if(input->on){
        if(event->type == SDL_TEXTINPUT && isdigit(event->text.text[0])){
            input->buffer = append(input->buffer,event->text.text[0],input->max_character);
            input->visible = true;
            input->time = SDL_GetTicks();
        }
        else if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE){
            input->buffer = pop(input->buffer);
            input->visible = true;
            input->time = SDL_GetTicks();
        }
    }
}

void create_init(Window_Create *window_create){
    window_create->open = true;

    window_create->ok.selected = false;
    window_create->ok.text = new_string("ok");

    window_create->cancel.selected = false;
    window_create->cancel.text = new_string("cancel");

    init_input(&window_create->width_map_input,"50",TYPE_NUMBER);
    init_input(&window_create->height_map_input,"50",TYPE_NUMBER);
    init_input(&window_create->width_block_input,"50",TYPE_NUMBER);
    init_input(&window_create->height_block_input,"50",TYPE_NUMBER);

    window_create->create_map_text = new_string("Create map");

    window_create->orientation_text = new_string("Orientation");
    window_create->isometric_text = new_string("isometric");
    window_create->orthogonal_text = new_string("orthogonal");

    window_create->map_size_text = new_string("Map size");
    window_create->map_width_text = new_string("width");
    window_create->map_height_text = new_string("height");

    window_create->block_size_text = new_string("Block size");
    window_create->block_width_text = new_string("width");
    window_create->block_height_text = new_string("height");

    window_create->current_orientation = TYPE_ISOMETRIC;
    window_create->is_selection = false;
}

void create_config_rect(Window_Create *window_create,Settings *setting,int screen_w,int screen_h){

    //rect
    window_create->rect.w = screen_w*CREATE_WIDTH_PERCENT;
    window_create->rect.h = screen_h*CREATE_HEIGHT_PERCENT;
    window_create->rect.x = screen_w*0.5-window_create->rect.w*0.5;
    window_create->rect.y = screen_h*0.5-window_create->rect.h*0.5;

    //crate map rect
    get_size_text(setting->font,window_create->create_map_text,&window_create->create_map.w,NULL);
    window_create->create_map.x = window_create->rect.x+SPACING;
    window_create->create_map.y = window_create->rect.y+SPACING;
    window_create->create_map.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    //orientation text
    get_size_text(setting->font,window_create->orientation_text,&window_create->orientation.w,NULL);
    window_create->orientation.x = window_create->rect.x+SPACING;
    window_create->orientation.y = window_create->create_map.y+window_create->create_map.h+SPACING;
    window_create->orientation.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    //selection orientation rect
    window_create->selection_orientation.x = window_create->rect.x+window_create->rect.w*0.3;
    window_create->selection_orientation.y = window_create->orientation.y;
    window_create->selection_orientation.w = window_create->rect.w*CREATE_WIDTH_ELEMENT_PERCENT;
    window_create->selection_orientation.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    window_create->vertex_rect.w = window_create->selection_orientation.w*0.16;
    window_create->vertex_rect.h = window_create->selection_orientation.h-12;

    window_create->vertex_rect.x = (window_create->selection_orientation.x+window_create->selection_orientation.w)-window_create->vertex_rect.w-3;
    window_create->vertex_rect.y = window_create->selection_orientation.y+6;

    SDL_Vertex vertex1 = {{window_create->vertex_rect.x+window_create->vertex_rect.w*0.2,window_create->vertex_rect.y+window_create->vertex_rect.h*0.2},setting->border_color,{1,1}};
    SDL_Vertex vertex2 = {{window_create->vertex_rect.x+window_create->vertex_rect.w*0.5,window_create->vertex_rect.y+window_create->vertex_rect.h*0.8},setting->border_color,{1,1}};
    SDL_Vertex vertex3 = {{window_create->vertex_rect.x+window_create->vertex_rect.w*0.8,window_create->vertex_rect.y+window_create->vertex_rect.h*0.2},setting->border_color,{1,1}};

    window_create->vertex[0] = vertex1;
    window_create->vertex[1] = vertex2;
    window_create->vertex[2] = vertex3;

    window_create->orthogonal_rect = window_create->selection_orientation;

    window_create->isometric_rect.x = window_create->orthogonal_rect.x;
    window_create->isometric_rect.y = window_create->orthogonal_rect.y+window_create->orthogonal_rect.h-1;
    window_create->isometric_rect.w = window_create->orthogonal_rect.w;
    window_create->isometric_rect.h = window_create->orthogonal_rect.h;

    get_size_text(setting->font,window_create->map_size_text,&window_create->map_size.w,NULL);
    window_create->map_size.x = window_create->rect.x+SPACING;
    window_create->map_size.y = window_create->orientation.y+window_create->orientation.h+SPACING;
    window_create->map_size.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    get_size_text(setting->font,window_create->map_width_text,&window_create->map_width.w,NULL);
    window_create->map_width.x = window_create->rect.x+SPACING*2;
    window_create->map_width.y = window_create->map_size.y+window_create->map_size.h+SPACING;
    window_create->map_width.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    window_create->width_map_input.rect.x = window_create->rect.x+window_create->rect.w*0.3;
    window_create->width_map_input.rect.y = window_create->map_width.y;
    window_create->width_map_input.rect.w = window_create->rect.w*CREATE_WIDTH_ELEMENT_PERCENT;
    window_create->width_map_input.rect.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    get_size_text(setting->font,window_create->map_height_text,&window_create->map_height.w,NULL);
    window_create->map_height.x = window_create->rect.x+SPACING*2;
    window_create->map_height.y = window_create->map_width.y+window_create->map_width.h+SPACING;
    window_create->map_height.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    window_create->height_map_input.rect.x = window_create->rect.x+window_create->rect.w*0.3;
    window_create->height_map_input.rect.y = window_create->map_height.y;
    window_create->height_map_input.rect.w = window_create->rect.w*CREATE_WIDTH_ELEMENT_PERCENT;
    window_create->height_map_input.rect.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    get_size_text(setting->font,window_create->block_size_text,&window_create->block_size.w,NULL);
    window_create->block_size.x = window_create->rect.x+SPACING;
    window_create->block_size.y = window_create->map_height.y+window_create->map_height.h+SPACING;
    window_create->block_size.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    get_size_text(setting->font,window_create->block_width_text,&window_create->block_width.w,NULL);
    window_create->block_width.x = window_create->rect.x+SPACING*2;
    window_create->block_width.y = window_create->block_size.y+window_create->block_size.h+SPACING;
    window_create->block_width.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    window_create->width_block_input.rect.x = window_create->rect.x+window_create->rect.w*0.3;
    window_create->width_block_input.rect.y = window_create->block_width.y;
    window_create->width_block_input.rect.w = window_create->rect.w*CREATE_WIDTH_ELEMENT_PERCENT;
    window_create->width_block_input.rect.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    get_size_text(setting->font,window_create->block_height_text,&window_create->block_height.w,NULL);
    window_create->block_height.x = window_create->rect.x+SPACING*2;
    window_create->block_height.y = window_create->block_width.y+window_create->block_width.h+SPACING;
    window_create->block_height.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    window_create->height_block_input.rect.x = window_create->rect.x+window_create->rect.w*0.3;
    window_create->height_block_input.rect.y = window_create->block_height.y;
    window_create->height_block_input.rect.w = window_create->rect.w*CREATE_WIDTH_ELEMENT_PERCENT;
    window_create->height_block_input.rect.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;

    window_create->cancel.rect.w = window_create->rect.w*CREATE_BUTTON_WIDTH_PERCENT;
    window_create->cancel.rect.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;
    window_create->cancel.rect.x = (window_create->rect.x+window_create->rect.w)-window_create->cancel.rect.w-SPACING;
    window_create->cancel.rect.y = (window_create->rect.y+window_create->rect.h)-window_create->cancel.rect.h-SPACING;

    window_create->ok.rect.w = window_create->rect.w*CREATE_BUTTON_WIDTH_PERCENT;
    window_create->ok.rect.h = window_create->rect.h*CREATE_HEIGHT_ELEMENT_PERCENT;
    window_create->ok.rect.x = window_create->cancel.rect.x-window_create->ok.rect.w-SPACING;
    window_create->ok.rect.y = window_create->cancel.rect.y;

}

void create_run(Data *data){

    create_input(data);
    create_draw(data->window_create,data->setting,data->renderer);

    if(data->window_create->open == false){
        create_free(&data->window_create);
    }
}

void create_input(Data *data){

    Window_Create *window_create = data->window_create;

    create_manager_input(&window_create->width_map_input,data->event,data->mouse);
    create_manager_input(&window_create->height_map_input,data->event,data->mouse);
    create_manager_input(&window_create->width_block_input,data->event,data->mouse);
    create_manager_input(&window_create->height_block_input,data->event,data->mouse);
    
    if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT){
        if(collidePoint(data->mouse,&window_create->ok.rect)){
            window_create->open = false;
            if(data->window_menu){
                data->window_menu->open = false;
            }
            
            Window_Map *window_map = calloc(1,sizeof(Window_Map));
            window_map->path = NULL;
            window_map->path_export = NULL;

            Properties *properties = calloc(1,sizeof(Properties));

            properties->width_map = atoi(window_create->width_map_input.buffer);
            properties->height_map = atoi(window_create->height_map_input.buffer);
            properties->height_block = atoi(window_create->height_block_input.buffer);
            properties->width_block =  atoi(window_create->width_block_input.buffer);
            properties->size = properties->width_map * properties->height_map;
            properties->type = window_create->current_orientation;

            if(window_create->current_orientation == TYPE_ISOMETRIC){
                properties->orientation = new_string("isometric");
            }
            else if(window_create->current_orientation == TYPE_ORTHOGONAL){
                properties->orientation = new_string("orthogonal");
            }

            map_init(window_map,data->setting,data->renderer,properties);
            create_tag_layer(window_map->wCamadas,data->setting);
            map_config_rect(window_map,data->setting);

            Tag *tag = calloc(1,sizeof(Tag));
            tag->name = new_string("Unnamed");
            tag->path = NULL;
            tag->element = (void*)window_map;
            tag->type = TYPE_MAP;

            if(!data->window_project){

                data->window_project = calloc(1,sizeof(Window_Project));
                project_init(data->window_project,data->setting,data->renderer,tag);
                project_config_rect(data->window_project,data->setting,data->screen_w,data->screen_h);
                
            }else{
                push_tag(data->window_project,data->setting,data->renderer,tag);
            }
        }
        else if(collidePoint(data->mouse,&window_create->cancel.rect)){
            window_create->open = false;
        }
        else if(collidePoint(data->mouse,&window_create->selection_orientation) && !window_create->is_selection){
            window_create->is_selection = true;
        }
        else if(collidePoint(data->mouse,&window_create->orthogonal_rect) && window_create->is_selection){
            window_create->current_orientation = TYPE_ORTHOGONAL;
            window_create->is_selection = false;
        }
        else if(collidePoint(data->mouse,&window_create->isometric_rect) && window_create->is_selection){
            window_create->current_orientation = TYPE_ISOMETRIC;
            window_create->is_selection = false;
        }

    }

    if(collidePoint(data->mouse,&window_create->ok.rect)){
        window_create->ok.selected = true;
    }
    else if(collidePoint(data->mouse,&window_create->cancel.rect)){
        window_create->cancel.selected = true;
    }
    else if(collidePoint(data->mouse,&window_create->selection_orientation) && !window_create->is_selection){
        window_create->selection_orientation_selected = true;
    }
    else if(collidePoint(data->mouse,&window_create->orthogonal_rect) && window_create->is_selection){
        window_create->orthogonal_selected = true;
    }
    else if(collidePoint(data->mouse,&window_create->isometric_rect) && window_create->is_selection){
        window_create->isometric_selected = true;
    }
}

void create_draw(Window_Create *window_create,Settings *setting,SDL_Renderer *renderer){
    SDL_Color text_color = setting->text_color;
    SDL_Color background_color = setting->background_color;
    SDL_Color border_color = setting->border_color;
    SDL_Color background_color_selected = setting->background_color_selected;
    TTF_Font *fonte = setting->font;

    SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
    SDL_RenderFillRect(renderer,&window_create->rect);

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&window_create->rect);

    //create map
    draw_text(renderer,fonte,window_create->create_map_text,text_color,window_create->create_map,true,true);

    //orientation
    draw_text(renderer,fonte,window_create->orientation_text,text_color,window_create->orientation,true,true);
    //selection orientation

    if(window_create->is_selection){
        SDL_SetRenderDrawColor(renderer,background_color_selected.r,background_color_selected.g,background_color_selected.b,background_color_selected.a);
        
        if(window_create->isometric_selected){
            SDL_RenderFillRect(renderer,&window_create->isometric_rect);
            window_create->isometric_selected = false;
        }
        else if(window_create->orthogonal_selected){
            SDL_RenderFillRect(renderer,&window_create->orthogonal_rect);
            window_create->orthogonal_selected = false;
        }

        SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);

        SDL_RenderDrawRect(renderer,&window_create->orthogonal_rect);
        draw_text(renderer,fonte,window_create->orthogonal_text,text_color,window_create->orthogonal_rect,false,true);

        SDL_RenderDrawRect(renderer,&window_create->isometric_rect);
        draw_text(renderer,fonte,window_create->isometric_text,text_color,window_create->isometric_rect,false,true);
    }
    else{
        SDL_SetRenderDrawColor(renderer,background_color_selected.r,background_color_selected.g,background_color_selected.b,background_color_selected.a);
        
        if(window_create->selection_orientation_selected){
            SDL_RenderFillRect(renderer,&window_create->selection_orientation);
            window_create->selection_orientation_selected = false;
        }

        switch(window_create->current_orientation){
            case TYPE_ISOMETRIC:
                draw_text(renderer,fonte,window_create->isometric_text,text_color,window_create->selection_orientation,false,true);
                break;
            case TYPE_ORTHOGONAL:
                draw_text(renderer,fonte,window_create->orthogonal_text,text_color,window_create->selection_orientation,false,true);
                break;
        }
        
        SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
        SDL_RenderGeometry(renderer,NULL,window_create->vertex,3,NULL,0);
        SDL_RenderDrawRect(renderer,&window_create->selection_orientation);
    }

    //map size
    draw_text(renderer,fonte,window_create->map_size_text,text_color,window_create->map_size,true,true);
    //map width
    draw_text(renderer,fonte,window_create->map_width_text,text_color,window_create->map_width,true,true);
    //map width input
    draw_input(renderer,&window_create->width_map_input,setting,text_color,border_color);
    //map height
    draw_text(renderer,fonte,window_create->map_height_text,text_color,window_create->map_height,true,true);
    //map height input
    draw_input(renderer,&window_create->height_map_input,setting,text_color,border_color);

    //block size
    draw_text(renderer,fonte,window_create->block_size_text,text_color,window_create->block_size,true,true);
    //block width
    draw_text(renderer,fonte,window_create->block_width_text,text_color,window_create->block_width,true,true);
    //block width input
    draw_input(renderer,&window_create->width_block_input,setting,text_color,border_color);
    //block height
    draw_text(renderer,fonte,window_create->block_height_text,text_color,window_create->block_height,true,true);
    //block height input
    draw_input(renderer,&window_create->height_block_input,setting,text_color,border_color);

    //button ok
    draw_button(renderer,&window_create->ok,setting,fonte,text_color,border_color);
    //button cancel
    draw_button(renderer,&window_create->cancel,setting,fonte,text_color,border_color);
}

void create_free(Window_Create **window_create){
    if(*window_create != NULL){

        free((*window_create)->orientation_text);
        free((*window_create)->isometric_text);
        free((*window_create)->orthogonal_text);

        free((*window_create)->cancel.text);
        free((*window_create)->ok.text);
        free((*window_create)->create_map_text);
        free((*window_create)->map_width_text);
        free((*window_create)->map_height_text);
        free((*window_create)->map_size_text);
        free((*window_create)->block_size_text);
        free((*window_create)->block_width_text);
        free((*window_create)->block_height_text);

        free(*window_create);
        *window_create = NULL;
    }
}