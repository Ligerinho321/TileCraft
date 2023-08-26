#ifndef HEADER_H_INCLUDE
#define HEADER_H_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/types.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#define VERSION "v1.0.0"

#define MAX_STRING 260
#define MAX_STRING_FILE 100024
#define MAX_DIGIT 4
#define COLLDOWN_INPUT 600
#define COLLDOWN_BUFFER 4000
#define COLLDOWN_DOUBLE_CLICK 250
#define FONT_SIZE 14
#define FONT_LOWER_SIZE 11
#define SPACING 10
#define SCROLL_BAR_SPACING 2
#define CHUNCK_WIDTH 50
#define CHUNCK_HEIGHT 50

typedef enum{
    TYPE_VERTICAL,
    TYPE_HORIZONTAL,
    TYPE_ISOMETRIC,
    TYPE_ORTHOGONAL,
    TYPE_UP,
    TYPE_DOWN,
    TYPE_LOCAL,
    TYPE_GLOBAL,
    TYPE_NUMBER,
    TYPE_TEXT,
    TYPE_APPEND,
    TYPE_REMOVE,
    TYPE_ERASER,
    TYPE_ADD,
    TYPE_UNDO,
    TYPE_REDO,
    TYPE_AN_IMAGE,
    TYPE_MULTIPLE_IMAGES,
    TYPE_MAP,
    TYPE_TILE_SET
}Tipo;

typedef enum{
    MOD_SELECTION_FILE,
    MOD_SELECTION_IMG_TILE,
    MOD_SELECTION_IMG_TILE_SET,
    MOD_SELECTION_FONT,
    MOD_SAVE_TILE_SET,
    MOD_SAVE_MAP,
    MOD_EXPORT,
    

    MOD_PICKER,
    MOD_ERASER,
    MOD_FILL,
    MOD_FILL_RECT,
    MOD_CLEAR_RECT
}Modo;

typedef enum{
    false,
    true,
}Bool;

#endif