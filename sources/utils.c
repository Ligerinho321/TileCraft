#include <utils.h>

void init_settings(Data *data){

    data->setting = calloc(1,sizeof(Settings));

    char *base_path = SDL_GetBasePath();

    char buffer[MAX_STRING];
    memset(buffer,'\0',MAX_STRING);
    snprintf(buffer,MAX_STRING-1,"%sassets\\font\\retro_gaming.ttf",base_path);
    printf("%s",buffer);

    data->setting->font = TTF_OpenFont(buffer,FONT_SIZE);
    data->setting->fontlower = TTF_OpenFont(buffer,FONT_LOWER_SIZE);

    load_settings(data->setting);

    SDL_free(base_path);
}

void free_settings(Data *data){
    TTF_CloseFont(data->setting->font);
    TTF_CloseFont(data->setting->fontlower);
    free(data->setting);
}


void init_input(Input *input,char *value,Tipo type){

    input->buffer = NULL;
    input->text = NULL;

    if(value != NULL && (type == TYPE_TEXT || lenght(value) <= MAX_DIGIT)){
        input->text = new_string(value);
        input->buffer = new_string(value);
    }

    input->type = type;

    if(type == TYPE_NUMBER) input->max_character = MAX_DIGIT;
    else if(type == TYPE_TEXT) input->max_character = MAX_STRING;

    input->on = false;
    input->selected = false;
    input->visible = false;
    input->time = 0;
}

void free_input(Input *input){
    if(input->buffer) free(input->buffer);
    if(input->text) free(input->text);
}

SDL_Rect create_rect(int x,int y,int w,int h){
    SDL_Rect rect = {x,y,w,h};
    return rect;
}

void set_size_rect(SDL_Rect *rect,TTF_Font *font,char *text,int spacing_x,int spacing_y){
    SDL_Color color = {0,0,0,255};
    SDL_Surface *font_surface = TTF_RenderText_Solid(font,text,color);

    rect->w = font_surface->w + spacing_x * 2;
    rect->h = font_surface->h + spacing_y * 2;

    SDL_FreeSurface(font_surface);
}

void get_size_text(TTF_Font *font,char *text,int *width,int *height){
    SDL_Color color = {0,0,0,255};
    SDL_Surface *font_surface = TTF_RenderText_Solid(font,text,color);

    if(width != NULL) *width = font_surface->w;
    if(height != NULL) *height = font_surface->h;

    SDL_FreeSurface(font_surface);
}

void calcSize(ScrollBar *scroll_bar,int type){
    if(type == TYPE_VERTICAL){
        scroll_bar->subRect.h = scroll_bar->rect.h / (scroll_bar->lenght / (float)scroll_bar->rect.h);
    }
    else if(type == TYPE_HORIZONTAL){
        scroll_bar->subRect.w = scroll_bar->rect.w / (scroll_bar->lenght / (float)scroll_bar->rect.w);
    }
}


Bool check_tile_by_path(Tile *tiles,char *path_tile){
    while(tiles){
        if(!strcmp(tiles->path,path_tile)) return true;
        tiles = tiles->next;
    }
    return false;
}

void append_tag_tile(Window_Tiles *window_tiles,SDL_Renderer *renderer,char *path,Tipo type_tag_tile){

    Tag_Tiles *aux = window_tiles->tag_tiles;

    Tag_Tiles *tag_tile = calloc(1,sizeof(Window_Tiles));
    tag_tile->tiles = NULL;
    tag_tile->tile_selected = NULL;
    tag_tile->next = NULL;
    tag_tile->type = type_tag_tile;
    tag_tile->scale = 1.0f;
    tag_tile->size = 0;

    char buffer[MAX_STRING];
    memset(buffer,'\0',MAX_STRING);
    strncpy(buffer,path,MAX_STRING-1);

    tag_tile->path = new_string(buffer);

    char *name = strtok(basename(buffer),".");
    tag_tile->name = new_string(name);

    if(aux == NULL){
        window_tiles->tag_tiles = tag_tile;
        window_tiles->tag_selected = tag_tile;
    }
    else{
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = tag_tile;
    }

    FILE *file = fopen(tag_tile->path,"r");
    fgets(buffer,MAX_STRING-1,file);
    trim(buffer);
    fclose(file);

    if(strstr(buffer,"tile_set_based_on_multiple_images")){
        load_tile_set_based_on_multiple_images(&tag_tile->tiles,tag_tile->path,renderer);
        tag_tile->number_of_columns = 0;
    }
    else if(strstr(buffer,"tile_set_based_on_an_image")){
        tag_tile->tiles = load_tile_set_based_on_an_image(path,renderer,&tag_tile->number_of_columns);
    }

    sort_tag_tiles(window_tiles->tag_tiles);
}

void free_tiles(Tile **tiles){
    Tile *tile = *tiles;
    while(tile != NULL){
        Tile *aux = tile->next;
        SDL_DestroyTexture(tile->texture);
        free(tile->path);
        free(tile);
        tile = aux;
    }
    *tiles = NULL;
}

void update_tiles(Tile *tiles,ScrollBar *vertScrollBar,ScrollBar *horiScrollBar,float scale,size_t number_of_columns,int max_width){

    Tile *tile = tiles;
    Tile *ultimo = NULL;

    int colunas = 0;
    int height = 0;
    int x = 1;
    int y = 1;

    vertScrollBar->lenght = 0;
    horiScrollBar->lenght = 0;

    while(tile != NULL){
        tile->rect.w = tile->largura*scale;
        tile->rect.h = tile->altura*scale;
        tile->rect.x = x;
        tile->rect.y = y;
                
        x+=tile->rect.w+1;
        colunas++;
        height = (tile->rect.h > height) ? tile->rect.h : height;
                
        horiScrollBar->lenght = (x > horiScrollBar->lenght) ? x : horiScrollBar->lenght;
        
        if((number_of_columns && number_of_columns == colunas) || (!number_of_columns && x+tile->rect.w > max_width)){
            colunas = 0;
            x = 1;
            y+=height+1;
            height = 0;
        }

        vertScrollBar->lenght = (tile->rect.y+tile->rect.h > vertScrollBar->lenght) ? tile->rect.y+tile->rect.h : vertScrollBar->lenght;
        
        ultimo = tile;
        tile = tile->next;
    }
}

Bool remove_tile(Tile **tiles,char *path){
    Tile *tile = *tiles;
    Tile *anterior = NULL;

    while((strcmp(tile->path,path) && tile != NULL)){
        anterior = tile;
        tile = tile->next;
    }

    if(tile != NULL){
        if(anterior == NULL){
            *tiles = tile->next;
        }
        else{
            anterior->next = tile->next;
        }
    }
    else{
        return false;
    }

    SDL_DestroyTexture(tile->texture);
    free(tile->path);
    free(tile);
    return true;
}

void append_tile(Tile **tiles,char *path,SDL_Renderer *renderer){

    Tile *tile = *tiles;
    Tile *novo = calloc(1,sizeof(Tile));

    novo->path = new_string(path);

    SDL_Surface *surface = IMG_Load(path);
    novo->texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    novo->selected = false;
    novo->largura = surface->w;
    novo->altura = surface->h;

    if(tile == NULL){
        *tiles = novo;
    }else{
        while(tile->next != NULL){
            tile = tile->next;
        }
        tile->next = novo;
        tile = tile->next;
        tile->next = NULL;
    }
}

Tile* get_tile_in_tag_tile(Tile *tiles,int id,char *path){
    while(tiles != NULL){
        if(tiles->id_local == id && (!path || !strcmp(tiles->path,path))){
            return tiles;
        }
        tiles = tiles->next;
    }
    return NULL;
}

Tile* get_tile_in_tag_tiles(Tag_Tiles *tag_tiles,int id,char *path){
    while(tag_tiles != NULL){
        Tile *tile = tag_tiles->tiles;
        while(tile != NULL){
            if(tile->id_local == id && (!path || !strcmp(path,tile->path))) return tile;
            tile = tile->next;
        }
        tag_tiles = tag_tiles->next;
    }
    return NULL;
}

Tag_Tiles* get_tag_tile(Tag_Tiles *tag_tiles,Tile *tile){
    while(tag_tiles){
        Tile *tiles = tag_tiles->tiles;
        while(tiles){
            if(tiles == tile){
                return tag_tiles;
            }
            tiles = tiles->next;
        }
        tag_tiles = tag_tiles->next;
    }

    return NULL;
}

void sort_tag_tiles(Tag_Tiles *tag_tiles){

    int first_id = 0;

    while(tag_tiles != NULL){
        sort_tiles(tag_tiles->tiles,&tag_tiles->size,((tag_tiles->type ==TYPE_LOCAL) ? &first_id : 0));
        tag_tiles = tag_tiles->next;
    }
}

void sort_tiles(Tile *tiles,size_t *size,int *first_id){

    *size = 0;
    
    while(tiles != NULL){
        tiles->id_global = *size;
        tiles->id_local = (first_id) ? (*first_id)++ : tiles->id_global;
        *size += 1;
        tiles = tiles->next;
    }
}



void update_input(Input *input,SDL_Event *event,Mouse *mouse,SDL_Rect *trans_rect){
    if(collidePointOffset(mouse,&input->rect,0,0,trans_rect) && !input->on){
        input->selected = true;
    }
    else{
        input->selected = false;
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

void ScrollBarUpdate(ScrollBar *scroll_bar,int type,Mouse *mouse){

    if(type == TYPE_VERTICAL && scroll_bar->visible){

        if(scroll_bar->on_bar){
            scroll_bar->subRect.y = mouse->pos_y - scroll_bar->mouseDiff;
        }
        if(scroll_bar->subRect.y < scroll_bar->rect.y+2){
            scroll_bar->subRect.y = scroll_bar->rect.y+2;
        }
        else if(scroll_bar->subRect.y+scroll_bar->subRect.h > scroll_bar->rect.y+scroll_bar->rect.h-2){
            scroll_bar->subRect.y = (scroll_bar->rect.y+scroll_bar->rect.h)-scroll_bar->subRect.h-2;
        }
        
        int lenght_diff = scroll_bar->lenght - scroll_bar->rect.h;
        int scroll_lenght = scroll_bar->rect.h - scroll_bar->subRect.h-5;
        scroll_bar->offset = lenght_diff / (scroll_lenght * 1.0) * (scroll_bar->subRect.y-scroll_bar->rect.y-2) * -1.0;

        if(scroll_bar->offset > 0){
            scroll_bar->offset = 0;
        }
        else if(scroll_bar->offset+scroll_bar->lenght < scroll_bar->rect.h){
            scroll_bar->offset = scroll_bar->rect.h-scroll_bar->lenght;
        }

    }
    else if(type == TYPE_HORIZONTAL && scroll_bar->visible){

        if(scroll_bar->on_bar){
            scroll_bar->subRect.x = mouse->pos_x - scroll_bar->mouseDiff;
        }
        if(scroll_bar->subRect.x < scroll_bar->rect.x+2){
            scroll_bar->subRect.x = scroll_bar->rect.x+2;
        }
        else if(scroll_bar->subRect.x+scroll_bar->subRect.w > scroll_bar->rect.x+scroll_bar->rect.w-2){
            scroll_bar->subRect.x = (scroll_bar->rect.x+scroll_bar->rect.w)-scroll_bar->subRect.w-2;
        }

        int lenght_diff = scroll_bar->lenght - scroll_bar->rect.w;
        int scroll_lenght = scroll_bar->rect.w - scroll_bar->subRect.w-5;
        scroll_bar->offset = lenght_diff / (scroll_lenght * 1.0) * (scroll_bar->subRect.x-scroll_bar->rect.x-2) * -1.0;

        if(scroll_bar->offset > 0){
            scroll_bar->offset = 0;
        }
        else if(scroll_bar->offset+scroll_bar->lenght < scroll_bar->rect.w){
            scroll_bar->offset = scroll_bar->rect.w-scroll_bar->lenght;
        }
    
    }
}


Bool collidePointMap(Mouse *mouse,Properties *properties){
    get_pos_in_map(mouse,properties);

    if(properties->pos_x >= 0 && properties->pos_x < properties->width_map &&
       properties->pos_y >= 0 && properties->pos_y < properties->height_map){
        return true;
    }
    else{
        return false;
    }
}

int collidePointOffset(Mouse *mouse,SDL_Rect *rect,int offsetX,int offsetY,SDL_Rect *transRect){
    if((mouse->pos_x > rect->x+transRect->x+offsetX && mouse->pos_x < rect->x+rect->w+transRect->x+offsetX) &&
       (mouse->pos_y > rect->y+transRect->y+offsetY && mouse->pos_y < rect->y+rect->h+transRect->y+offsetY)){
        return 1;
    }
    else{
        return 0;
    }
}

int collidePoint(Mouse *mouse,SDL_Rect *rect){
    if((mouse->pos_x > rect->x && mouse->pos_x < rect->x+rect->w) && 
       (mouse->pos_y > rect->y && mouse->pos_y < rect->y+rect->h)){
        return 1;
    }else{
        return 0;
    }
}

void createMouse(Mouse *mouse,SDL_Renderer *renderer){
    mouse->pos_x = 0;
    mouse->pos_y = 0;
    mouse->rel_x = 0;
    mouse->rel_y = 0;
    mouse->pos_x_after = 0;
    mouse->pos_y_after = 0;
    mouse->pos_x_before = 0;
    mouse->pos_y_before = 0;
    mouse->press_left = false;
    mouse->press_right = false;
}

void convert_to_screen(int x,int y,SDL_Point *point,Properties *properties,SDL_Rect rect){
    x *= properties->width_block;
    y *= properties->height_block;

    if(properties->type == TYPE_ISOMETRIC){
        point->x = x - y;
        point->y = (y + x)*0.5f;
    }
    else if(properties->type == TYPE_ORTHOGONAL){
        point->x = x;
        point->y = y;
    }             
                        
    point->x = ((point->x-properties->transX)*properties->scale)-rect.x;
    point->y = ((point->y-properties->transY)*properties->scale)-rect.y;
               
}

void get_pos_in_map(Mouse *mouse,Properties *properties){
    float iso_x,iso_y;
    
    screenToWorld(mouse->pos_x,mouse->pos_y,&iso_x,&iso_y,properties->transX,properties->transY,properties->scale);
    
    if(properties->type == TYPE_ISOMETRIC) convertCartisian(iso_x,iso_y,&iso_x,&iso_y);

    properties->pos_x = (int)(floor(iso_x / properties->width_block));
    properties->pos_y = (int)(floor(iso_y / properties->height_block));
}

void world_to_screen(int *x,int *y,float transX,float transY,float scale,SDL_Rect *rect){
    *x = ((*x-transX)*scale)-rect->x;
    *y = ((*y-transY)*scale)-rect->y;
}

void screenToWorld(int screenX,int screenY,float *worldX,float *worldY,float transX,float transY,float scale){
    *worldX = ((float)(screenX/scale)) + transX;
    *worldY = ((float)(screenY/scale)) + transY;
}

void convertIsometric(int x,int y,int *destX,int *destY){
    *destX = x-y;
    *destY = (y+x)*0.5;
}

void convertCartisian(float x, float y, float *destX, float *destY){
    *destX = (x*0.5) + y;
    *destY = y - (x*0.5);
}
