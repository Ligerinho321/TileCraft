#include <file_utils.h>

void set_color(SDL_Color *color,char *name,char *value){
    if     (!strcmp(name,"r")) color->r = atoi(value);
    else if(!strcmp(name,"g")) color->g = atoi(value);
    else if(!strcmp(name,"b")) color->b = atoi(value);
    else if(!strcmp(name,"a")) color->a = atoi(value);
}

void load_settings(Settings *setting){

    char *base_path = SDL_GetBasePath();

    char buffer[MAX_STRING];
    memset(buffer,'\0',MAX_STRING);
    snprintf(buffer,MAX_STRING-1,"%ssettings.txt",base_path);

    FILE *file = fopen(buffer,"r");
    if(file){

        char struct_name[MAX_STRING];
        char line[MAX_STRING];
        char prop_name[MAX_STRING];
        char prop_value[MAX_STRING];

        memset(line,'\0',MAX_STRING);
        memset(prop_name,'\0',MAX_STRING);
        memset(prop_value,'\0',MAX_STRING);
        memset(struct_name,'\0',MAX_STRING);

        while(fgets(line,MAX_STRING-1,file) != NULL){

            trim(line);
            if(strlen(line)){
                int x = find_first_of(line,'=');
                if(x != 0){
                    substr(line,prop_name,0,x);
                    trim(prop_name);

                    substr(line,prop_value,x+1,strlen(line));
                    trim(prop_value);

                    if(!strcmp(struct_name,"text_color")){
                        set_color(&setting->text_color,prop_name,prop_value);
                    }
                    else if(!strcmp(struct_name,"border_color")){
                        set_color(&setting->border_color,prop_name,prop_value);
                    }
                    else if(!strcmp(struct_name,"background_color")){
                        set_color(&setting->background_color,prop_name,prop_value);
                    }
                    else if(!strcmp(struct_name,"background_color_selected")){
                        set_color(&setting->background_color_selected,prop_name,prop_value);
                    }
                    else if(!strcmp(struct_name,"quit_color")){
                        set_color(&setting->quit_color,prop_name,prop_value);
                    }
                    else if(!strcmp(struct_name,"background_color_map")){
                        set_color(&setting->background_color_map,prop_name,prop_value);
                    }
                    else if(!strcmp(struct_name,"grade_color")){
                        set_color(&setting->grade_color,prop_name,prop_value);
                    }
                }
                else{
                    if(line[0] == '{'){
                        strncpy(struct_name,prop_name,MAX_STRING-1);
                    }
                    else if(line[0] == '}'){
                        memset(struct_name,'\0',MAX_STRING);
                        strncpy(struct_name,"settings",MAX_STRING-1);
                    }
                    else{
                        strncpy(prop_name,line,MAX_STRING-1);
                    }
                }

            }
        }
  
        fclose(file);
    }
    else{
        SDL_Log("arquivo nao encontrado");
    }
}

void print_color(FILE *file,char *name,SDL_Color color){

    fprintf(file,"\t%s\n",name);
    fprintf(file,"\t{\n");

    fprintf(file,"\t\tr = %d\n",color.r);
    fprintf(file,"\t\tg = %d\n",color.g);
    fprintf(file,"\t\tb = %d\n",color.b);
    fprintf(file,"\t\ta = %d\n",color.a);

    fprintf(file,"\t}\n");
}

void save_settings(Settings *setting){

    char *base_path = SDL_GetBasePath();

    char buffer[MAX_STRING];
    memset(buffer,'\0',MAX_STRING);
    snprintf(buffer,MAX_STRING-1,"%ssettings.txt",base_path);

    FILE *file = fopen(buffer,"w");

    if(file){
        fprintf(file,"settings\n");
        fprintf(file,"{\n");

        print_color(file,"text_color",setting->text_color);
        print_color(file,"border_color",setting->border_color);
        print_color(file,"background_color",setting->background_color);
        print_color(file,"background_color_selected",setting->background_color_selected);
        print_color(file,"quit_color",setting->quit_color);
        print_color(file,"background_color_map",setting->background_color_map);
        print_color(file,"grade_color",setting->grade_color);

        fprintf(file,"}\n");

        fclose(file);
    }
    else{
        SDL_Log("falha em salva arquivo");
    }
}

void append_tag_camada(Window_Layers *wCamadas,Tag_Camadas *tag_camada){
    Tag_Camadas *tag = wCamadas->tags;

    if(tag == NULL){
        wCamadas->tags = tag_camada;
        wCamadas->tag_atual = tag_camada;
    }
    else{
        while(tag->next != NULL){
            tag = tag->next;
        }
        tag_camada->previous = tag;
        tag->next = tag_camada;
    }
}

void save_map(Window_Map *wMap){
    FILE *file = fopen(wMap->path,"w");

    if(file){
        fprintf(file,"map\n");
        fprintf(file,"{\n");

        fprintf(file,"\n");

        fprintf(file,"\tpath_export = %s\n",((wMap->path_export) ? wMap->path_export : "NULL"));

        fprintf(file,"\n");

        fprintf(file,"\tproperties\n");
        fprintf(file,"\t{\n");
        fprintf(file,"\t\torientation = %s\n",wMap->properties->orientation);
        fprintf(file,"\t\ttype = %d\n",wMap->properties->type);
        fprintf(file,"\t\twidth_map = %d\n",wMap->properties->width_map);
        fprintf(file,"\t\theight_map = %d\n",wMap->properties->height_map);
        fprintf(file,"\t\twidth_block = %d\n",wMap->properties->width_block);
        fprintf(file,"\t\theight_block = %d\n",wMap->properties->height_block);
        fprintf(file,"\t\tsize = %d\n",wMap->properties->size);
        fprintf(file,"\t}\n");

        fprintf(file,"\n");

        fprintf(file,"\ttile_sets\n");
        fprintf(file,"\t{\n");
        Tag_Tiles *tag_tiles = wMap->wTiles->tag_tiles;
        while(tag_tiles != NULL){
            if(tag_tiles->type == TYPE_LOCAL) fprintf(file,"\t\tpath = %s\n",tag_tiles->path);
            tag_tiles = tag_tiles->next;
        }
        fprintf(file,"\t}\n");

        fprintf(file,"\n");

        fprintf(file,"\tlayers\n");
        fprintf(file,"\t{\n");
        Tag_Camadas *tag_camada = wMap->wCamadas->tags;
        while(tag_camada != NULL){
            fprintf(file,"\n");
            fprintf(file,"\t\t{\n");
            fprintf(file,"\t\t\tid = %d\n",tag_camada->id);
            fprintf(file,"\t\t\tname = %s\n",tag_camada->input_name.text);
            fprintf(file,"\t\t\tmatriz\n");
            fprintf(file,"\t\t\t{\n");
            char *linha = NULL;
            for(int y=0; y<wMap->properties->height_map; ++y){
                for(int x=0; x<wMap->properties->width_map; ++x){
                    if(x > 0) linha = cat(linha,",",0);
                    Tile *tile = tag_camada->matriz[y][x].tile;
                    linha = cat(linha,NULL,((tile != NULL) ? tile->id_local : -1));
                }
                fprintf(file,"\t\t\t\t{%s}\n",linha);
                free(linha);
                linha = NULL;
            }
            fprintf(file,"\t\t\t}\n");
            fprintf(file,"\t\t}\n");
            fprintf(file,"\n");
            tag_camada = tag_camada->next;
        }
        fprintf(file,"\t}\n");

        fprintf(file,"\n");

        fprintf(file,"}\n");

        fclose(file);
        wMap->updated = true;
    }
    else{
        printf("Falha em salvar %s\n",wMap->path);
    }
}

void load_layers(FILE *file,Window_Map *wMap,Settings *setting){

    char *linha = calloc(MAX_STRING_FILE,sizeof(char));

    char prop_value[MAX_STRING];
    memset(prop_value,'\0',MAX_STRING);

    char prop_name[MAX_STRING];
    memset(prop_name,'\0',MAX_STRING);

    char name[MAX_STRING];
    memset(name,'\0',MAX_STRING);

    Tag_Camadas *tag_camada = NULL;

    int nkeys = 0;
    int y = 0;

    while(fgets(linha,MAX_STRING_FILE-1,file)){
        trim(linha);
        if(lenght(linha)){
            int pos_igual = find_first_of(linha,'=');
            if(pos_igual != 0){
                substr(linha,prop_name,0,pos_igual);
                trim(prop_name);

                substr(linha,prop_value,pos_igual+1,lenght(linha));
                trim(prop_value);

                if(!strcmp(prop_name,"id")){
                    tag_camada->id = atoi(prop_value);
                }      
                else if(!strcmp(prop_name,"name")){
                    init_input(&tag_camada->input_name,prop_value,TYPE_TEXT);
                } 
            }
            else if(lenght(linha) == 1 && linha[0] == '{'){
                
                ++nkeys;
                if(nkeys == 2){

                    tag_camada = calloc(1,sizeof(Tag_Camadas));
                    tag_camada->id = 0;
                    tag_camada->clicked = false;
                    tag_camada->time = 0;
                    tag_camada->next = NULL;
                    tag_camada->previous = NULL;
                    
                    y = 0;
                    tag_camada->matriz = calloc(wMap->properties->height_map,sizeof(Bloco*));
                    for(int i=0; i<wMap->properties->height_map; ++i){
                        tag_camada->matriz[i] = calloc(wMap->properties->width_map,sizeof(Bloco));
                    }
                }
            }
            else if(lenght(linha) == 1 && linha[0] == '}'){
                --nkeys;
                if(nkeys == 1){
                    memset(name,'\0',MAX_STRING);
                    append_tag_camada(wMap->wCamadas,tag_camada);
                }
                else if(nkeys == 0){
                    break;
                }
            }
            else if(!strcmp(name,"matriz") && nkeys == 3){
                remove_char(linha,"{}");
                
                char *token = NULL;
                char *p = linha;

                int x = 0;

                while(*p != '\0'){

                    if(isdigit(*p) || *p == '-') token = append(token,*p,MAX_STRING);

                    ++p;

                    if(*p == '\0' || *p == ','){
                        if(token != NULL){
                            int id = atoi(token);
                            Bloco *bloco = &tag_camada->matriz[y][x];
                            bloco->x = x;
                            bloco->y = y;
                            bloco->tile = (id != -1) ? get_tile_in_tag_tiles(wMap->wTiles->tag_tiles,atoi(token),NULL) : NULL;
                            if(bloco->tile){
                                bloco->tag_tile = get_tag_tile(wMap->wTiles->tag_tiles,bloco->tile);
                            }
                            else{
                                bloco->tag_tile = NULL;
                            }
                            bloco->buffer = (bloco->tile) ? bloco->tile->id_local : -1;

                            free(token);
                            token = NULL;
                            ++x;
                        }

                    }
                }
                ++y;
            }
            else{
                strncpy(name,linha,MAX_STRING-1);
            }
        }
    }

    free(linha);
}

void load_map(Window_Map *wMap,Settings *setting,SDL_Window *window,SDL_Renderer *renderer){

    FILE *file = fopen(wMap->path,"r");

    Properties *properties;

    if(file){

        char linha[MAX_STRING];
        char name[MAX_STRING];
        char prop_name[MAX_STRING];
        char prop_value[MAX_STRING];

        memset(linha,'\0',MAX_STRING);
        memset(name,'\0',MAX_STRING);
        memset(prop_name,'\0',MAX_STRING);
        memset(prop_value,'\0',MAX_STRING);

        Properties *properties;

        while(fgets(linha,MAX_STRING-1,file)){
            trim(linha);
            if(lenght(linha)){
                int x = find_first_of(linha,'=');
                if(x != 0){
                    substr(linha,prop_name,0,x);
                    trim(prop_name);
                    
                    substr(linha,prop_value,x+1,lenght(linha));
                    trim(prop_value);

                    if(!strcmp(prop_name,"path_export")){
                        wMap->path_export = (!strcmp(prop_value,"NULL")) ? NULL : new_string(prop_value);
                    }

                    if(!strcmp(name,"properties")){
                        if(!strcmp(prop_name,"orientation"))       properties->orientation = new_string(prop_value);
                        else if(!strcmp(prop_name,"type"))         properties->type = atoi(prop_value);
                        else if(!strcmp(prop_name,"width_map"))    properties->width_map = atoi(prop_value);
                        else if(!strcmp(prop_name,"height_map"))   properties->height_map = atoi(prop_value);
                        else if(!strcmp(prop_name,"width_block"))  properties->width_block = atoi(prop_value);
                        else if(!strcmp(prop_name,"height_block")) properties->height_block = atoi(prop_value);
                        else if(!strcmp(prop_name,"size"))         properties->size = atoi(prop_value);
                    }
                    else if(!strcmp(name,"tile_sets") && !strcmp(prop_name,"path")){
                        append_tag_tile(wMap->wTiles,renderer,prop_value,TYPE_LOCAL);
                    }
                }
                else{
                    if(linha[0] == '{'){
                        if(!strcmp(name,"properties")) properties = calloc(1,sizeof(Properties));
                        
                    }
                    else if(linha[0] == '}'){
                        if(!strcmp(name,"properties")) map_init(wMap,setting,renderer,properties);
                        memset(name,'\0',MAX_STRING);
                    }
                    else{
                        strncpy(name,linha,MAX_STRING-1);
                        if(!strcmp(name,"layers")){
                            load_layers(file,wMap,setting);
                        }
                    }
                }
            }
        }
        fclose(file);
    }
    else{
        SDL_Log("Falha ao ler arquivo");
    }
}

void export_map(Window_Map *wMap){

    DIR *dir = opendir(wMap->path_export);
    if(!dir){
        printf("diretorio nao encontrado\n");
        return;
    }
    closedir(dir);

    char fullPath[MAX_STRING];
    char path_map[MAX_STRING];
    memset(path_map,'\0',MAX_STRING);
    memset(fullPath,'\0',MAX_STRING);

    strncpy(path_map,wMap->path,MAX_STRING-1);

    char *name = strtok(basename(path_map),".");

    Tag_Camadas *tag_camada = wMap->wCamadas->tags;
    char *linha = NULL;
    Tile *tile;

    while(tag_camada){
        snprintf(fullPath,MAX_STRING-1,"%s\\%s_%s.txt",wMap->path_export,name,tag_camada->input_name.buffer);
        FILE *file = fopen(fullPath,"w");

        for(int y=0; y<wMap->properties->height_map; ++y){
            for(int x=0; x<wMap->properties->width_map; ++x){
                if(x > 0) linha = cat(linha,",",0);
                tile = tag_camada->matriz[y][x].tile;
                linha = cat(linha,NULL,((tile) ? tile->id_global : -1));
            }
            fprintf(file,"%s\n",linha);
            free(linha);
            linha = NULL;
        }

        fclose(file);
        tag_camada = tag_camada->next;
    }
}

void save_tile_set_based_on_an_image(char *path_file,char *path_image,int tile_width,int tile_height,int spacing,int margin){
    FILE *file = fopen(path_file,"w");

    if(!file){
        printf("falha em salvar arquivo");
        return;
    }

    fprintf(file,"tile_set_based_on_an_image\n");
    fprintf(file,"{\n");
    fprintf(file,"\tpath = %s\n",path_image);
    fprintf(file,"\ttile_width = %d\n",tile_width);
    fprintf(file,"\ttile_height = %d\n",tile_height);
    fprintf(file,"\tspacing = %d\n",spacing);
    fprintf(file,"\tmargin = %d\n",margin);
    fprintf(file,"}\n");

    fclose(file);
}

void save_tile_set_based_on_multiple_images(Tile *tile,char *path){

    FILE *file = fopen(path,"w");

    if(!file){
        printf("falha em salvar arquivo");
        return;
    }

    fprintf(file,"tile_set_based_on_multiple_images\n");
    fprintf(file,"{\n");

    while(tile != NULL){
        fprintf(file,"\ttile\n");
        fprintf(file,"\t{\n");

        fprintf(file,"\t\tid_global = %d\n",tile->id_global);
        fprintf(file,"\t\tid_local = %d\n",tile->id_local);
        fprintf(file,"\t\twidth = %d\n",tile->largura);
        fprintf(file,"\t\theight = %d\n",tile->altura);
        fprintf(file,"\t\tpath = %s\n",tile->path);

        fprintf(file,"\t}\n");
        tile = tile->next;
    }

    fprintf(file,"}\n");

    fclose(file);
}

void load_tile_set_based_on_multiple_images(Tile **tiles,char *path,SDL_Renderer *renderer){

    free_tiles(&(*tiles));

    FILE *file = fopen(path,"r");

    if(file){

        Tile *tile = NULL;
        Tile *aux = NULL;

        char prop_name[MAX_STRING];
        char prop_value[MAX_STRING];
        char line[MAX_STRING];
        char name[MAX_STRING];

        memset(prop_name,'\0',MAX_STRING);
        memset(prop_value,'\0',MAX_STRING);
        memset(line,'\0',MAX_STRING);
        memset(name,'\0',MAX_STRING);

        while(fgets(line,MAX_STRING,file)){

            trim(line);

            if(lenght(line)){

                int x = find_first_of(line,'=');

                if(x != 0 && tile != NULL){

                    substr(line,prop_name,0,x);
                    trim(prop_name);

                    substr(line,prop_value,x+1,lenght(line));
                    trim(prop_value);
                    
                    if(!strcmp(prop_name,"id_global"))     tile->id_global = atoi(prop_value);

                    else if(!strcmp(prop_name,"id_local")) tile->id_local = atoi(prop_value);
                       
                    else if(!strcmp(prop_name,"width"))    tile->largura = atoi(prop_value);
                        
                    else if(!strcmp(prop_name,"height"))   tile->altura = atoi(prop_value);
                        
                    else if(!strcmp(prop_name,"path")){
                        tile->path = new_string(prop_value);
                        SDL_Surface *surface = IMG_Load(tile->path);
                        tile->texture = SDL_CreateTextureFromSurface(renderer,surface);
                        SDL_FreeSurface(surface);
                    }   
                }
                else{
                    if(line[0] == '{' && !strcmp(name,"tile")){
                        tile = calloc(1,sizeof(Tile));
                    }
                    else if(line[0] == '}' && lenght(name)){
                        if(*tiles == NULL){
                            *tiles = tile;
                            aux = tile;
                        }
                        else{
                            aux->next = tile;
                            aux = aux->next;
                            aux->next = NULL;
                        }
                        memset(name,'\0',MAX_STRING-1);
                    }
                    else{
                        strncpy(name,line,MAX_STRING-1);
                    }
                }
            }
        }
        fclose(file);
    }
    else{
        SDL_Log("falha ao carregar arquivo");
    }
}

Tile* load_tile_set_based_on_an_image(char *path,SDL_Renderer *renderer,size_t *number_of_columns){
    Tile *tile = NULL;
    Tile *aux = NULL;

    SDL_Surface *image;
    int tile_width = 0;
    int tile_height = 0;
    int spacing = 0;
    int margin = 0;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;

    char linha[MAX_STRING];
    char name[MAX_STRING];
    char prop_name[MAX_STRING];
    char prop_value[MAX_STRING];

    memset(name,'\0',MAX_STRING);
    memset(linha,'\0',MAX_STRING);
    memset(prop_name,'\0',MAX_STRING);
    memset(prop_value,'\0',MAX_STRING);

    FILE *file = fopen(path,"r");

    if(!file){
        printf("failed to load tiles\n");
        return NULL;
    }

    while(fgets(linha,MAX_STRING-1,file)){
        trim(linha);
        if(lenght(linha)){

            int x = find_first_of(linha,'=');

            if(x > 0 && !strcmp(name,"tile_set_based_on_an_image")){
                substr(linha,prop_name,0,x);
                trim(prop_name);

                substr(linha,prop_value,x+1,lenght(linha));
                trim(prop_value);

                if(!strcmp(prop_name,"path")){
                    image = IMG_Load(prop_value);
                }
                else if(!strcmp(prop_name,"tile_width")){
                    tile_width = atoi(prop_value);
                }
                else if(!strcmp(prop_name,"tile_height")){
                    tile_height = atoi(prop_value);
                }
                else if(!strcmp(prop_name,"spacing")){
                    spacing = atoi(prop_value);
                }
                else if(!strcmp(prop_name,"margin")){
                    margin = atoi(prop_value);
                }
            }
            else{
                if(linha[0] == '{'){
                    strncpy(name,prop_name,MAX_STRING-1);
                }
                else if(linha[0] == '}'){
                    break;
                }
                else{
                    strncpy(prop_name,linha,MAX_STRING-1);
                }
            }
        }
    }

    fclose(file);

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = tile_width;
    dst_rect.h = tile_height;

    *number_of_columns = image->w/tile_width;

    for(int y = margin; y<image->h; y+=(tile_height+spacing)){
        for(int x = margin; x<image->w; x+=(tile_width+spacing)){

            Tile *buffer_tile = calloc(1,sizeof(Tile));
            buffer_tile->largura = tile_width;
            buffer_tile->altura = tile_height;
            buffer_tile->path = NULL;
            buffer_tile->next = NULL;

            src_rect.x = x;
            src_rect.y = y;
            src_rect.w = tile_width;
            src_rect.h = tile_height;

            SDL_Surface *surface = SDL_CreateRGBSurface(0,tile_width,tile_height,32,0xff,0xff00,0xff0000,0xff000000);
            SDL_BlitSurface(image,&src_rect,surface,&dst_rect);
            buffer_tile->texture = SDL_CreateTextureFromSurface(renderer,surface);
            SDL_FreeSurface(surface);

            if(!tile){
                tile = buffer_tile;
                aux = buffer_tile;
            }
            else{
                aux->next = buffer_tile;
                aux = aux->next;
            }
        }
    }
    SDL_FreeSurface(image);

    return tile;
}