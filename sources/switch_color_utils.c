#include <switch_color_utils.h>

void init_switch_color(Switch_Color *switch_color,SDL_Color *buffer_color,Settings *setting){

    switch_color->buffer_color = buffer_color;

    init_switch_uint8_t(&switch_color->r,"R:",switch_color->buffer_color->r,setting);
    init_switch_uint8_t(&switch_color->g,"G:",switch_color->buffer_color->g,setting);
    init_switch_uint8_t(&switch_color->b,"B:",switch_color->buffer_color->b,setting);
};

void init_switch_uint8_t(Switch_Uint8_t *switch_uint8_t,char *name,uint8_t value,Settings *setting){

    switch_uint8_t->name = new_string(name);
    switch_uint8_t->value = value;
    switch_uint8_t->on = false;
    switch_uint8_t->mouse_diff = 0;

};

void config_switch_color(Switch_Color *switch_color,Settings *setting){

    config_switch_sigla_rect(switch_color,setting);
    config_switch_value_rect(switch_color,setting);

    config_switch_interruptor(switch_color,&switch_color->r);
    config_switch_interruptor(switch_color,&switch_color->g);
    config_switch_interruptor(switch_color,&switch_color->b);

    switch_color->rect_color.x = switch_color->rect.x+6;
    switch_color->rect_color.y = switch_color->b.sigla_rect.y+switch_color->b.sigla_rect.h+switch_color->rect.h*0.05;
    switch_color->rect_color.w = switch_color->rect.w-12;
    switch_color->rect_color.h = (switch_color->rect.y+switch_color->rect.h) - switch_color->rect_color.y - 6;

}

void config_switch_sigla_rect(Switch_Color *switch_color,Settings *setting){
    switch_color->r.sigla_rect.x = switch_color->rect.x+6;
    switch_color->r.sigla_rect.y = switch_color->rect.y+6;
    set_size_rect(&switch_color->r.sigla_rect,setting->font,switch_color->r.name,0,0);

    switch_color->g.sigla_rect.x = switch_color->rect.x+6;
    switch_color->g.sigla_rect.y = switch_color->r.sigla_rect.y+switch_color->r.sigla_rect.h+switch_color->rect.h*0.05;
    set_size_rect(&switch_color->g.sigla_rect,setting->font,switch_color->g.name,0,0);

    switch_color->b.sigla_rect.x = switch_color->rect.x+6;
    switch_color->b.sigla_rect.y = switch_color->g.sigla_rect.y+switch_color->g.sigla_rect.h+switch_color->rect.h*0.05;
    set_size_rect(&switch_color->b.sigla_rect,setting->font,switch_color->b.name,0,0);
}

void config_switch_value_rect(Switch_Color *switch_color,Settings *setting){

    SDL_Surface *buffer = TTF_RenderText_Solid(setting->font,"255",setting->text_color);

    switch_color->r.value_rect.x = (switch_color->rect.x+switch_color->rect.w)-buffer->w-6;
    switch_color->r.value_rect.y = switch_color->r.sigla_rect.y;
    switch_color->r.value_rect.w = buffer->w;
    switch_color->r.value_rect.h = buffer->h;

    switch_color->g.value_rect.x = (switch_color->rect.x+switch_color->rect.w)-buffer->w-6;
    switch_color->g.value_rect.y = switch_color->g.sigla_rect.y;
    switch_color->g.value_rect.w = buffer->w;
    switch_color->g.value_rect.h = buffer->h;

    switch_color->b.value_rect.x = (switch_color->rect.x+switch_color->rect.w)-buffer->w-6;
    switch_color->b.value_rect.y = switch_color->b.sigla_rect.y;
    switch_color->b.value_rect.w = buffer->w;
    switch_color->b.value_rect.h = buffer->h;

    SDL_FreeSurface(buffer);
}

void config_switch_interruptor(Switch_Color *switch_color,Switch_Uint8_t *switch_uint8_t){
    switch_uint8_t->interruptor.y = switch_uint8_t->sigla_rect.y;
    switch_uint8_t->interruptor.w = switch_color->rect.w*0.035;
    switch_uint8_t->interruptor.h = switch_uint8_t->sigla_rect.h;

    switch_uint8_t->ponto1.x = (switch_uint8_t->sigla_rect.x+switch_uint8_t->sigla_rect.w)+switch_uint8_t->interruptor.w*0.5+4;
    switch_uint8_t->ponto1.y = switch_uint8_t->sigla_rect.y+switch_uint8_t->interruptor.h*0.5;

    switch_uint8_t->ponto2.x = switch_uint8_t->value_rect.x-switch_uint8_t->interruptor.w*0.5-4;
    switch_uint8_t->ponto2.y = switch_uint8_t->ponto1.y;

    set_pos_interruptor(switch_color,switch_uint8_t);
}

void set_pos_interruptor(Switch_Color *switch_color,Switch_Uint8_t *switch_uint8_t){
    float percent = (float)switch_uint8_t->value / 255.0f;
    switch_uint8_t->interruptor.x = (switch_uint8_t->ponto1.x + ((switch_uint8_t->ponto2.x - switch_uint8_t->ponto1.x)*percent))-switch_uint8_t->interruptor.w*0.5;
}

void set_value_switch(Switch_Uint8_t *switch_uint8_t,Settings *setting){

    int pos_x = (switch_uint8_t->interruptor.x+switch_uint8_t->interruptor.w*0.5)-switch_uint8_t->ponto1.x;
    int size = switch_uint8_t->ponto2.x - switch_uint8_t->ponto1.x;
    float percent = (float)pos_x / (float)size;

    int value = 255.0f * percent;
    if(value > 255) value = 255;
    if(value < 0)   value = 0;

    switch_uint8_t->value = value;
}

void input_switch_color(Switch_Color *switch_color,SDL_Event *event,Mouse *mouse,Settings *setting,SDL_Rect *rect_offset){

    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT){
        if(collidePointOffset(mouse,&switch_color->r.interruptor,0,0,rect_offset)){
            switch_color->r.on = true;
            switch_color->r.mouse_diff = mouse->pos_x - switch_color->r.interruptor.x;
        }
        else if(collidePointOffset(mouse,&switch_color->g.interruptor,0,0,rect_offset)){
            switch_color->g.on = true;
            switch_color->g.mouse_diff = mouse->pos_x - switch_color->g.interruptor.x;
        }
        else if(collidePointOffset(mouse,&switch_color->b.interruptor,0,0,rect_offset)){
            switch_color->b.on = true;
            switch_color->b.mouse_diff = mouse->pos_x - switch_color->b.interruptor.x;
        }
    }
    else if(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT){
        switch_color->r.on = false;
        switch_color->g.on = false;
        switch_color->b.on = false;
    }

    input_switch_uint8_t(&switch_color->r,mouse,setting);
    input_switch_uint8_t(&switch_color->g,mouse,setting);
    input_switch_uint8_t(&switch_color->b,mouse,setting);

    switch_color->buffer_color->r = switch_color->r.value;
    switch_color->buffer_color->g = switch_color->g.value;
    switch_color->buffer_color->b = switch_color->b.value;

}

void input_switch_uint8_t(Switch_Uint8_t *switch_uint8_t,Mouse *mouse,Settings *setting){
    if(switch_uint8_t->on){
        switch_uint8_t->interruptor.x = mouse->pos_x - switch_uint8_t->mouse_diff;
        

        if(switch_uint8_t->interruptor.x+switch_uint8_t->interruptor.w*0.5 < switch_uint8_t->ponto1.x){
            switch_uint8_t->interruptor.x = switch_uint8_t->ponto1.x - switch_uint8_t->interruptor.w*0.5;
        }
        else if(switch_uint8_t->interruptor.x+switch_uint8_t->interruptor.w*0.5+1 > switch_uint8_t->ponto2.x){
            switch_uint8_t->interruptor.x = switch_uint8_t->ponto2.x - switch_uint8_t->interruptor.w*0.5+1;
        }

        set_value_switch(switch_uint8_t,setting);
    }
}

void draw_switch_uint8_t(Switch_Uint8_t *switch_uint8_t,SDL_Renderer *renderer,Settings *setting){
    const SDL_Color border_color = setting->border_color;
    const SDL_Color text_color = setting->text_color;

    draw_text(renderer,setting->font,switch_uint8_t->name,setting->text_color,switch_uint8_t->sigla_rect,true,true);

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);

    SDL_RenderDrawLine(renderer,switch_uint8_t->ponto1.x,switch_uint8_t->ponto1.y,switch_uint8_t->ponto2.x,switch_uint8_t->ponto2.y);

    SDL_RenderFillRect(renderer,&switch_uint8_t->interruptor);

    char value[MAX_STRING];
    memset(value,'\0',MAX_STRING);
    itoa(switch_uint8_t->value,value,10);

    draw_text(renderer,setting->font,value,text_color,switch_uint8_t->value_rect,true,true);
}

void draw_switch_color(Switch_Color *switch_color,SDL_Renderer *renderer,Settings *setting){

    draw_switch_uint8_t(&switch_color->r,renderer,setting);
    draw_switch_uint8_t(&switch_color->g,renderer,setting);
    draw_switch_uint8_t(&switch_color->b,renderer,setting);

    SDL_SetRenderDrawColor(renderer,switch_color->buffer_color->r,switch_color->buffer_color->g,switch_color->buffer_color->b,255);
    SDL_RenderFillRect(renderer,&switch_color->rect_color);

    SDL_SetRenderDrawColor(renderer,setting->border_color.r,setting->border_color.g,setting->border_color.b,setting->border_color.a);
    SDL_RenderDrawRect(renderer,&switch_color->rect);
}

void free_switch_color(Switch_Color *switch_color){
    free(switch_color->r.name);
    free(switch_color->g.name);
    free(switch_color->b.name);
}