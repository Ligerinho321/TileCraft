#ifndef STRUCTS_H_INCLUDE
#define STRUCTS_H_INCLUDE

#include <header.h>

typedef struct{
    TTF_Font *font;
    TTF_Font *fontlower;
    SDL_Color text_color;
    SDL_Color border_color;
    SDL_Color background_color;
    SDL_Color background_color_selected;
    SDL_Color quit_color;
    SDL_Color background_color_map;
    SDL_Color grade_color;
}Settings;

typedef struct{
    int pos_x;
    int pos_y;
    int rel_x;
    int rel_y;
    Bool press_right;
    Bool press_left;
    float pos_x_after;
    float pos_y_after;
    float pos_x_before;
    float pos_y_before;
}Mouse;

typedef struct{
    SDL_Rect rect;
    Bool selected;
    char *text;
}Button;

typedef struct{
    SDL_Rect rect;
    SDL_Rect subRect;
    int mouseDiff;
    Bool selected;
    Bool on_bar;
    int offset;
    int lenght;
    Bool visible;
}ScrollBar;

typedef struct{
    char *text;
    char *buffer;
    SDL_Rect rect;
    Bool selected;
    int time;
    Bool on;
    Bool visible;
    int max_character;
    Tipo type;
}Input;

typedef struct diretorio{
    SDL_Rect rect;
    char name[MAX_STRING];
    char tipo[MAX_STRING];
    Bool selected;
    struct diretorio *next;
}Diretorio;

typedef struct{
    SDL_Rect rect;
    char text[MAX_STRING];
    int time;
}Buffer;

typedef struct{
    char *orientation;
    int width_map;
    int height_map;
    int width_block;
    int height_block;
    int size;
    float scale;
    float transX;
    float transY;
    int current_id;
    int pos_x;
    int pos_y;
    Tipo type;
}Properties;

typedef struct{
    SDL_Rect rect;
    SDL_Rect adicionar;
    SDL_Rect remove;
    SDL_Rect rect1;
    SDL_Rect rect2;
    SDL_Rect rect3;
    Bool adiSelected;
    Bool remSelected;
}Widget;

typedef struct{
    SDL_Rect textRect;
    SDL_Rect rect;
    Button confirmar;
    Button cancelar;
    Bool visible;
}WidgetConfirm;

typedef struct tile{
    char *path;
    SDL_Texture *texture;
    SDL_Rect rect;
    int altura;
    int largura;
    int id_local;
    int id_global;
    Bool selected;
    struct tile *next;
}Tile;

typedef struct tag{
    
    struct tag *previous;
    struct tag *next;

    char *name;
    char *path;
    void *element;
    Button button_quit;
    SDL_Rect rect;
    SDL_Rect textRect;
    SDL_Rect current_rect;
    Bool on;
    Bool selected;
    int mouseDiff;
    Tipo type;
    Bool checked;
}Tag;

typedef struct Tag_Tiles{
    struct Tag_Tiles *next;
    char *name;
    char *path;
    Tile *tiles;
    Tile *tile_selected;
    SDL_Rect rect;
    Tipo type;
    size_t size;
    float scale;
    size_t number_of_columns;
}Tag_Tiles;

typedef struct{
    Tile *tile;
    Tag_Tiles *tag_tile;
    size_t buffer;
    int x;
    int y;
}Bloco;

typedef struct tag_camadas{
    Bloco **matriz;
    Input input_name;
    Bool clicked;
    int time;
    size_t id;
    int width;
    struct tag_camadas *next;
    struct tag_camadas *previous;
}Tag_Camadas;

typedef struct{
    char *name;
    SDL_Rect sigla_rect;

    uint8_t value;
    SDL_Rect value_rect;

    SDL_Point ponto1;
    SDL_Point ponto2;
    SDL_Rect interruptor;

    int mouse_diff;
    Bool on;
}Switch_Uint8_t;

typedef struct{
    Switch_Uint8_t r;
    Switch_Uint8_t g;
    Switch_Uint8_t b;
    SDL_Color *buffer_color;
    SDL_Rect rect;
    SDL_Rect rect_color;
}Switch_Color;

typedef struct{
    size_t start_x;
    size_t start_y;
    size_t end_x;
    size_t end_y;
}Chunck;

typedef struct element{
    size_t x;
    size_t y;
    Tile *tile;
    Tag_Tiles *tag_tile;
    struct element *previous;
}Element;

typedef struct map_undo_redo_action{
    Element *elements;
    Tag_Camadas *tag_camada;
    Tipo type;
    struct map_undo_redo_action *previous;
}Map_Undo_Redo_Action;

typedef struct edit_undo_redo_action{
    char *path_tile;
    Tipo type;
    struct edit_undo_redo_action *previous;
}Edit_Undo_Redo_Action;




typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect settings;
    SDL_Rect rect;
    char *settings_text;
    Button cancel;
    Button apply;
    Button ok;
    Bool open;

    //text_color
    //-----------------------------------
    char *text_color_text;
    SDL_Rect text_color;
    Switch_Color text_color_switch;
    //-----------------------------------
    //background_color
    //-----------------------------------
    char *background_color_text;
    SDL_Rect background_color;
    Switch_Color background_color_switch;
    //-----------------------------------
    //border_color
    char *border_color_text;
    SDL_Rect border_color;
    Switch_Color border_color_switch;
    //---------------------------------------
    //background_color_map
    char *background_color_map_text;
    SDL_Rect background_color_map;
    Switch_Color background_color_map_switch;
    //---------------------------------------
    //selection_color
    char *selection_color_text;
    SDL_Rect selection_color;
    Switch_Color selection_color_switch;
    //--------------------------------------
    //grade_color
    char *grade_color_text;
    SDL_Rect grade_color;
    Switch_Color grade_color_switch;
    //-------------------------------------
    //quit_color
    char *quit_color_text;
    SDL_Rect quit_color;
    Switch_Color quit_color_switch;
    //---------------------------------------
}Window_Settings;

typedef struct{
    Tag_Tiles *tag_tiles;
    Tag_Tiles *tag_selected;

    ScrollBar vertical_scroll_bar_tiles;
    ScrollBar horizontal_scroll_bar_tiles;

    ScrollBar vertical_scroll_bar_tags;
    ScrollBar horizontal_scroll_bar_tags;

    Button edit;
    Button delete;
    Button new_tile_set;
    SDL_Rect rect;
    SDL_Rect tags_rect;
    SDL_Rect tags_rect_reference;
    SDL_Rect tiles_rect;
    SDL_Rect tiles_rect_reference;

    SDL_Rect titulo_rect;
    char *titulo_text;
    int max_width;
}Window_Tiles;

typedef struct{
    Tag_Camadas *tags;
    Tag_Camadas *tag_atual;
    Properties *propriedades;
    SDL_Rect rect;
    SDL_Rect subRect;

    SDL_Rect layers;
    char *layers_text;

    SDL_Rect up;
    SDL_Vertex up_vertices[3];
    Bool up_selected;

    SDL_Rect down;
    SDL_Vertex down_vertices[3];
    Bool down_selected;

    SDL_Rect add;
    SDL_Rect add_rect_1;
    SDL_Rect add_rect_2;
    Bool add_selected;

    SDL_Rect eraser;
    SDL_Rect eraser_rect_1;
    Bool eraser_selected;

    ScrollBar vertScrollBar;
    ScrollBar horiScrollBar;
}Window_Layers;

typedef struct{
    Properties *properties;
    SDL_Rect rect;

    SDL_Rect titulo;
    SDL_Rect orientation;
    SDL_Rect width;
    SDL_Rect height;
    SDL_Rect width_block;
    SDL_Rect height_block;
    SDL_Rect pos_x;
    SDL_Rect pos_y;
    SDL_Rect current_id;
    SDL_Rect scale;

    SDL_Rect orientation_rect;
    SDL_Rect width_rect;
    SDL_Rect height_rect;
    SDL_Rect width_block_rect;
    SDL_Rect height_block_rect;
    SDL_Rect pos_x_rect;
    SDL_Rect pos_y_rect;
    SDL_Rect current_id_rect;
    SDL_Rect scale_rect;

    char *titulo_text;
    char *orientation_text;
    char *width_text;
    char *height_text;
    char *width_block_text;
    char *height_block_text;
    char *pos_x_text;
    char *pos_y_text;
    char *current_id_text;
    char *scale_text;
}Map_Informations;

typedef struct{
    char *path;
    char *path_export;
    Properties *properties;
    Window_Tiles *wTiles;
    Window_Layers *wCamadas;
    Map_Informations *mapInfo;
    Map_Undo_Redo_Action *undo;
    Map_Undo_Redo_Action *redo;
    SDL_Rect rect_reference;
    SDL_Rect map_rect;
    SDL_Rect bar_rect;
    Button picker;
    Button eraser;
    Button fill;
    Button fill_rect;
    Button clear_rect;
    Modo mode;
    SDL_Point shape_buffer[2];
    SDL_Point vertex_buffer[4];
    size_t nChuncks;
    Chunck *chuncks;
    SDL_Texture *cross_hair_true_texture;
    SDL_Texture *cross_hair_false_texture;
    SDL_Point *pilha;
    SDL_Point current_point;
    Bool updated;
    Bool grade_visible;
}Window_Map;

typedef struct{
    SDL_Rect rect;
    Button ok;
    Button cancel;

    SDL_Rect create_map;
    char *create_map_text;

    SDL_Rect orientation;
    char *orientation_text;
    
    SDL_Rect selection_orientation;
    Bool selection_orientation_selected;
    Tipo current_orientation;
    Bool is_selection;

    SDL_Rect vertex_rect;
    SDL_Vertex vertex[3];

    SDL_Rect isometric_rect;
    Bool isometric_selected;
    char *isometric_text;

    SDL_Rect orthogonal_rect;
    Bool orthogonal_selected;
    char *orthogonal_text;


    SDL_Rect map_size;
    char *map_size_text;

    SDL_Rect block_size;
    char *block_size_text;

    SDL_Rect map_width;
    char *map_width_text;

    SDL_Rect map_height;
    char *map_height_text;

    SDL_Rect block_width;
    char *block_width_text;

    SDL_Rect block_height;
    char *block_height_text;

    Input width_map_input;
    Input height_map_input;
    Input width_block_input;
    Input height_block_input;
    Bool open;
}Window_Create;

typedef struct{
    SDL_Rect rect;

    SDL_Rect titulo;
    char *titulo_text;

    SDL_Rect id;
    char *id_text;
    SDL_Rect id_rect;

    SDL_Rect altura;
    char *altura_text;
    SDL_Rect altura_rect;

    SDL_Rect largura;
    char *largura_text;
    SDL_Rect largura_rect;

    SDL_Rect name;
    char *name_text;
    SDL_Rect name_rect;

    SDL_Rect size;
    char *size_text;
    SDL_Rect size_rect;
}Edit_Informations;

typedef struct{
    char *path;
    Edit_Informations *editInfo;
    Tile *tiles;
    Tile *tile_selected;
    Edit_Undo_Redo_Action *undo;
    Edit_Undo_Redo_Action *redo;
    SDL_Rect rect_reference;
    SDL_Rect blocos_rect;
    Widget widget;
    WidgetConfirm wConfirm;
    ScrollBar vertical_scroll_bar;
    ScrollBar horizontal_scroll_bar;
    float scale;
    size_t size;
    Tipo type;
    size_t number_of_columns;
    Bool updated;
    int max_width;
}Window_Edit;

typedef struct{
    char *current_path;
    char *titulo_text;
    char *path_text;
    char *name_text;
    Diretorio *diretorios;
    Window_Edit *window_edit;
    Input inputPath;
    Input inputFile;
    SDL_Rect rect;
    SDL_Rect subRect;
    ScrollBar verticalScrollBar;
    Button confirmar;
    Button cancel;
    Button back;
    Button search;
    SDL_Rect titulo_rect;
    SDL_Rect path_rect;
    SDL_Rect name_rect;
    Buffer buffer;
    Modo modo;
    Bool open;
}Window_File;

typedef struct{
    Tag *tags;
    Tag *tag_atual;
    char *not_save_text;
    char *saved_text;
    SDL_Rect tag_rect;
    SDL_Rect bar_rect;
    SDL_Rect rect;
    SDL_Rect rect_item;
    Button export;
    Button save;
    Button open_file;
    Button new_tile_set;
    Button new_map;
    Button close;
    Button quit;
    Bool open;

    Bool closing;
    Bool exiting;

    SDL_Rect modal;
    SDL_Rect modal_text_rect;
    char *modal_text;
    Button modal_save;
    Button modal_discard;
    Button modal_cancel;
    Bool modal_visible;

}Window_Project;

typedef struct{
    SDL_Rect rect;
    SDL_Rect tile_editor;
    char *tile_editor_text;
    SDL_Rect version_rect;

    SDL_Rect f11;
    char *f11_text;
    
    SDL_Rect esc;
    char *esc_text;

    SDL_Rect ctrl_z;
    char *ctrl_z_text;

    SDL_Rect ctrl_y;
    char *ctrl_y_text;

    SDL_Rect ctrl_r;
    char *ctrl_r_text;

    SDL_Rect ctrl_g;
    char *ctrl_g_text;

    Button open_file;
    Button quit;
    Button settings;
    Button new_map;
    Button new_tile_set;
    Bool open;
}Window_Menu;

typedef struct{
    SDL_Rect rect;

    char *tile_set_text;
    SDL_Rect tile_set_rect;

    char *name_text;
    SDL_Rect name_rect;
    Input name_input;

    char *type_text;
    SDL_Rect type_rect;
    SDL_Rect type_selection;
    Bool type_selection_selected;
    Bool is_selected;
    Tipo current_type;

    SDL_Rect vertex_rect;
    SDL_Vertex vertex[3];

    char *multiple_images_text;
    SDL_Rect multiple_images_rect;
    Bool multiple_images_selected;

    char *an_image_text;
    SDL_Rect an_image_rect;
    Bool an_image_selected;

    char *image_text;
    SDL_Rect image_rect;

    char *path_text;
    SDL_Rect path_rect;
    Input path_input;
    Button search;

    char *tile_width_text;
    SDL_Rect tile_width_rect;
    Input tile_width_input;

    char *tile_height_text;
    SDL_Rect tile_height_rect;
    Input tile_height_input;

    char *margin_text;
    SDL_Rect margin_rect;
    Input margin_input;

    char *spacing_text;
    SDL_Rect spacing_rect;
    Input spacing_input;

    Button save;
    Button cancel;
    Buffer buffer;
    Bool open;
}Window_New_Tile_Set;

typedef struct{
    Window_Settings *window_settings;
    Window_Project *window_project;
    Window_File *window_file;
    Window_Create *window_create;
    Window_Menu *window_menu;
    Window_New_Tile_Set *window_new_tile_set;
    Settings *setting;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Mouse *mouse;
    SDL_Event *event;

    int screen_w;
    int screen_h;
    
    Bool run;
    Bool update_rects;
}Data;



#endif