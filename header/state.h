#ifndef CSTATE_HEADER
#define CSTATE_HEADER

#include <bulletC_API.h>
#include <world.h>

#include <math.h>

#ifndef RAY_LIB
#define GRAPHICS_API_OPENGL_43
#include <raylib.h>
#endif

/*
///////// for Cimgui. ////////////////
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
/////////////////////////////////////
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <cglm/cglm.h>
#include <cglm/call.h>

#include <globals.h>

// nuklear gui (no implementation)
#define NK_PRIVATE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_glfw_gl4.h>

// rlights raylib example (no implementation)
#include "rlights.h"

typedef struct CWindowState {
    // try to allocate memory on heap on initialization here. (200 units)
    char*             WINDOW_TITLE;
    unsigned int      WINDOW_TITLE_LENGTH;
} CWindowState;

typedef struct CShaderState {
    // TODO: try to arrange Shaders in an array/vector
    // use a gui to list them by their name.
    Shader      shader_light;
    Shader      shader_phong;
    
    Light       lights[MAX_LIGHTS];

    int         viewPosLoc;

    // phong shader values
    int         lightPosLoc;
    int         lightColLoc;
    int         colDiffLoc;
} CShaderState;

typedef struct CThreadingState {
    void * data;
} CThreadingState;

typedef struct CEventState {
    void * data;
} CEventState;

typedef struct CGuiState {
    struct nk_context           *ctx;
    struct nk_colorf            bg_color;
    struct nk_font_atlas        *font_atlas;
    struct nk_font              *default_font;
    struct nk_font_config       font_cfg;
} CGuiState;



// what is supposed to be rendered to the screen. Aka. final world view projection. (World + UI)
typedef struct CRenderState {
    Texture2D   render_texture;
    CModel      terrain_model;
    Image       height_field;
    Shader      shader_light;
    void *      renderables;

    int         SCREEN_WIDTH;
    int         SCREEN_HEIGHT;

    int         VIEW_WIDTH;
    int         VIEW_HEIGHT;

    int         VIEW_XPOS;
    int         VIEW_YPOS;

} CRenderState;

typedef enum CControlMode {
    CONTROLMODE_TRAVEL, // normal mode
    CONTROLMODE_MENU, // menu mode
    CONTROLMODE_MAP, // map mode
    CONTROLMODE_SPECTATOR, // spectator mode
    CONTROLMODE_GHOST, // -> go through physical bounds. no gravity, etc. (advanced travel/debug mode)
} CControlMode;

// control state -> state of the object that represents the player/user entity.
typedef struct CControlState {
    CObject_ID          object_id; // ID of current user controlled object.
    CControlMode         current_mode;
    CPhysObject         * control_object;
} CControlState;

typedef enum InteractionMode {
    INTERACTIONMODE_NONE,
    INTERACTIONMODE_MOVE,
    INTERACTIONMODE_ROTATE
} InteractionMode;

// interaction state-> state of obj that user interacts with, but is not a player/user entity.
typedef struct CInteractionState {
    CObject_ID           object_id; // ID of current interaction object.
    InteractionMode     current_mode;
    int                 bodies_affected;
    CPhysObject         * interaction_object;
} CInteractionState;

// TODO: switch case between CameraModes when processing/updating camera
typedef enum CCameraMode {
    CAMERAMODE_FREE,
    CAMERAMODE_THIRDPERSON,
    CAMERAMODE_FIRSTPERSON
} CCameraMode;

typedef struct CCameraState {
    Camera camera;

    CCameraMode mode;

    vec3 position;
    vec3 target;
    vec3 up_vector;
    vec3 right;

    vec3 direction;
    // enforced distance between camera eye and target.
    float distance;

    mat4 view;

    // mouse related
    float pitch;
    float yaw;

    float lastX;
    float lastY;

} CCameraState;

//////////////////////////////////////////
// State types declaration.

extern CWindowState * WINDOW_STATE;

extern CCameraState * CAMERA_STATE;

extern CControlState * CONTROL_STATE;

extern CInteractionState * INTERACTION_STATE;

extern CShaderState * SHADER_STATE;

extern CRenderState * RENDER_STATE;

extern CGuiState * GUI_STATE;

//////////////////////////////////////////

extern void init_state();

extern void deinit_state();

#ifdef __cplusplus
}
#endif

#endif