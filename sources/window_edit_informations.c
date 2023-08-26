#include <window_edit_informations.h>

void edit_info_init(Edit_Informations *editInfo){

    editInfo->titulo_text = new_string("informations");
    editInfo->id_text = new_string("id:");
    editInfo->largura_text = new_string("width:");
    editInfo->altura_text = new_string("height:");
    editInfo->name_text = new_string("name:");
    editInfo->size_text = new_string("size:");
}

void edit_info_config_rect(Edit_Informations *editInfo,Settings *setting){
    
    //informations
    get_size_text(setting->fontlower,editInfo->titulo_text,&editInfo->titulo.w,&editInfo->titulo.h);
    editInfo->titulo.x = INFO_SPACING;
    editInfo->titulo.y = INFO_SPACING;
    editInfo->titulo.h = editInfo->rect.h*INFO_HEIGHT_ELEMENT_PERCENT;

    //id
    get_size_text(setting->fontlower,editInfo->id_text,&editInfo->id.w,&editInfo->id.h);
    editInfo->id.x = INFO_SPACING;
    editInfo->id.y = editInfo->titulo.y+editInfo->titulo.h+INFO_SPACING;
    editInfo->id.h = editInfo->rect.h*INFO_HEIGHT_ELEMENT_PERCENT;

    //width
    get_size_text(setting->fontlower,editInfo->largura_text,&editInfo->largura.w,&editInfo->largura.h);
    editInfo->largura.x = INFO_SPACING;
    editInfo->largura.y = editInfo->id.y+editInfo->id.h+INFO_SPACING;
    editInfo->largura.h = editInfo->rect.h*INFO_HEIGHT_ELEMENT_PERCENT;

    //height
    get_size_text(setting->fontlower,editInfo->altura_text,&editInfo->altura.w,&editInfo->altura.h);
    editInfo->altura.x = INFO_SPACING;
    editInfo->altura.y = editInfo->largura.y+editInfo->largura.h+INFO_SPACING;
    editInfo->altura.h = editInfo->rect.h*INFO_HEIGHT_ELEMENT_PERCENT;

    //name
    get_size_text(setting->fontlower,editInfo->name_text,&editInfo->name.w,&editInfo->name.h);
    editInfo->name.x = INFO_SPACING;
    editInfo->name.y = editInfo->altura.y+editInfo->altura.h+INFO_SPACING;
    editInfo->name.h = editInfo->rect.h*INFO_HEIGHT_ELEMENT_PERCENT;

    //size
    get_size_text(setting->fontlower,editInfo->size_text,&editInfo->size.w,&editInfo->size.h);
    editInfo->size.x = INFO_SPACING;
    editInfo->size.y = editInfo->name.y+editInfo->name.h+INFO_SPACING;
    editInfo->size.h = editInfo->rect.h*INFO_HEIGHT_ELEMENT_PERCENT;

    //id rect
    editInfo->id_rect.x = editInfo->altura.x+editInfo->altura.w+INFO_SPACING;
    editInfo->id_rect.y = editInfo->id.y;
    editInfo->id_rect.w = editInfo->rect.w-editInfo->id_rect.x-INFO_SPACING;
    editInfo->id_rect.h = editInfo->id.h;

    //width rect
    editInfo->largura_rect.x = editInfo->id_rect.x;
    editInfo->largura_rect.y = editInfo->largura.y;
    editInfo->largura_rect.w = editInfo->id_rect.w;
    editInfo->largura_rect.h = editInfo->largura.h;

    //height rect
    editInfo->altura_rect.x = editInfo->largura_rect.x;
    editInfo->altura_rect.y = editInfo->altura.y;
    editInfo->altura_rect.w = editInfo->largura_rect.w;
    editInfo->altura_rect.h = editInfo->altura.h;

    //name rect
    editInfo->name_rect.x = editInfo->altura_rect.x;
    editInfo->name_rect.y = editInfo->name.y;
    editInfo->name_rect.w = editInfo->altura_rect.w;
    editInfo->name_rect.h = editInfo->name.h;

    //size rect
    editInfo->size_rect.x = editInfo->name_rect.x;
    editInfo->size_rect.y = editInfo->size.y;
    editInfo->size_rect.w = editInfo->name_rect.w;
    editInfo->size_rect.h = editInfo->size.h;
}

void edit_info_draw(Edit_Informations *editInfo,Settings *setting,SDL_Renderer *renderer,Tile *tile,int size){
    SDL_Color background_color = setting->background_color;
    SDL_Color background_color_selected = setting->background_color_selected;
    SDL_Color border_color = setting->border_color;
    SDL_Color text_color = setting->text_color;
    TTF_Font *font = setting->fontlower;

    SDL_RenderSetViewport(renderer,&editInfo->rect);

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,NULL);

    char buffer[MAX_STRING];
    memset(buffer,'\0',MAX_STRING);

    //informations
    draw_text(renderer,font,editInfo->titulo_text,text_color,editInfo->titulo,true,true);

    //id
    draw_text(renderer,font,editInfo->id_text,text_color,editInfo->id,true,true);
    snprintf(buffer,MAX_STRING-1,"%d",((tile != NULL) ? tile->id_global : -1));
    draw_text(renderer,font,buffer,text_color,editInfo->id_rect,false,true);

    //width
    draw_text(renderer,font,editInfo->largura_text,text_color,editInfo->largura,true,true);
    snprintf(buffer,MAX_STRING-1,"%d",((tile != NULL) ? tile->largura : 0));
    draw_text(renderer,font,buffer,text_color,editInfo->largura_rect,false,true);

    //height
    draw_text(renderer,font,editInfo->altura_text,text_color,editInfo->altura,true,true);
    snprintf(buffer,MAX_STRING-1,"%d",((tile != NULL) ? tile->altura : 0));
    draw_text(renderer,font,buffer,text_color,editInfo->altura_rect,false,true);

    //name
    draw_text(renderer,font,editInfo->name_text,text_color,editInfo->name,true,true);
    if(tile && tile->path){
        snprintf(buffer,MAX_STRING-1,"%s",basename(tile->path));
    }
    else{
        snprintf(buffer,MAX_STRING-1,"none");
    }
    draw_text(renderer,font,buffer,text_color,editInfo->name_rect,false,true);

    //size
    draw_text(renderer,font,editInfo->size_text,text_color,editInfo->size,true,true);
    snprintf(buffer,MAX_STRING-1,"%d",size);
    draw_text(renderer,font,buffer,text_color,editInfo->size_rect,false,true);

    SDL_RenderSetViewport(renderer,NULL);
}

void edit_info_run(Edit_Informations *editInfo,SDL_Renderer *renderer,Settings *setting,Mouse *mouse,SDL_Event *event,Tile *tile,int size){
    edit_info_draw(editInfo,setting,renderer,tile,size);
}

void edit_info_free(Edit_Informations **editInfo){
    if(*editInfo != NULL){

        free((*editInfo)->titulo_text);
        free((*editInfo)->id_text);
        free((*editInfo)->largura_text);
        free((*editInfo)->altura_text);
        free((*editInfo)->name_text);
        free((*editInfo)->size_text);

        free(*editInfo);
    }
}