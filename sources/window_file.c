#include <window_file.h>

void file_manager_input(Input *input,SDL_Event *event,Mouse *mouse){
    if(collidePoint(mouse,&input->rect) && !input->on){
        input->selected = true;
    }
    else{
        input->selected = false;
    }

    if(input->on){
        if(event->type == SDL_TEXTINPUT){
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

void cd(Window_File *window_file,char *diretorio){
    char path[MAX_STRING];
    memset(path,'\0',MAX_STRING-1);
    snprintf(path,MAX_STRING-1,"%s\\%s",window_file->current_path,diretorio);

    if(!_chdir(path)){
        directUpdate(window_file);
    }
    else{
        strncpy(window_file->buffer.text,"failed to access directory",MAX_STRING-1);
        window_file->buffer.time = SDL_GetTicks();
    }
}

void selection_img_tile_set(Data *data,char *img){
    char buffer[MAX_STRING];
    memset(buffer,'\0',MAX_STRING);
    snprintf(buffer,MAX_STRING-1,"%s\\%s",data->window_file->current_path,img);

    if(data->window_new_tile_set->path_input.buffer) free(data->window_new_tile_set->path_input.buffer);
    if(data->window_new_tile_set->name_input.buffer) free(data->window_new_tile_set->name_input.buffer);

    data->window_new_tile_set->path_input.buffer = new_string(buffer);
    data->window_new_tile_set->name_input.buffer = new_string(strtok(basename(buffer),"."));

    data->window_file->open = false;
}

void selection_img_tile(Data *data,char *img){
    char path_tile[MAX_STRING];
    memset(path_tile,'\0',MAX_STRING);
    snprintf(path_tile,MAX_STRING-1,"%s\\%s",data->window_file->current_path,img);

    if(check_tile_by_path(data->window_file->window_edit->tiles,path_tile)){
        strncpy(data->window_file->buffer.text,"This tile already exists",MAX_STRING-1);
        data->window_file->buffer.time = SDL_GetTicks();
    }
    else{
        append_tile(&data->window_file->window_edit->tiles,path_tile,data->renderer);
        sort_tiles(data->window_file->window_edit->tiles,&data->window_file->window_edit->size,0);
        
        data->window_file->window_edit->undo = edit_push_stack(data->window_file->window_edit->undo,path_tile,TYPE_ERASER);
        data->window_file->window_edit->redo = edit_free_stack(data->window_file->window_edit->redo);
        data->window_file->window_edit->updated = false;
        
        reload_tag_tile(data->window_project,data->renderer,data->window_file->window_edit->path,path_tile,TYPE_APPEND);
        
        data->window_file->open = false;
    }
}

void selection_file(Data *data,char *file_name){
    char type[MAX_STRING];
    char fullPath[MAX_STRING];
    memset(fullPath,'\0',MAX_STRING);
    memset(type,'\0',MAX_STRING);

    snprintf(fullPath,MAX_STRING-1,"%s\\%s",data->window_file->current_path,file_name);

    if(data->window_project && check_tag_by_path(data->window_project,fullPath)){
        strncpy(data->window_file->buffer.text,"File is already open",MAX_STRING-1);
        data->window_file->buffer.time = SDL_GetTicks();
    }
    else{
        FILE *file = fopen(fullPath,"r");
        fgets(type,MAX_STRING-1,file);
        trim(type);
        fclose(file);

        if(strstr(type,"tile_set_based_on_an_image") || strstr(type,"tile_set_based_on_multiple_images")){
            
            data->window_file->open = false;

            if(data->window_menu) data->window_menu->open = false;
            if(data->window_new_tile_set) data->window_new_tile_set = false;

            Window_Edit *window_edit = calloc(1,sizeof(Window_Edit));
            window_edit->path = new_string(fullPath);

            edit_init(window_edit,data->renderer,((strstr(type,"tile_set_based_on_an_image")) ? TYPE_AN_IMAGE : TYPE_MULTIPLE_IMAGES));

            Tag *tag = calloc(1,sizeof(Tag));
            tag->name = new_string(file_name);
            tag->path = new_string(fullPath);
            tag->type = TYPE_TILE_SET;
            tag->element = (void*)window_edit;

            if(!data->window_project){

                data->window_project = calloc(1,sizeof(Window_Project));
                project_init(data->window_project,data->setting,data->renderer,tag);
                project_config_rect(data->window_project,data->setting,data->screen_w,data->screen_h);

            }else{
                push_tag(data->window_project,data->setting,data->renderer,tag);
            }
        }
        else if(strstr(type,"map")){

            data->window_file->open = false;
            if(data->window_menu){
                data->window_menu->open = false;
            }

            Window_Map *window_map = calloc(1,sizeof(Window_Map));
            window_map->path = new_string(fullPath);

            load_map(window_map,data->setting,data->window,data->renderer);

            Tag *tag = calloc(1,sizeof(Tag));
            tag->name = new_string(file_name);
            tag->path = new_string(fullPath);
            tag->type = TYPE_MAP;
            tag->element = (void*)window_map;

            if(!data->window_project){

                data->window_project = calloc(1,sizeof(Window_Project));
                project_init(data->window_project,data->setting,data->renderer,tag);
                project_config_rect(data->window_project,data->setting,data->screen_w,data->screen_h);
                
            }else{
                push_tag(data->window_project,data->setting,data->renderer,tag);
            }

        }else{
            strncpy(data->window_file->buffer.text,"Invalid file",MAX_STRING-1);
            data->window_file->buffer.time = SDL_GetTicks();
        }

    }
}

void directReset(Window_File *window_file){
    Diretorio *diretorio = window_file->diretorios;
    Diretorio *aux;
    while(diretorio != NULL){
        aux = diretorio;
        diretorio = diretorio->next;
        free(aux);
    }
    window_file->diretorios = NULL;
}

void directCreate(Window_File *window_file,struct dirent *file,Diretorio **aux,int *y){

    Diretorio *diretorio = calloc(1,sizeof(Diretorio));
    diretorio->next = NULL;
    diretorio->clicked = false;
    diretorio->time = 0;

    if(file->d_type == DT_DIR){
        strncpy(diretorio->tipo,"diretorio",MAX_STRING-1);
    }
    else if(file->d_type == DT_REG){
        strncpy(diretorio->tipo,"arquivo",MAX_STRING-1);
    }
                    
    strncpy(diretorio->name,file->d_name,MAX_STRING-1);
                    
    diretorio->rect.x = 0;
    diretorio->rect.y = *y+1;
    diretorio->rect.w = window_file->rect.w-SPACING*2;
    diretorio->rect.h = window_file->subRect.h*FILE_HEIGHT_DIRECTORY_ELEMENT_PERCENT;
                    
    *y+= window_file->subRect.h*FILE_HEIGHT_DIRECTORY_ELEMENT_PERCENT;
                    
    if(window_file->diretorios == NULL){
        window_file->diretorios = diretorio;
        *aux = diretorio;
    }else{
        (*aux)->next = diretorio;
        (*aux) = (*aux)->next;
    }
}

void directUpdate(Window_File *window_file){

    directReset(window_file);

    free(window_file->current_path);
    free(window_file->inputPath.buffer);
    free(window_file->inputPath.text);

    window_file->current_path = _getcwd(NULL,0);
    window_file->inputPath.buffer = new_string(window_file->current_path);
    window_file->inputPath.text = new_string(window_file->current_path);

    Diretorio *aux = NULL;
    int y = 0;

    DIR *dir;
    struct dirent *file;
    dir = opendir(window_file->current_path);

    if(dir != NULL){

        while((file = readdir(dir)) != NULL){
            Bool create = false;

            switch(window_file->modo){
                case MOD_SELECTION_IMG_TILE:
                case MOD_SELECTION_IMG_TILE_SET:
                    if(file->d_type == DT_DIR || (file->d_type == DT_REG && (strstr(file->d_name,".png") || strstr(file->d_name,".jpg")))) create = true;
                    break;
                case MOD_SELECTION_FILE:
                    if(file->d_type == DT_DIR || (file->d_type == DT_REG && strstr(file->d_name,".txt"))) create = true;
                    break;
                case MOD_SELECTION_FONT:
                    if(file->d_type == DT_DIR || (file->d_type == DT_REG && (strstr(file->d_name,".ttf") || strstr(file->d_name,".TTF")))) create = true;
                    break;
                case MOD_SAVE_MAP:
                case MOD_SAVE_TILE_SET:
                case MOD_EXPORT:
                    if(file->d_type == DT_DIR) create = true;
                    break;
            }

            if(create == true) directCreate(window_file,file,&aux,&y);
        }
    }

    closedir(dir);

    window_file->verticalScrollBar.lenght = (aux != NULL) ? aux->rect.y+aux->rect.h : 0;
    window_file->verticalScrollBar.subRect.y = window_file->verticalScrollBar.rect.y+SCROLL_BAR_SPACING;

    calcSize(&window_file->verticalScrollBar,TYPE_VERTICAL);

    if(window_file->verticalScrollBar.subRect.h >= window_file->subRect.h || window_file->verticalScrollBar.subRect.h <= 0){
        window_file->verticalScrollBar.visible = false;
        window_file->verticalScrollBar.offset = 0;
        window_file->subRect.w = window_file->rect.w-SPACING*2;
    }
    else{
        window_file->verticalScrollBar.visible = true;
        window_file->subRect.w =  window_file->verticalScrollBar.rect.x-window_file->subRect.x-SCROLL_BAR_SPACING;
    }
}


void file_init(Window_File *window_file,Modo modo){

    window_file->open = true;
    window_file->modo = modo;

    window_file->path_text = new_string("Path:");
    window_file->name_text = new_string("Name:");

    init_input(&window_file->inputPath,NULL,TYPE_TEXT);
    init_input(&window_file->inputFile,NULL,TYPE_TEXT);

    window_file->back.selected = false;
    window_file->back.text = new_string("back");

    window_file->search.selected = false;
    window_file->search.text = new_string("search");

    window_file->cancel.selected = false;
    window_file->cancel.text = new_string("cancel");

    window_file->confirmar.selected = false;
    switch(window_file->modo){
        case MOD_SAVE_TILE_SET:
        case MOD_SAVE_MAP:
            window_file->confirmar.text = new_string("save");
            break;
        case MOD_SELECTION_FILE:
        case MOD_SELECTION_IMG_TILE_SET:
            window_file->confirmar.text = new_string("open");
            break;
        case MOD_SELECTION_IMG_TILE:
        case MOD_SELECTION_FONT:
        case MOD_EXPORT:
            window_file->confirmar.text = new_string("select");
            break;
    }

    memset(window_file->buffer.text,'\0',MAX_STRING);
    window_file->buffer.time = 0;

    switch(window_file->modo){
        case MOD_SELECTION_FILE:
            window_file->titulo_text = new_string("Open as");
            break;
        case MOD_EXPORT:
            window_file->titulo_text = new_string("Select path");
            break;
        case MOD_SELECTION_IMG_TILE:
            window_file->titulo_text = new_string("Add tile");
            break;
        case MOD_SAVE_TILE_SET:
        case MOD_SAVE_MAP:
            window_file->titulo_text = new_string("Save as");
            break;
        case MOD_SELECTION_FONT:
            window_file->titulo_text = new_string("Select font");
            break;
        case MOD_SELECTION_IMG_TILE_SET:
            window_file->titulo_text = new_string("Select image");
            break;
    }

    window_file->verticalScrollBar.mouseDiff = 0;
    window_file->verticalScrollBar.selected = false;
    window_file->verticalScrollBar.on_bar = false;
    window_file->verticalScrollBar.visible = false;
}

void file_config_rect(Window_File *window_file,Settings *setting,int screen_w,int screen_h){

    window_file->rect.w = screen_w*FILE_WIDTH_PERCENT;
    window_file->rect.h = screen_h*FILE_HEIGHT_PERCENT;
    window_file->rect.x = screen_w*0.5-window_file->rect.w*0.5;
    window_file->rect.y = screen_h*0.5-window_file->rect.h*0.5;

    get_size_text(setting->font,window_file->titulo_text,&window_file->titulo_rect.w,&window_file->titulo_rect.h);
    window_file->titulo_rect.x = window_file->rect.x+SPACING;
    window_file->titulo_rect.y = window_file->rect.y+SPACING;
    window_file->titulo_rect.h = window_file->rect.h*FILE_HEIGHT_ELEMENT_PERCENT;

    get_size_text(setting->font,window_file->path_text,&window_file->path_rect.w,&window_file->path_rect.h);
    window_file->path_rect.x = window_file->rect.x+SPACING;
    window_file->path_rect.y = window_file->titulo_rect.y+window_file->titulo_rect.h+SPACING;
    window_file->path_rect.h = window_file->rect.h*FILE_HEIGHT_ELEMENT_PERCENT;

    window_file->back.rect.w = window_file->rect.w*FILE_BUTTON_WIDTH_PERCENT;
    window_file->back.rect.h = window_file->rect.h*FILE_HEIGHT_ELEMENT_PERCENT;
    window_file->back.rect.x = (window_file->rect.x+window_file->rect.w)-window_file->back.rect.w-SPACING;
    window_file->back.rect.y = window_file->path_rect.y;

    window_file->search.rect.w = window_file->rect.w*FILE_BUTTON_WIDTH_PERCENT;
    window_file->search.rect.h = window_file->rect.h*FILE_HEIGHT_ELEMENT_PERCENT;
    window_file->search.rect.x = window_file->back.rect.x-window_file->search.rect.w-SPACING;
    window_file->search.rect.y = window_file->back.rect.y;

    window_file->inputPath.rect.x = window_file->path_rect.x+window_file->path_rect.w+SPACING;
    window_file->inputPath.rect.y = window_file->path_rect.y;
    window_file->inputPath.rect.w = window_file->search.rect.x-window_file->inputPath.rect.x-SPACING;
    window_file->inputPath.rect.h = window_file->rect.h*FILE_HEIGHT_ELEMENT_PERCENT;

    window_file->cancel.rect.w = window_file->rect.w*FILE_BUTTON_WIDTH_PERCENT;
    window_file->cancel.rect.h = window_file->rect.h*FILE_HEIGHT_ELEMENT_PERCENT;
    window_file->cancel.rect.x = (window_file->rect.x+window_file->rect.w)-window_file->cancel.rect.w-SPACING;
    window_file->cancel.rect.y = (window_file->rect.y+window_file->rect.h)-window_file->cancel.rect.h-SPACING;

    window_file->confirmar.rect.w = window_file->rect.w*FILE_BUTTON_WIDTH_PERCENT;
    window_file->confirmar.rect.h = window_file->rect.h*FILE_HEIGHT_ELEMENT_PERCENT;
    window_file->confirmar.rect.x = window_file->cancel.rect.x-window_file->confirmar.rect.w-SPACING;
    window_file->confirmar.rect.y = window_file->cancel.rect.y;

    get_size_text(setting->font,window_file->name_text,&window_file->name_rect.w,&window_file->name_rect.h);
    window_file->name_rect.h = window_file->rect.h*FILE_HEIGHT_ELEMENT_PERCENT;
    window_file->name_rect.x = window_file->rect.x+SPACING;
    window_file->name_rect.y = window_file->confirmar.rect.y-window_file->name_rect.h-SPACING;

    window_file->inputFile.rect.x = window_file->name_rect.x+window_file->name_rect.w+SPACING;
    window_file->inputFile.rect.y = window_file->name_rect.y;
    window_file->inputFile.rect.w = window_file->rect.w-window_file->inputFile.rect.x-SPACING;
    window_file->inputFile.rect.h = window_file->rect.h*FILE_HEIGHT_ELEMENT_PERCENT;

    window_file->subRect.x = window_file->rect.x+SPACING;
    window_file->subRect.y = window_file->path_rect.y+window_file->path_rect.h+SPACING;
    window_file->subRect.h = window_file->name_rect.y-window_file->subRect.y-SPACING;

    window_file->verticalScrollBar.rect.w = window_file->rect.w*FILE_SIZE_SCROLLBAR_PERCENT;
    window_file->verticalScrollBar.rect.h = window_file->subRect.h;
    window_file->verticalScrollBar.rect.x = (window_file->rect.x+window_file->rect.w)-window_file->verticalScrollBar.rect.w-SPACING;
    window_file->verticalScrollBar.rect.y = window_file->subRect.y;

    window_file->verticalScrollBar.subRect.x = window_file->verticalScrollBar.rect.x+SCROLL_BAR_SPACING;
    window_file->verticalScrollBar.subRect.y = window_file->verticalScrollBar.rect.y+SCROLL_BAR_SPACING;
    window_file->verticalScrollBar.subRect.w = window_file->verticalScrollBar.rect.w-SCROLL_BAR_SPACING*2;

    window_file->buffer.rect.x = window_file->rect.x+SPACING;
    window_file->buffer.rect.y = window_file->confirmar.rect.y;
    window_file->buffer.rect.w = window_file->confirmar.rect.x-window_file->buffer.rect.x-SPACING;
    window_file->buffer.rect.h = window_file->rect.h*FILE_HEIGHT_ELEMENT_PERCENT;

    directUpdate(window_file);
}

void file_draw(Window_File *window_file,Settings *setting,SDL_Renderer *renderer){

    SDL_Color text_color = setting->text_color;
    SDL_Color border_color = setting->border_color;
    SDL_Color background_color = setting->background_color;
    SDL_Color background_color_selected = setting->background_color_selected;


    SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
    SDL_RenderFillRect(renderer,&window_file->rect);

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&window_file->rect);

    draw_text(renderer,setting->font,window_file->titulo_text,text_color,window_file->titulo_rect,true,true);

    //path
    draw_text(renderer,setting->font,window_file->path_text,text_color,window_file->path_rect,true,true);
    //input path
    draw_input(renderer,&window_file->inputPath,setting,text_color,border_color);
    //button back
    draw_button(renderer,&window_file->back,setting,setting->font,text_color,border_color);
    //draw search
    draw_button(renderer,&window_file->search,setting,setting->font,text_color,border_color);

    //draw diretorios

    SDL_RenderSetViewport(renderer,&window_file->subRect);

    Diretorio *diretorio = window_file->diretorios;

    while(diretorio != NULL){
        SDL_Rect rect = {
            diretorio->rect.x,
            diretorio->rect.y+window_file->verticalScrollBar.offset,
            diretorio->rect.w,
            diretorio->rect.h
        };

        if(diretorio->selected){
            SDL_SetRenderDrawColor(renderer,background_color_selected.r,background_color_selected.g,background_color_selected.b,background_color_selected.a);
            SDL_RenderFillRect(renderer,&rect);
            diretorio->selected = false;
        }

        draw_text(renderer,setting->font,diretorio->name,text_color,rect,false,true);

        diretorio = diretorio->next;
    }

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,NULL);

    SDL_RenderSetViewport(renderer,NULL);

    draw_scroll_bar(&window_file->verticalScrollBar,setting,renderer);

    //name
    draw_text(renderer,setting->font,window_file->name_text,text_color,window_file->name_rect,true,true);
    //input file
    draw_input(renderer,&window_file->inputFile,setting,text_color,border_color);
    //button cancel
    draw_button(renderer,&window_file->cancel,setting,setting->font,text_color,border_color);
    //button confirmar
    draw_button(renderer,&window_file->confirmar,setting,setting->font,text_color,border_color);
    //draw buffer
    draw_text(renderer,setting->font,window_file->buffer.text,text_color,window_file->buffer.rect,false,true);
}

void file_input(Data *data){

    Window_File *window_file = data->window_file;

    file_manager_input(&window_file->inputPath,data->event,data->mouse);
    file_manager_input(&window_file->inputFile,data->event,data->mouse);

    if(collidePoint(data->mouse,&window_file->rect)){
            
        if(collidePoint(data->mouse,&window_file->subRect)){
            Diretorio *current = window_file->diretorios;
            while(current != NULL){
                if(collidePointOffset(data->mouse,&current->rect,0,window_file->verticalScrollBar.offset,&window_file->subRect)){
                    current->selected = true;
                }
                current = current->next;
            }
        }
        else if(collidePoint(data->mouse,&window_file->confirmar.rect)){
            window_file->confirmar.selected = true;
        }
        else if(collidePoint(data->mouse,&window_file->cancel.rect)){
            window_file->cancel.selected = true;
        }
        else if(collidePoint(data->mouse,&window_file->back.rect)){
            window_file->back.selected = true;
        }
        else if(collidePoint(data->mouse,&window_file->search.rect)){
            window_file->search.selected = true;
        }
        else if(collidePoint(data->mouse,&window_file->verticalScrollBar.subRect) && window_file->verticalScrollBar.visible){
            window_file->verticalScrollBar.selected = true;
        }
    }

    if(data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT){

        if(collidePoint(data->mouse,&window_file->subRect)){
            Diretorio *current = window_file->diretorios;
            while(current != NULL){

                if(collidePointOffset(data->mouse,&current->rect,0,window_file->verticalScrollBar.offset,&window_file->subRect)){
                    
                    if(current->clicked && SDL_GetTicks() - current->time <= COLLDOWN_DOUBLE_CLICK){
                        current->clicked = false;
                        
                        //open directory
                        if(!strcmp(current->tipo,"diretorio")){
                            cd(window_file,current->name);
                            break;
                        }
                        else{
                            //select img tile set
                            if(window_file->modo == MOD_SELECTION_IMG_TILE_SET){
                                selection_img_tile_set(data,current->name);
                            }
                            //selection img tile
                            else if(window_file->modo == MOD_SELECTION_IMG_TILE){
                                selection_img_tile(data,current->name);
                                break;
                            }
                            //open file (map or blocos)
                            else if(window_file->modo == MOD_SELECTION_FILE){
                                selection_file(data,current->name);
                                break;
                            }
                        }
                    }
                    else{
                        current->clicked = true;
                    }
                    current->time = SDL_GetTicks();
                }

                current = current->next;
            }
        }

        if(collidePoint(data->mouse,&window_file->verticalScrollBar.subRect) && window_file->verticalScrollBar.visible){
            window_file->verticalScrollBar.on_bar = true;
            window_file->verticalScrollBar.mouseDiff = data->mouse->pos_y - window_file->verticalScrollBar.subRect.y;
        }
        else if(collidePoint(data->mouse,&window_file->back.rect)){
            _chdir("..");
            directUpdate(window_file);
        }
        else if(collidePoint(data->mouse,&window_file->cancel.rect)){
            window_file->open = false;
            if(window_file->modo == MOD_SAVE_MAP){
                cancel_exiting_closing(data->window_project);
            }
        }

        //Input Path
        if(collidePoint(data->mouse,&window_file->inputPath.rect)){
            window_file->inputPath.on = true;
            window_file->inputPath.time = SDL_GetTicks();
            window_file->inputPath.visible = true;

        }
        else if(collidePoint(data->mouse,&window_file->search.rect)){
            if(_chdir(window_file->inputPath.buffer) != -1){
                window_file->inputPath.on = false;
                directUpdate(window_file);
            }
            else{
                strncpy(window_file->buffer.text,"Invalid directory",MAX_STRING-1);
                window_file->buffer.time = SDL_GetTicks();
            }
        }
        else{
            free(window_file->inputPath.buffer);
            window_file->inputPath.buffer = new_string(window_file->inputPath.text);
            window_file->inputPath.on = false;
        }

        //Input File
        if(collidePoint(data->mouse,&window_file->inputFile.rect)){
            window_file->inputFile.on = true;
            window_file->inputFile.time = SDL_GetTicks();
            window_file->inputFile.visible = true;

        }
        else{
            window_file->inputFile.on = false;
        }
    }

    else if(data->event->type == SDL_MOUSEBUTTONUP && data->event->button.button == SDL_BUTTON_LEFT){
        window_file->verticalScrollBar.on_bar = false;

    }

    if((data->event->type == SDL_KEYDOWN && data->event->key.keysym.scancode == SDL_SCANCODE_RETURN) ||
       (data->event->type == SDL_MOUSEBUTTONDOWN && data->event->button.button == SDL_BUTTON_LEFT && collidePoint(data->mouse,&window_file->confirmar.rect))){
        
        if(window_file->inputFile.buffer){

            if(window_file->modo == MOD_SELECTION_FILE || window_file->modo == MOD_EXPORT){

                char fullPath[MAX_STRING];
                memset(fullPath,'\0',MAX_STRING);
                
                strncpy(fullPath,window_file->current_path,MAX_STRING-1);
                strncat(fullPath,"\\",MAX_STRING - strlen(fullPath) - 1);
                strncat(fullPath,window_file->inputFile.buffer,MAX_STRING - strlen(fullPath) - 1);
                
                if(_chdir(fullPath) != -1){
                    window_file->inputFile.on = false;
                    memset(window_file->inputFile.buffer,'\0',MAX_STRING);
                    directUpdate(window_file);
                }
                else{
                    strncpy(window_file->buffer.text,"Invalid directory",MAX_STRING-1);
                    window_file->buffer.time = SDL_GetTicks();
                }
            }
            else if(window_file->modo == MOD_SAVE_TILE_SET){
                
                char fileName[MAX_STRING];
                memset(fileName,'\0',MAX_STRING);

                sprintf(fileName,"%s.txt",window_file->inputFile.buffer);

                if(data->window_new_tile_set->current_type == TYPE_AN_IMAGE){
                    save_tile_set_based_on_an_image(fileName,
                                                    data->window_new_tile_set->path_input.buffer,
                                                    atoi(data->window_new_tile_set->tile_width_input.buffer),
                                                    atoi(data->window_new_tile_set->tile_height_input.buffer),
                                                    atoi(data->window_new_tile_set->spacing_input.buffer),
                                                    atoi(data->window_new_tile_set->margin_input.buffer)
                                                   );
                }
                else if(data->window_new_tile_set->current_type == TYPE_MULTIPLE_IMAGES){
                    save_tile_set_based_on_multiple_images(NULL,fileName);
                }

                selection_file(data,fileName);
            }
            else if(window_file->modo == MOD_SAVE_MAP){

                char file_name[MAX_STRING];
                char path[MAX_STRING];

                memset(path,'\0',MAX_STRING);
                memset(file_name,'\0',MAX_STRING);

                snprintf(file_name,MAX_STRING-1,"%s.txt",window_file->inputFile.buffer);
                snprintf(path,MAX_STRING-1,"%s\\%s",window_file->current_path,file_name);

                free(data->window_project->tag_atual->name);
                data->window_project->tag_atual->name = new_string(file_name);
                data->window_project->tag_atual->path = new_string(path);
                ((Window_Map*)data->window_project->tag_atual->element)->path = new_string(path);

                save_map((Window_Map*)data->window_project->tag_atual->element);

                config_tag_rect(data->window_project,data->setting,TYPE_LOCAL);

                window_file->open = false;
            }
        }
        else{
            if(window_file->modo == MOD_EXPORT){

                Window_Map *window_map = (Window_Map*)data->window_project->tag_atual->element;
                window_map->path_export = new_string(window_file->current_path);
                printf("%s\n",window_map->path_export);

                export_map(window_map);

                window_file->open = false;
            }
        }
    }
}

void file_colldown(Window_File *window_file){
    int current_time = SDL_GetTicks();
    if(strlen(window_file->buffer.text) > 0){
        if(current_time - window_file->buffer.time >= COLLDOWN_BUFFER){
            memset(window_file->buffer.text,'\0',MAX_STRING);
        }
    }
}

void file_run(Data *data){

    file_input(data);
    file_colldown(data->window_file);
    ScrollBarUpdate(&data->window_file->verticalScrollBar,TYPE_VERTICAL,data->mouse);
    file_draw(data->window_file,data->setting,data->renderer);

    if(data->window_file->open == false){
        file_free(&data->window_file);
    }
}

void file_free(Window_File **window_file){
    if(*window_file != NULL){
        directUpdate(*window_file);
        directReset(*window_file);

        free((*window_file)->inputFile.buffer);
        free((*window_file)->inputFile.text);
        free((*window_file)->inputPath.buffer);
        free((*window_file)->inputPath.text);

        free((*window_file)->current_path);
        free((*window_file)->titulo_text);
        free((*window_file)->path_text);
        free((*window_file)->name_text);
        free((*window_file)->back.text);
        free((*window_file)->cancel.text);
        free((*window_file)->confirmar.text);
        free((*window_file)->search.text);

        free(*window_file);
        *window_file = NULL;
    }
}