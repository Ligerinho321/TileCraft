#include <window_new_tile_set.h>

#define NEW_TILE_WIDTH_PERCENT 0.5
#define NEW_TILE_HEIGHT_PERCENT 0.5
#define NEW_TILE_HEIGHT_ELEMENT_PERCENT 0.09
#define NEW_TILE_WIDTH_BUTTON 0.2
#define NEW_TILE_WIDTH_INPUT_NUMBER 0.2

void new_tile_manager_input(Input *input,SDL_Event *event,Mouse *mouse){
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
            if(!input->buffer && input->text){
                input->buffer = new_string(input->text);
            }
            input->on = false;
        }
    }

    if(input->on){
        if(event->type == SDL_TEXTINPUT && ((input->type == TYPE_NUMBER && isdigit(event->text.text[0])) || input->type == TYPE_TEXT)){
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

void new_tile_init(Window_New_Tile_Set *newTile){

    newTile->open = true;

    newTile->tile_set_text = new_string("Tile set");

    newTile->name_text = new_string("name:");
    newTile->type_text = new_string("type:");

    newTile->image_text = new_string("Image");

    newTile->path_text = new_string("path:");
    newTile->tile_width_text = new_string("tile width:");
    newTile->tile_height_text = new_string("tile height:");
    newTile->spacing_text = new_string("spacing:");
    newTile->margin_text = new_string("margin:");

    init_input(&newTile->path_input,NULL,TYPE_TEXT);
    init_input(&newTile->name_input,NULL,TYPE_TEXT);
    init_input(&newTile->tile_width_input,"50",TYPE_NUMBER);
    init_input(&newTile->tile_height_input,"50",TYPE_NUMBER);
    init_input(&newTile->spacing_input,"0",TYPE_NUMBER);
    init_input(&newTile->margin_input,"0",TYPE_NUMBER);

    newTile->is_selected = false;
    newTile->type_selection_selected = false;
    newTile->current_type = TYPE_AN_IMAGE;

    newTile->multiple_images_selected = false;
    newTile->multiple_images_text = new_string("set based on multiple images");

    newTile->an_image_selected = false;
    newTile->an_image_text = new_string("set based on an image");

    newTile->search.selected = false;
    newTile->search.text = new_string("search");

    newTile->cancel.selected = false;
    newTile->cancel.text = new_string("cancel");

    newTile->save.selected = false;
    newTile->save.text = new_string("save");

    memset(newTile->buffer.text,'\0',MAX_STRING);
    newTile->buffer.time = 0;
}

void new_tile_config_rect(Window_New_Tile_Set *newTile,Settings *setting,int screen_w,int screen_h){

    //rect
    newTile->rect.w = screen_w*NEW_TILE_WIDTH_PERCENT;
    newTile->rect.h = screen_h*NEW_TILE_HEIGHT_PERCENT;
    newTile->rect.x = screen_w*0.5f - newTile->rect.w*0.5f;
    newTile->rect.y = screen_h*0.5f - newTile->rect.h*0.5f;

    //tile set
    get_size_text(setting->font,newTile->tile_set_text,&newTile->tile_set_rect.w,NULL);
    newTile->tile_set_rect.x = newTile->rect.x+SPACING;
    newTile->tile_set_rect.y = newTile->rect.y+SPACING;
    newTile->tile_set_rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;

    //name
    get_size_text(setting->font,newTile->name_text,&newTile->name_rect.w,NULL);
    newTile->name_rect.x = newTile->rect.x+SPACING*2;
    newTile->name_rect.y = newTile->tile_set_rect.y+newTile->tile_set_rect.h+SPACING;
    newTile->name_rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;

    //name input
    newTile->name_input.rect.x = newTile->name_rect.x+newTile->name_rect.w+SPACING;
    newTile->name_input.rect.y = newTile->name_rect.y;
    newTile->name_input.rect.w = (newTile->rect.x+newTile->rect.w)-newTile->name_input.rect.x-SPACING;
    newTile->name_input.rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;

    //type
    get_size_text(setting->font,newTile->type_text,&newTile->type_rect.w,NULL);
    newTile->type_rect.x = newTile->rect.x+SPACING*2;
    newTile->type_rect.y = newTile->name_rect.y+newTile->name_rect.h+SPACING;
    newTile->type_rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;

    //type selection
    newTile->type_selection.x = newTile->name_input.rect.x;
    newTile->type_selection.y = newTile->type_rect.y;
    newTile->type_selection.w = newTile->name_input.rect.w;
    newTile->type_selection.h = newTile->type_rect.h;

    //vertex rect
    newTile->vertex_rect.w = newTile->type_selection.w*0.08;
    newTile->vertex_rect.h = newTile->type_selection.h-6;
    newTile->vertex_rect.x = (newTile->type_selection.x+newTile->type_selection.w)-newTile->vertex_rect.w-3;
    newTile->vertex_rect.y = newTile->type_selection.y+3;

    SDL_Vertex vertex1 = {{newTile->vertex_rect.x+newTile->vertex_rect.w*0.2,newTile->vertex_rect.y+newTile->vertex_rect.h*0.2},setting->border_color,{1,1}};
    SDL_Vertex vertex2 = {{newTile->vertex_rect.x+newTile->vertex_rect.w*0.5,newTile->vertex_rect.y+newTile->vertex_rect.h*0.8},setting->border_color,{1,1}};
    SDL_Vertex vertex3 = {{newTile->vertex_rect.x+newTile->vertex_rect.w*0.8,newTile->vertex_rect.y+newTile->vertex_rect.h*0.2},setting->border_color,{1,1}};
    
    newTile->vertex[0] = vertex1;
    newTile->vertex[1] = vertex2;
    newTile->vertex[2] = vertex3;

    newTile->an_image_rect = newTile->type_selection;

    newTile->multiple_images_rect.x = newTile->an_image_rect.x;
    newTile->multiple_images_rect.y = newTile->an_image_rect.y+newTile->an_image_rect.h-1;
    newTile->multiple_images_rect.w = newTile->an_image_rect.w;
    newTile->multiple_images_rect.h = newTile->an_image_rect.h;

    //image
    get_size_text(setting->font,newTile->image_text,&newTile->image_rect.w,NULL);
    newTile->image_rect.x = newTile->rect.x+SPACING;
    newTile->image_rect.y = newTile->type_rect.y+newTile->type_rect.h+SPACING;
    newTile->image_rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;

    //path
    get_size_text(setting->font,newTile->path_text,&newTile->path_rect.w,NULL);
    newTile->path_rect.x = newTile->rect.x+SPACING*2;
    newTile->path_rect.y = newTile->image_rect.y+newTile->image_rect.h+SPACING;
    newTile->path_rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;

    //search
    newTile->search.rect.w = newTile->rect.w*NEW_TILE_WIDTH_BUTTON;
    newTile->search.rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;
    newTile->search.rect.x = (newTile->rect.x+newTile->rect.w)-newTile->search.rect.w-SPACING;
    newTile->search.rect.y = newTile->path_rect.y;

    //path input
    newTile->path_input.rect.x = newTile->path_rect.x+newTile->path_rect.w+SPACING;
    newTile->path_input.rect.y = newTile->path_rect.y;
    newTile->path_input.rect.w = newTile->search.rect.x-newTile->path_input.rect.x-SPACING;
    newTile->path_input.rect.h = newTile->path_rect.h;

    //tile width
    get_size_text(setting->font,newTile->tile_width_text,&newTile->tile_width_rect.w,NULL);
    newTile->tile_width_rect.x = newTile->rect.x+SPACING*2;
    newTile->tile_width_rect.y = newTile->path_rect.y+newTile->path_rect.h+SPACING;
    newTile->tile_width_rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;

    //tile height
    get_size_text(setting->font,newTile->tile_height_text,&newTile->tile_height_rect.w,NULL);
    newTile->tile_height_rect.x = newTile->rect.x+SPACING*2;
    newTile->tile_height_rect.y = newTile->tile_width_rect.y+newTile->tile_width_rect.h+SPACING;
    newTile->tile_height_rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;

    //tile height input
    newTile->tile_height_input.rect.x = newTile->tile_height_rect.x+newTile->tile_height_rect.w+SPACING;
    newTile->tile_height_input.rect.y = newTile->tile_height_rect.y;
    newTile->tile_height_input.rect.w = newTile->rect.w*NEW_TILE_WIDTH_INPUT_NUMBER;
    newTile->tile_height_input.rect.h = newTile->tile_height_rect.h;

    //tile width input
    newTile->tile_width_input.rect.x = newTile->tile_height_input.rect.x;
    newTile->tile_width_input.rect.y = newTile->tile_width_rect.y;
    newTile->tile_width_input.rect.w = newTile->rect.w*NEW_TILE_WIDTH_INPUT_NUMBER;
    newTile->tile_width_input.rect.h = newTile->tile_width_rect.h;

    //spacing
    get_size_text(setting->font,newTile->spacing_text,&newTile->spacing_rect.w,NULL);
    newTile->spacing_rect.x = newTile->tile_width_input.rect.x+newTile->tile_width_input.rect.w+SPACING;
    newTile->spacing_rect.y = newTile->tile_width_rect.y;
    newTile->spacing_rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;

    //margin
    get_size_text(setting->font,newTile->margin_text,&newTile->margin_rect.w,NULL);
    newTile->margin_rect.x = newTile->spacing_rect.x;
    newTile->margin_rect.y = newTile->tile_height_rect.y;
    newTile->margin_rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;

    //spacing input
    newTile->spacing_input.rect.x = newTile->spacing_rect.x+newTile->spacing_rect.w+SPACING;
    newTile->spacing_input.rect.y = newTile->spacing_rect.y;
    newTile->spacing_input.rect.w = newTile->rect.w*NEW_TILE_WIDTH_INPUT_NUMBER; 
    newTile->spacing_input.rect.h = newTile->spacing_rect.h;

    //margin input
    newTile->margin_input.rect.x = newTile->spacing_input.rect.x;
    newTile->margin_input.rect.y = newTile->margin_rect.y;
    newTile->margin_input.rect.w = newTile->rect.w*NEW_TILE_WIDTH_INPUT_NUMBER;
    newTile->margin_input.rect.h = newTile->margin_rect.h;

    //cancel
    newTile->cancel.rect.w = newTile->rect.w*NEW_TILE_WIDTH_BUTTON;
    newTile->cancel.rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;
    newTile->cancel.rect.x = (newTile->rect.x+newTile->rect.w)-newTile->cancel.rect.w-SPACING;
    newTile->cancel.rect.y = (newTile->rect.y+newTile->rect.h)-newTile->cancel.rect.h-SPACING;

    //save
    newTile->save.rect.w = newTile->rect.w*NEW_TILE_WIDTH_BUTTON;
    newTile->save.rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;
    newTile->save.rect.x = newTile->cancel.rect.x-newTile->save.rect.w-SPACING;
    newTile->save.rect.y = newTile->cancel.rect.y;

    //buffer
    newTile->buffer.rect.x = newTile->rect.x+SPACING;
    newTile->buffer.rect.y = newTile->save.rect.y;
    newTile->buffer.rect.w = newTile->save.rect.x-newTile->buffer.rect.x-SPACING;
    newTile->buffer.rect.h = newTile->rect.h*NEW_TILE_HEIGHT_ELEMENT_PERCENT;
}

void new_tile_draw(Window_New_Tile_Set *newTile,Settings *setting,SDL_Renderer *renderer){
    SDL_Color background_color = setting->background_color;
    SDL_Color background_color_selected = setting->background_color_selected;
    
    SDL_Color border_color = setting->border_color;
    SDL_Color text_color = setting->text_color;

    SDL_Color color_off = {255,255,255,50};

    TTF_Font *font = setting->font;

    SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
    SDL_RenderFillRect(renderer,&newTile->rect);

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&newTile->rect);

    //tile set
    draw_text(renderer,font,newTile->tile_set_text,text_color,newTile->tile_set_rect,true,true);

    //name
    draw_text(renderer,font,newTile->name_text,text_color,newTile->name_rect,true,true);
    draw_input(renderer,&newTile->name_input,setting,text_color,border_color);

    //type
    draw_text(renderer,font,newTile->type_text,text_color,newTile->type_rect,true,true);
    if(newTile->is_selected){
        SDL_SetRenderDrawColor(renderer,background_color_selected.r,background_color_selected.g,background_color_selected.b,background_color_selected.a);
        if(newTile->an_image_selected){
            SDL_RenderFillRect(renderer,&newTile->an_image_rect);
            newTile->an_image_selected = false;
        }
        else if(newTile->multiple_images_selected){
            SDL_RenderFillRect(renderer,&newTile->multiple_images_rect);
            newTile->multiple_images_selected = false;
        }

        SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
        SDL_RenderDrawRect(renderer,&newTile->an_image_rect);
        SDL_RenderDrawRect(renderer,&newTile->multiple_images_rect);

        draw_text(renderer,font,newTile->an_image_text,text_color,newTile->an_image_rect,false,true);
        draw_text(renderer,font,newTile->multiple_images_text,text_color,newTile->multiple_images_rect,false,true);
    }
    else{
        if(newTile->type_selection_selected){
            SDL_SetRenderDrawColor(renderer,background_color_selected.r,background_color_selected.g,background_color_selected.b,background_color_selected.a);
            SDL_RenderFillRect(renderer,&newTile->type_selection);
            newTile->type_selection_selected = false;
        }

        SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
        SDL_RenderGeometry(renderer,NULL,newTile->vertex,3,NULL,0);
        SDL_RenderDrawRect(renderer,&newTile->type_selection);

        switch(newTile->current_type){
            case TYPE_AN_IMAGE:
                draw_text(renderer,font,newTile->an_image_text,text_color,newTile->type_selection,false,true);
                break;
            case TYPE_MULTIPLE_IMAGES:
                draw_text(renderer,font,newTile->multiple_images_text,text_color,newTile->type_selection,false,true);
                break;
        }
    }

    //buffer
    draw_text(renderer,font,newTile->buffer.text,text_color,newTile->buffer.rect,false,true);

    draw_button(renderer,&newTile->cancel,setting,font,text_color,border_color);
    draw_button(renderer,&newTile->save,setting,font,text_color,border_color);

    if(newTile->current_type == TYPE_MULTIPLE_IMAGES){
        text_color = color_off;
        border_color = color_off;
    }

    //image
    draw_text(renderer,font,newTile->image_text,text_color,newTile->image_rect,true,true);

    //path
    draw_text(renderer,font,newTile->path_text,text_color,newTile->path_rect,true,true);
    draw_input(renderer,&newTile->path_input,setting,text_color,border_color);
    draw_button(renderer,&newTile->search,setting,font,text_color,border_color);

    //tile width
    draw_text(renderer,font,newTile->tile_width_text,text_color,newTile->tile_width_rect,true,true);
    draw_input(renderer,&newTile->tile_width_input,setting,text_color,border_color);

    //tile height
    draw_text(renderer,font,newTile->tile_height_text,text_color,newTile->tile_height_rect,true,true);
    draw_input(renderer,&newTile->tile_height_input,setting,text_color,border_color);

    //spacing
    draw_text(renderer,font,newTile->spacing_text,text_color,newTile->spacing_rect,true,true);
    draw_input(renderer,&newTile->spacing_input,setting,text_color,border_color);

    //margin
    draw_text(renderer,font,newTile->margin_text,text_color,newTile->margin_rect,true,true);
    draw_input(renderer,&newTile->margin_input,setting,text_color,border_color);
}

void new_tile_input(Data *data){

    if(!data->window_file){
        Window_New_Tile_Set *window_new_tile_set = data->window_new_tile_set;
            
        new_tile_manager_input(&window_new_tile_set->name_input,data->event,data->mouse);

        if(window_new_tile_set->current_type == TYPE_AN_IMAGE){
            new_tile_manager_input(&window_new_tile_set->path_input,data->event,data->mouse);
            new_tile_manager_input(&window_new_tile_set->tile_width_input,data->event,data->mouse);
            new_tile_manager_input(&window_new_tile_set->tile_height_input,data->event,data->mouse);
            new_tile_manager_input(&window_new_tile_set->spacing_input,data->event,data->mouse);
            new_tile_manager_input(&window_new_tile_set->margin_input,data->event,data->mouse);
        }
            
        if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT){
            if(collidePoint(data->mouse,&window_new_tile_set->cancel.rect)){
                window_new_tile_set->open = false;
            }
            else if(collidePoint(data->mouse,&window_new_tile_set->save.rect) && window_new_tile_set->name_input.buffer){
                
                if(window_new_tile_set->current_type == TYPE_AN_IMAGE){

                    if(!window_new_tile_set->path_input.buffer) return;

                    SDL_Surface *surface = IMG_Load(window_new_tile_set->path_input.buffer);
                    
                    if(surface){
                        SDL_FreeSurface(surface);
                    }
                    else{
                        strncpy(window_new_tile_set->buffer.text,"failed to load image",MAX_STRING-1);
                        window_new_tile_set->buffer.time = SDL_GetTicks();
                        return;
                    }
                }

                data->window_file = calloc(1,sizeof(Window_File));
                file_init(data->window_file,MOD_SAVE_TILE_SET);
                file_config_rect(data->window_file,data->setting,data->screen_w,data->screen_h);

                data->window_file->inputFile.text = new_string(window_new_tile_set->name_input.buffer);
                data->window_file->inputFile.buffer = new_string(window_new_tile_set->name_input.buffer);
            }
            else if(collidePoint(data->mouse,&window_new_tile_set->search.rect) && window_new_tile_set->current_type == TYPE_AN_IMAGE){

                data->window_file = calloc(1,sizeof(Window_File));
                file_init(data->window_file,MOD_SELECTION_IMG_TILE_SET);
                file_config_rect(data->window_file,data->setting,data->screen_w,data->screen_h);
                
            }
            else if(collidePoint(data->mouse,&window_new_tile_set->type_selection) && !window_new_tile_set->is_selected){
                window_new_tile_set->is_selected = true;
            }
            else if(collidePoint(data->mouse,&window_new_tile_set->an_image_rect) && window_new_tile_set->is_selected){
                window_new_tile_set->current_type = TYPE_AN_IMAGE;
                window_new_tile_set->is_selected = false;
            }
            else if(collidePoint(data->mouse,&window_new_tile_set->multiple_images_rect) && window_new_tile_set->is_selected){
                window_new_tile_set->current_type = TYPE_MULTIPLE_IMAGES;
                window_new_tile_set->is_selected = false;
                
                if(window_new_tile_set->path_input.buffer){
                    free(window_new_tile_set->path_input.buffer);
                    window_new_tile_set->path_input.buffer = NULL;
                } 
            }
        }
        
        if(collidePoint(data->mouse,&window_new_tile_set->cancel.rect)){
            window_new_tile_set->cancel.selected = true;
        }
        else if(collidePoint(data->mouse,&window_new_tile_set->save.rect)){
            window_new_tile_set->save.selected = true;
        }
        else if(collidePoint(data->mouse,&window_new_tile_set->search.rect) && window_new_tile_set->current_type == TYPE_AN_IMAGE){
            window_new_tile_set->search.selected = true;
        }
        else if(collidePoint(data->mouse,&window_new_tile_set->type_selection) && !window_new_tile_set->is_selected){
            window_new_tile_set->type_selection_selected = true;
        }
        else if(collidePoint(data->mouse,&window_new_tile_set->an_image_rect) && window_new_tile_set->is_selected){
            window_new_tile_set->an_image_selected = true;
        }
        else if(collidePoint(data->mouse,&window_new_tile_set->multiple_images_rect) && window_new_tile_set->is_selected){
            window_new_tile_set->multiple_images_selected = true;
        }
    }
}

void new_tile_run(Data *data){
    new_tile_input(data);
    new_tile_draw(data->window_new_tile_set,data->setting,data->renderer);

    if(lenght(data->window_new_tile_set->buffer.text)){
        int current_time = SDL_GetTicks();
        if(current_time - data->window_new_tile_set->buffer.time >= COLLDOWN_BUFFER){
            memset(data->window_new_tile_set->buffer.text,'\0',MAX_STRING);
        }
    }

    if(!data->window_new_tile_set->open){
        new_tile_free(&data->window_new_tile_set);
    }
}

void new_tile_free(Window_New_Tile_Set **newTile){
    if(*newTile != NULL){

        free((*newTile)->tile_set_text);
        free((*newTile)->name_text);
        free((*newTile)->type_text);
        free((*newTile)->image_text);
        free((*newTile)->path_text);
        free((*newTile)->search.text);
        free((*newTile)->tile_width_text);
        free((*newTile)->tile_height_text);
        free((*newTile)->spacing_text);
        free((*newTile)->margin_text);
        free((*newTile)->cancel.text);
        free((*newTile)->save.text);
        free((*newTile)->an_image_text);
        free((*newTile)->multiple_images_text);

        free_input(&(*newTile)->tile_width_input);
        free_input(&(*newTile)->tile_height_input);
        free_input(&(*newTile)->spacing_input);
        free_input(&(*newTile)->margin_input);
        free_input(&(*newTile)->name_input);
        free_input(&(*newTile)->path_input);

        free(*newTile);
        *newTile = NULL;
    }
}