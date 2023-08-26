#include <window_map_informations.h>

void map_informations_init(Map_Informations *mapInfo,Properties *properties){
    mapInfo->properties = properties;

    mapInfo->titulo_text = new_string("informations");
    mapInfo->orientation_text = new_string("orientation:");
    mapInfo->width_text = new_string("map width:");
    mapInfo->height_text = new_string("map height:");
    mapInfo->width_block_text = new_string("block width:");
    mapInfo->height_block_text = new_string("block height:");
    mapInfo->pos_x_text = new_string("x:");
    mapInfo->pos_y_text = new_string("y:");
    mapInfo->current_id_text = new_string("id:");
    mapInfo->scale_text = new_string("scale:");
}

void map_informations_config_rect(Map_Informations *mapInfo,Settings *setting){
    TTF_Font *font = setting->fontlower;

    //titulo
    get_size_text(font,mapInfo->titulo_text,&mapInfo->titulo.w,NULL);
    mapInfo->titulo.x = PROPRIEDADE_SPACING;
    mapInfo->titulo.y = PROPRIEDADE_SPACING;
    mapInfo->titulo.h = mapInfo->rect.h*PROPRIEDADE_HEIGHT_ELEMENT_PERCENT;

    //orientation
    get_size_text(font,mapInfo->orientation_text,&mapInfo->orientation.w,NULL);
    mapInfo->orientation.x = PROPRIEDADE_SPACING;
    mapInfo->orientation.y = mapInfo->titulo.y+mapInfo->titulo.h+PROPRIEDADE_SPACING;
    mapInfo->orientation.h = mapInfo->rect.h*PROPRIEDADE_HEIGHT_ELEMENT_PERCENT;

    //width
    get_size_text(font,mapInfo->width_text,&mapInfo->width.w,NULL);
    mapInfo->width.x = PROPRIEDADE_SPACING;
    mapInfo->width.y = mapInfo->orientation.y+mapInfo->orientation.h+PROPRIEDADE_SPACING;
    mapInfo->width.h = mapInfo->rect.h*PROPRIEDADE_HEIGHT_ELEMENT_PERCENT;

    //height
    get_size_text(font,mapInfo->height_text,&mapInfo->height.w,NULL);
    mapInfo->height.x = PROPRIEDADE_SPACING;
    mapInfo->height.y = mapInfo->width.y+mapInfo->width.h+PROPRIEDADE_SPACING;
    mapInfo->height.h = mapInfo->rect.h*PROPRIEDADE_HEIGHT_ELEMENT_PERCENT;

    //width block
    get_size_text(font,mapInfo->width_block_text,&mapInfo->width_block.w,NULL);
    mapInfo->width_block.x = PROPRIEDADE_SPACING;
    mapInfo->width_block.y = mapInfo->height.y+mapInfo->height.h+PROPRIEDADE_SPACING;
    mapInfo->width_block.h = mapInfo->rect.h*PROPRIEDADE_HEIGHT_ELEMENT_PERCENT;

    //height block
    get_size_text(font,mapInfo->height_block_text,&mapInfo->height_block.w,NULL);
    mapInfo->height_block.x = PROPRIEDADE_SPACING;
    mapInfo->height_block.y = mapInfo->width_block.y+mapInfo->width_block.h+PROPRIEDADE_SPACING;
    mapInfo->height_block.h = mapInfo->rect.h*PROPRIEDADE_HEIGHT_ELEMENT_PERCENT;

    //scale
    get_size_text(font,mapInfo->scale_text,&mapInfo->scale.w,NULL);
    mapInfo->scale.x = PROPRIEDADE_SPACING;
    mapInfo->scale.y = mapInfo->height_block.y+mapInfo->height_block.h+PROPRIEDADE_SPACING;
    mapInfo->scale.h = mapInfo->rect.h*PROPRIEDADE_HEIGHT_ELEMENT_PERCENT;

    //pos_x
    get_size_text(font,mapInfo->pos_x_text,&mapInfo->pos_x.w,NULL);
    mapInfo->pos_x.x = PROPRIEDADE_SPACING;
    mapInfo->pos_x.y = mapInfo->scale.y+mapInfo->scale.h+PROPRIEDADE_SPACING;
    mapInfo->pos_x.h = mapInfo->rect.h*PROPRIEDADE_HEIGHT_ELEMENT_PERCENT;

    //pos_y
    get_size_text(font,mapInfo->pos_y_text,&mapInfo->pos_y.w,NULL);
    mapInfo->pos_y.x = PROPRIEDADE_SPACING;
    mapInfo->pos_y.y = mapInfo->pos_x.y+mapInfo->pos_x.h+PROPRIEDADE_SPACING;
    mapInfo->pos_y.h = mapInfo->rect.h*PROPRIEDADE_HEIGHT_ELEMENT_PERCENT;

    //current_id
    get_size_text(font,mapInfo->current_id_text,&mapInfo->current_id.w,NULL);
    mapInfo->current_id.x = PROPRIEDADE_SPACING;
    mapInfo->current_id.y = mapInfo->pos_y.y+mapInfo->pos_y.h+PROPRIEDADE_SPACING;
    mapInfo->current_id.h = mapInfo->rect.h*PROPRIEDADE_HEIGHT_ELEMENT_PERCENT;

    mapInfo->orientation_rect.x = mapInfo->height_block.x+mapInfo->height_block.w+PROPRIEDADE_SPACING;
    mapInfo->orientation_rect.y = mapInfo->orientation.y;
    mapInfo->orientation_rect.w = mapInfo->rect.w-mapInfo->orientation_rect.x-PROPRIEDADE_SPACING;
    mapInfo->orientation_rect.h = mapInfo->orientation.h;

    mapInfo->width_rect.x = mapInfo->orientation_rect.x;
    mapInfo->width_rect.y = mapInfo->width.y;
    mapInfo->width_rect.w = mapInfo->orientation_rect.w;
    mapInfo->width_rect.h = mapInfo->width.h;

    mapInfo->height_rect.x = mapInfo->width_rect.x;
    mapInfo->height_rect.y = mapInfo->height.y;
    mapInfo->height_rect.w = mapInfo->width_rect.w;
    mapInfo->height_rect.h = mapInfo->height.h;

    mapInfo->width_block_rect.x = mapInfo->height_rect.x;
    mapInfo->width_block_rect.y = mapInfo->width_block.y;
    mapInfo->width_block_rect.w = mapInfo->height_rect.w;
    mapInfo->width_block_rect.h = mapInfo->width_block.h;

    mapInfo->height_block_rect.x = mapInfo->width_block_rect.x;
    mapInfo->height_block_rect.y = mapInfo->height_block.y;
    mapInfo->height_block_rect.w = mapInfo->width_block_rect.w;
    mapInfo->height_block_rect.h = mapInfo->height_block.h;

    mapInfo->pos_x_rect.x = mapInfo->height_block_rect.x;
    mapInfo->pos_x_rect.y = mapInfo->pos_x.y;
    mapInfo->pos_x_rect.w = mapInfo->height_block_rect.w;
    mapInfo->pos_x_rect.h = mapInfo->pos_x.h;

    mapInfo->pos_y_rect.x = mapInfo->pos_x_rect.x;
    mapInfo->pos_y_rect.y = mapInfo->pos_y.y;
    mapInfo->pos_y_rect.w = mapInfo->pos_x_rect.w;
    mapInfo->pos_y_rect.h = mapInfo->pos_y.h;

    mapInfo->scale_rect.x = mapInfo->pos_y_rect.x;
    mapInfo->scale_rect.y = mapInfo->scale.y;
    mapInfo->scale_rect.w = mapInfo->pos_y_rect.w;
    mapInfo->scale_rect.h = mapInfo->scale.h;

    mapInfo->current_id_rect.x = mapInfo->scale_rect.x;
    mapInfo->current_id_rect.y = mapInfo->current_id.y;
    mapInfo->current_id_rect.w = mapInfo->scale_rect.w;
    mapInfo->current_id_rect.h = mapInfo->current_id.h;
}

void map_informations_run(Map_Informations *mapInfo,Settings *setting,SDL_Renderer *renderer,Mouse *mouse){
    map_informations_draw(mapInfo,setting,renderer,mouse);
}

void map_informations_draw(Map_Informations *mapInfo,Settings *setting,SDL_Renderer *renderer,Mouse *mouse){
    const SDL_Color text_color = setting->text_color;
    const SDL_Color background_color = setting->background_color;
    const SDL_Color border_color = setting->border_color;
    TTF_Font *font = setting->fontlower;

    get_pos_in_map(mouse,mapInfo->properties);

    SDL_RenderSetViewport(renderer,&mapInfo->rect);

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,NULL);

    char buffer[MAX_STRING];
    memset(buffer,'\0',MAX_STRING);

    //titulo
    draw_text(renderer,font,mapInfo->titulo_text,text_color,mapInfo->titulo,true,true);

    //orientation
    draw_text(renderer,font,mapInfo->orientation_text,text_color,mapInfo->orientation,false,true);
    draw_text(renderer,font,mapInfo->properties->orientation,text_color,mapInfo->orientation_rect,false,true);

    //width
    draw_text(renderer,font,mapInfo->width_text,text_color,mapInfo->width,false,true);
    snprintf(buffer,MAX_STRING-1,"%d",mapInfo->properties->width_map);
    draw_text(renderer,font,buffer,text_color,mapInfo->width_rect,false,true);

    //height
    draw_text(renderer,font,mapInfo->height_text,text_color,mapInfo->height,false,true);
    snprintf(buffer,MAX_STRING-1,"%d",mapInfo->properties->height_map);
    draw_text(renderer,font,buffer,text_color,mapInfo->height_rect,false,true);

    //width block
    draw_text(renderer,font,mapInfo->width_block_text,text_color,mapInfo->width_block,false,true);
    snprintf(buffer,MAX_STRING-1,"%d",mapInfo->properties->width_block);
    draw_text(renderer,font,buffer,text_color,mapInfo->width_block_rect,false,true);

    //height block
    draw_text(renderer,font,mapInfo->height_block_text,text_color,mapInfo->height_block,false,true);
    snprintf(buffer,MAX_STRING-1,"%d",mapInfo->properties->height_block);
    draw_text(renderer,font,buffer,text_color,mapInfo->height_block_rect,false,true);

    //pos x
    draw_text(renderer,font,mapInfo->pos_x_text,text_color,mapInfo->pos_x,false,true);
    snprintf(buffer,MAX_STRING-1,"%d",mapInfo->properties->pos_x);
    draw_text(renderer,font,buffer,text_color,mapInfo->pos_x_rect,false,true);

    //pos y
    draw_text(renderer,font,mapInfo->pos_y_text,text_color,mapInfo->pos_y,false,true);
    snprintf(buffer,MAX_STRING-1,"%d",mapInfo->properties->pos_y);
    draw_text(renderer,font,buffer,text_color,mapInfo->pos_y_rect,false,true);

    //scale
    draw_text(renderer,font,mapInfo->scale_text,text_color,mapInfo->scale,false,true);
    snprintf(buffer,MAX_STRING-1,"%d%%",(int)(round(mapInfo->properties->scale*100.0f)));
    draw_text(renderer,font,buffer,text_color,mapInfo->scale_rect,false,true);

    //current id
    draw_text(renderer,font,mapInfo->current_id_text,text_color,mapInfo->current_id,false,true);
    snprintf(buffer,MAX_STRING-1,"%d",mapInfo->properties->current_id);
    draw_text(renderer,font,buffer,text_color,mapInfo->current_id_rect,false,true);

    SDL_RenderSetViewport(renderer,NULL);
}

void map_informations_free(Map_Informations **mapInfo){
    
    free((*mapInfo)->titulo_text);
    free((*mapInfo)->orientation_text);
    free((*mapInfo)->width_text);
    free((*mapInfo)->height_text);
    free((*mapInfo)->width_block_text);
    free((*mapInfo)->height_block_text);
    free((*mapInfo)->pos_x_text);
    free((*mapInfo)->pos_y_text);
    free((*mapInfo)->current_id_text);
    free((*mapInfo)->scale_text);

    free(*mapInfo);

    *mapInfo = NULL;
}