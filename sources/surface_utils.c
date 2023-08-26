#include <surface_utils.h>

void drawPixel(SDL_Surface *const surface,const SDL_Color color,const int x,const int y){
    unsigned char *data = surface->pixels;

    if(x >= 0 && x < surface->w && y >= 0 && y < surface->h){
        data[4 * (y * surface->w + x) + 0] = color.r;
        data[4 * (y * surface->w + x) + 1] = color.g;
        data[4 * (y * surface->w + x) + 2] = color.b;
        data[4 * (y * surface->w + x) + 3] = color.a;
    }
}

void fillRect(SDL_Surface *const surface,const SDL_Color color,const SDL_Rect *const rect){
    if(rect){
        for(int l=0; l<rect->h; l++){
            for(int c=0; c<rect->w; c++){
                drawPixel(surface,color,rect->x+c,rect->y+l);
            }
        }
    }
    else{
        for(int l=0; l<surface->h; l++){
            for(int c=0; c<surface->w; c++){
                drawPixel(surface,color,c,l);
            }
        }
    }
}

void drawRect(SDL_Surface *const surface,const SDL_Color color,const SDL_Rect *const rect){

    SDL_Point *points = calloc(4,sizeof(SDL_Point));

    if(rect){
        points[0].x = rect->x,         points[0].y = rect->y;
        points[1].x = rect->x+rect->w-1, points[1].y = rect->y;
        points[2].x = rect->x+rect->w-1, points[2].y = rect->y+rect->h-1;
        points[3].x = rect->x,         points[3].y = rect->y+rect->h-1;
    }
    else{
        points[0].x = 0,            points[0].y = 0;
        points[1].x = surface->w-1, points[1].y = 0;
        points[2].x = surface->w-1, points[2].y = surface->h-1;
        points[3].x = 0,            points[3].y = surface->h-1;
    }

    drawLines(surface,color,points,4,1);
    free(points);
}

void drawPolygon(SDL_Surface *const surface,const SDL_Color color,const SDL_Point *const pontos,const Bool fill){
    drawLine(surface,color,pontos[0],pontos[1]);
    drawLine(surface,color,pontos[1],pontos[2]);
    drawLine(surface,color,pontos[2],pontos[0]);
    if(fill){
        fillPolygon(surface,color,pontos,3);
    }
}

void fillPolygon(SDL_Surface *const surface,const SDL_Color color,const SDL_Point *const pontos,const int numVertices){
    int minY = pontos[0].y;
    int maxY = pontos[0].y;

    for(int i=0; i<numVertices; i++){
        if(pontos[i].y < minY){
            minY = pontos[i].y;
        }
        if(pontos[i].y > maxY){
            maxY = pontos[i].y;
        }
    }

    for(int y=minY; y<maxY; y++){
        int *intersections = calloc(numVertices,sizeof(int));
        int numIntersections = 0;

        for(int i = 0; i<numVertices; i++){
            int j = (i+1) % numVertices;
            SDL_Point v1 = pontos[i];
            SDL_Point v2 = pontos[j];

            if((v1.y <= y && v2.y > y) || (v1.y > y && v2.y <= y)){
                int intersectionX = v1.x + (y - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
                intersections[numIntersections++] = intersectionX;
            }
        }

        for(int i=0; i<numIntersections-1; i++){
            for(int j=0; j<numIntersections-i-1; j++){
                if(intersections[j] > intersections[j+1]){
                    int temp  = intersections[j];
                    intersections[j] = intersections[j+1];
                    intersections[j+1] = temp;
                }
            }
        }

        for(int i=0; i<numIntersections; i+=2){
            int startX = intersections[i];
            int endX = intersections[i+1];
            for(int x=startX; x<endX; x++){
                drawPixel(surface,color,x,y);
            }
        }

        free(intersections);
    }
}

void drawLines(SDL_Surface *const surface,const SDL_Color color,const SDL_Point *const points,const int count,const Bool close){
    if(count > 1){
        for(int i=0; i<count-1; i++){
            drawLine(surface,color,points[i],points[i+1]);
        }
        if(close){
            drawLine(surface,color,points[count-1],points[0]);
        }
    }
}

void drawLine(SDL_Surface *const surface,const SDL_Color color,const SDL_Point ponto1,const SDL_Point ponto2){
    int x1 = ponto1.x;
    int y1 = ponto1.y;
    int x2 = ponto2.x;
    int y2 = ponto2.y;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int controle = 0;
    int incX = 0;
    int incY = 0;

    if(dx >= dy) controle = dx / 2;
    else         controle = dy / 2;

    if(x2>x1) incX = 1;
    else incX = -1;

    if(y2>y1) incY = 1;
    else incY = -1;

    drawPixel(surface,color,x1,y1);

    while((x1 != x2 || y1 != y2)){
        if(dx == 0){
            y1 += incY;
        }
        else if(dy == 0){
            x1 += incX;
        }
        else{
            if(dx >= dy){
                x1 += incX;
                controle -= dy;
                if(controle < 0){
                    y1 += incY;
                    controle += dx;
                }
            }
            else{
                y1 += incY;
                controle -= dx;
                if(controle < 0){
                    x1 += incX;
                    controle += dy;
                }
            }
        }
        if(x1 >= 0 && x1 < surface->w && y1 >= 0 && y1 < surface->h) drawPixel(surface,color,x1,y1);
    }
}

void draw_font_surf_lite(SDL_Surface *const surface,SDL_Surface *const textSurf,const SDL_Rect destRect,const int center_x,const int center_y){
    SDL_Rect rect;
    rect.x = (center_x) ? (destRect.x+destRect.w*0.5)-textSurf->w*0.5 : destRect.x+2;
    rect.y = (center_y) ? (destRect.y+destRect.h*0.5)-textSurf->h*0.5 : destRect.y+2;
    rect.w = textSurf->w;
    rect.h = textSurf->h;
    SDL_BlitSurface(textSurf,NULL,surface,&rect);
}

void draw_text(SDL_Renderer *renderer,TTF_Font *font,char *text,SDL_Color color,SDL_Rect dst,Bool center_x,Bool center_y){
    if(lenght(text)){
        SDL_Rect rect;
        SDL_Surface *surface = TTF_RenderText_Solid(font,text,color);
        rect.x = (center_x) ? (dst.x+dst.w*0.5)-surface->w*0.5 : dst.x+2;
        rect.y = (center_y) ? (dst.y+dst.h*0.5)-surface->h*0.5 : dst.y+2;
        rect.w = surface->w;
        rect.h = surface->h;
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surface);
        SDL_RenderCopy(renderer,texture,NULL,&rect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void draw_font_surf_pro(SDL_Surface *const surface,TTF_Font *const font,const SDL_Color color,const char *const text,const SDL_Rect destRect,const Bool center_x,Bool const center_y){
    if(strlen(text)){
        SDL_Rect rect;
        SDL_Surface *textSurf = TTF_RenderText_Solid(font,text,color);
        rect.x = (center_x) ? (destRect.x+destRect.w*0.5)-textSurf->w*0.5 : destRect.x+2;
        rect.y = (center_y) ? (destRect.y+destRect.h*0.5)-textSurf->h*0.5 : destRect.y+2;
        rect.w = textSurf->w;
        rect.h = textSurf->h;

    SDL_BlitSurface(textSurf,NULL,surface,&rect);
    SDL_FreeSurface(textSurf);
    }
}

void draw_scroll_bar(ScrollBar *const scroll_bar,const Settings *const setting,SDL_Renderer *renderer){
    if(scroll_bar->visible){
        SDL_Color border_color = setting->border_color;
        SDL_Color background_color_selected = setting->background_color_selected;
        SDL_Color background_color = setting->background_color;

        SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
        SDL_RenderFillRect(renderer,&scroll_bar->rect);

        if(scroll_bar->selected || scroll_bar->on_bar){
            SDL_SetRenderDrawColor(renderer,background_color_selected.r,background_color_selected.g,background_color_selected.b,background_color_selected.a);
            SDL_RenderFillRect(renderer,&scroll_bar->subRect);
            scroll_bar->selected = false;
        }

        SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
        SDL_RenderDrawRect(renderer,&scroll_bar->rect);
        SDL_RenderDrawRect(renderer,&scroll_bar->subRect);
    }
}

void draw_button(SDL_Renderer *renderer,Button *button,Settings *setting,TTF_Font *font,SDL_Color text_color,SDL_Color border_color){
    SDL_Color background_color_selected = setting->background_color_selected;

    if(button->selected){
        SDL_SetRenderDrawColor(renderer,background_color_selected.r,background_color_selected.g,background_color_selected.b,background_color_selected.a);
        SDL_RenderFillRect(renderer,&button->rect);
        button->selected = false;
    }

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&button->rect);
    draw_text(renderer,font,button->text,text_color,button->rect,true,true);
}

void draw_input(SDL_Renderer *renderer,Input *input,Settings *setting,SDL_Color text_color,SDL_Color border_color){
 
    SDL_Color background_color = setting->background_color;
    SDL_Color background_color_selected = setting->background_color_selected;
    SDL_Rect rect;
    Uint32 fill_color;

    int current_time = SDL_GetTicks();
    if(input->on == true){
        if(current_time - input->time >= COLLDOWN_INPUT){
            input->time = SDL_GetTicks();
            input->visible = !input->visible;
        }
    }else{
        input->visible = false;
    }
    
    SDL_Surface *text_surface = SDL_CreateRGBSurface(0,input->rect.w,input->rect.h,32,0xff,0xff00,0xff0000,0xff000000);

    if(input->selected){
        fill_color = SDL_MapRGB(text_surface->format,background_color_selected.r,background_color_selected.g,background_color_selected.b);
    }
    else{
        fill_color = SDL_MapRGB(text_surface->format,background_color.r,background_color.g,background_color.b);
        
    }

    SDL_FillRect(text_surface,NULL,fill_color);

    if(lenght(input->buffer)){
        SDL_Surface *textSurf = TTF_RenderText_Solid(setting->font,input->buffer,text_color);
        
        if(input->on && textSurf->w > input->rect.w){
            rect.x = input->rect.w-textSurf->w-3;
        }else{
            rect.x = 1;
        }
        rect.y = input->rect.h*0.5-textSurf->h*0.5;
        rect.w = textSurf->w;
        rect.h = textSurf->h;

        SDL_BlitSurface(textSurf,NULL,text_surface,&rect);
        SDL_FreeSurface(textSurf);
    }

    if(input->visible){
        SDL_Point ponto1;
        SDL_Point ponto2;
        if(lenght(input->buffer)){
            ponto1.x = rect.w;
            ponto1.y = text_surface->h*0.1;
            ponto2.x = rect.w;
            ponto2.y = text_surface->h*0.9;
        }else{
            ponto1.x = 2;
            ponto1.y = text_surface->h*0.1;
            ponto2.x = 2;
            ponto2.y = text_surface->h*0.9;
        }
        drawLine(text_surface,border_color,ponto1,ponto2);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,text_surface);
    SDL_RenderCopy(renderer,texture,NULL,&input->rect);

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);
    SDL_RenderDrawRect(renderer,&input->rect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(texture);
}

void draw_button_quit(SDL_Renderer *renderer,Button *button,Settings *setting){
    SDL_Color border_color = setting->border_color;
    SDL_Color quit_color = setting->quit_color;
    SDL_Rect rect = button->rect;

    if(button->selected){
        SDL_SetRenderDrawColor(renderer,quit_color.r,quit_color.g,quit_color.b,quit_color.a);
        SDL_RenderFillRect(renderer,&button->rect);
        button->selected = false;
    }

    SDL_SetRenderDrawColor(renderer,border_color.r,border_color.g,border_color.b,border_color.a);

    SDL_Point ponto1 = {rect.x+rect.w*0.3,rect.y+rect.h*0.3};
    SDL_Point ponto2 = {rect.x+rect.w*0.7,rect.y+rect.h*0.7};
    SDL_RenderDrawLine(renderer,ponto1.x,ponto1.y,ponto2.x,ponto2.y);

    SDL_Point ponto3 = {rect.x+rect.w*0.7,rect.y+rect.h*0.3};
    SDL_Point ponto4 = {rect.x+rect.w*0.3,rect.y+rect.h*0.7};
    SDL_RenderDrawLine(renderer,ponto3.x,ponto3.y,ponto4.x,ponto4.y);
}