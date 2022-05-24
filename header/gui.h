#ifndef CGUI_HEADER
#define CGUI_HEADER

#include "globals.h"

#ifndef RAY_LIB
#define GRAPHICS_API_OPENGL_43
#include "raylib.h"
#endif

#include <state.h>

#include <world.h>
#include <physics.h>

/*

    Create a vector, of which the indices are the IDs identifying a specific widget,
    --> the vector will hold custom widget objects describing the widget properties.

    -- the IDs (vector indices) can be stored somewhere else.

*/

#ifdef __cplusplus
extern "C" {
#endif

extern void init_gui(GLFWwindow * window);

extern void deinit_gui();

extern void process_gui(); // glfw3_new_frame goes in here. change data state. (during render)

extern void update_gui(); // send draw ui commands with processed or old state (after/before render)

extern void draw_gui(); // where the drawing happens. (before buffer swap (not here) )

#ifdef __cplusplus
}
#endif

#endif