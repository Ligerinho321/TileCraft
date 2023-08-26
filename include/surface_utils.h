#ifndef SURFACE_UTILS_H_INCLUDE
#define SURFACE_UTILS_H_INCLUDE

#include <header.h>
#include <structs.h>
#include <string_utils.h>

void drawPixel(SDL_Surface *const surface,const SDL_Color color,const int x,const int y);

void fillPolygon(SDL_Surface *const surface,const SDL_Color color,const SDL_Point *const pontos,const int numVertices);

void drawPolygon(SDL_Surface *const surface,const SDL_Color color,const SDL_Point *const pontos,const Bool fill);

void drawRect(SDL_Surface *const surface,const SDL_Color color,const SDL_Rect *const rect);

void fillRect(SDL_Surface *const surface,const SDL_Color color,const SDL_Rect *const rect);

void drawLines(SDL_Surface *const surface,const SDL_Color color,const SDL_Point *const points,const int count,const Bool close);

void drawLine(SDL_Surface *const surface,const SDL_Color color,const SDL_Point ponto1,const SDL_Point ponto2);

void draw_font_surf_lite(SDL_Surface *const surface,SDL_Surface *const textSurf,const SDL_Rect destRect,const int center_x,const int center_y);

void draw_text(SDL_Renderer *renderer,TTF_Font *font,char *text,SDL_Color color,SDL_Rect dst,Bool center_x,Bool center_y);

void draw_font_surf_pro(SDL_Surface *const surface,TTF_Font *const font,const SDL_Color color,const char *const text,const SDL_Rect destRect,const Bool center_x,const Bool center_y);

void draw_scroll_bar(ScrollBar *const scrollBar,const Settings *const setting,SDL_Renderer *renderer);

void draw_button(SDL_Renderer *renderer,Button *button,Settings *setting,TTF_Font *font,SDL_Color text_color,SDL_Color border_color);

void draw_input(SDL_Renderer *renderer,Input *input,Settings *setting,SDL_Color text_color,SDL_Color border_color);

void draw_button_quit(SDL_Renderer *renderer,Button *button,Settings *setting);

#endif