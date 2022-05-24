#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <globals.h>

#ifndef RAY_LIB
#define GRAPHICS_API_OPENGL_43
#include "raylib.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum InputMode {
    INPUTMODE_CAMERA,
    INPUTMODE_INTERACTION,
    INPUTMODE_CAMERA_AND_INTERACTION,
    INPUTMODE_MENU,
    INPUTMODE_TEXT,
} InputMode;

//# pragma pack (1) // packed struct
typedef struct  InputState {
    bool forward; // 1 byte
    bool backward; // ...
    bool left; // ..
    bool right; // ..

    bool up; // ..
    bool down; // ..
    char _align_keys[2]; // 2 byte
    // sum: 8 bytes (4 byte alignment)

    bool click_left; // 1 b
    bool click_middle; // 1 b
    bool click_right; // 1 b
    char _align_click[1]; // 1 b

    bool wheel_up;
    bool wheel_down;

    float mouse_pos; // 4 b
    enum InputMode mode; // 4 b
    // sum: 12 bytes (4 byte alignment)
    char * text_input;

    // compiler adds padding in 4 bytes chunks (32 bit architecture / ) / 8 bytes (64 bit architecture)
} InputState;

extern InputState * INPUT_STATE;

extern void init_input(void);

extern void deinit_input(void);

extern InputState * get_input_state();

// use extern for any functions called outside this translation unit.
// extern := external linkage: function definition available for other translation units.
// static := internal linkage: function def. -not- available for other translation units.
// if storage class unspecified -> 
// inline definition only visible in current translation unit.
// but identifier still has external linkage, so external definition must be provided outside.

// inline is only a compiler hint -> compiler is free to ignore it.
// -> it instructs compiler to -attempt to- embed function content into the calling code,
// instead of making the actual function call.
extern void switch_input_mode(InputMode mode);

extern void process_input();

// TODO: do not declare these. only define them in the implementation file as inline.
//extern void process_keys();

//extern void process_btns();

//extern void process_mousemove();

//extern void process_chars();
/////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif


#endif