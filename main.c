#include <utils.h>
#include <window_menu.h>
#include <window_create_map.h>
#include <window_new_tile_set.h>
#include <window_project.h>
#include <window_file.h>
#include <window_settings.h>
#include <file_utils.h>

Data* data_init(){

    Data *data = calloc(1,sizeof(Data));

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0,&dm);
    data->screen_w = dm.w;
    data->screen_h = dm.h;

    data->run = true;
    data->update_rects = false;

    data->window = SDL_CreateWindow("TileCraft",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,data->screen_w,data->screen_h,SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
    data->renderer = SDL_CreateRenderer(data->window,-1,SDL_RENDERER_ACCELERATED);
    
    SDL_Rect rect = {0,0,data->screen_w,data->screen_h};
    SDL_RenderSetClipRect(data->renderer,&rect);

    init_settings(data);

    data->window_settings = NULL;
    data->window_project = NULL;
    data->window_file = NULL;
    data->window_create = NULL;
    data->window_new_tile_set = NULL;

    data->window_menu = calloc(1,sizeof(Window_Menu));
    menu_init(data->window_menu);
    menu_config_rect(data->window_menu,data->setting,data->screen_w,data->screen_h);
    
    data->mouse = calloc(1,sizeof(Mouse));
    createMouse(data->mouse,data->renderer);

    data->event = calloc(1,sizeof(SDL_Event));

    return data;
}

void data_free(Data *data){
    create_free(&data->window_create);
    menu_free(&data->window_menu);
    file_free(&data->window_file);
    project_free(&data->window_project);
    setting_free(&data->window_settings);
    new_tile_free(&data->window_new_tile_set);
    free_settings(data);

    free(data->mouse);
    free(data->event);

    SDL_DestroyWindow(data->window);
    SDL_DestroyRenderer(data->renderer);

    free(data);
}

int main(int argc, char **argv){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_SetHint(SDL_HINT_RENDER_VSYNC,"1");
    
    Data *data = data_init();

    SDL_StartTextInput();

    SDL_SetRenderDrawBlendMode(data->renderer,SDL_BLENDMODE_BLEND);

    while(data->run){

        SDL_Color background_color = data->setting->background_color;

        SDL_PollEvent(data->event);

        SDL_GetRelativeMouseState(&data->mouse->rel_x,&data->mouse->rel_y);

        SDL_GetMouseState(&data->mouse->pos_x,&data->mouse->pos_y);

        if(data->event->type == SDL_KEYDOWN){
            
            if(data->event->key.keysym.scancode == SDL_SCANCODE_F11){
                SDL_SetWindowFullscreen(data->window,SDL_WINDOW_FULLSCREEN);
            }
            else if(data->event->key.keysym.scancode == SDL_SCANCODE_ESCAPE){

                if(SDL_GetWindowFlags(data->window) & SDL_WINDOW_FULLSCREEN){
                    SDL_SetWindowFullscreen(data->window,false);
                }
                else{
                    data->run = false;
                }
            }
        }

        if(data->event->type == SDL_WINDOWEVENT && (data->event->window.event == SDL_WINDOWEVENT_RESIZED || data->event->window.event == SDL_WINDOWEVENT_FOCUS_GAINED)){
            data->update_rects = true;
            SDL_GetWindowSize(data->window,&data->screen_w,&data->screen_h);
            SDL_Rect rect = {0,0,data->screen_w,data->screen_h};
            SDL_RenderSetClipRect(data->renderer,&rect);
        }

        if(data->update_rects){
            if(data->window_settings)     setting_config_rect(data->window_settings,data->setting,data->screen_w,data->screen_h);
            if(data->window_project)      project_config_rect(data->window_project,data->setting,data->screen_w,data->screen_h);
            if(data->window_file)         file_config_rect(data->window_file,data->setting,data->screen_w,data->screen_h);
            if(data->window_create)       create_config_rect(data->window_create,data->setting,data->screen_w,data->screen_h);
            if(data->window_menu)         menu_config_rect(data->window_menu,data->setting,data->screen_w,data->screen_h);
            if(data->window_new_tile_set) new_tile_config_rect(data->window_new_tile_set,data->setting,data->screen_w,data->screen_h);
            data->update_rects = false;
        }

        if(data->event->type == SDL_MOUSEBUTTONDOWN){
            if(data->event->button.button == SDL_BUTTON_LEFT){
                data->mouse->press_left = true;
            }
            else if(data->event->button.button == SDL_BUTTON_RIGHT){
                data->mouse->press_right = true;
            }
            
        }
        else if(data->event->type == SDL_MOUSEBUTTONUP){
            if(data->event->button.button == SDL_BUTTON_LEFT){
               data->mouse->press_left = false;
            }
            else if(data->event->button.button == SDL_BUTTON_RIGHT){
                data->mouse->press_right = false;
            }
        }

        SDL_SetRenderDrawColor(data->renderer,background_color.r,background_color.g,background_color.b,background_color.a);
        SDL_RenderClear(data->renderer);

        if(data->window_menu) menu_run(data);
        
        else if(data->window_project) project_run(data);

        else data->run = false;


        if(data->window_create) create_run(data);
           
        if(data->window_settings) setting_run(data);

        if(data->window_new_tile_set) new_tile_run(data);

        if(data->window_file) file_run(data);


        SDL_RenderPresent(data->renderer);
    }
    SDL_StopTextInput();

    data_free(data);

    TTF_Quit();
    SDL_Quit();
    return 0;
}

