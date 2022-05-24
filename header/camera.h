#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <string.h>

#ifndef RAY_LIB
#define GRAPHICS_API_OPENGL_43
#include "raylib.h"
#endif

#include "input.h"

#include <cglm/cglm.h>
#include <cglm/call.h>

#include <globals.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <state.h>

#include <interaction.h>

typedef struct CameraController {
    bool    move_down;
    bool    move_up;
    bool    move_forward;
    bool    move_backward;
    bool    move_left;
    bool    move_right;

    void * data;
} CameraController;

extern CameraController * CAMERA_CONTROLLER;

extern int init_camera();

extern int deinit_camera();

// call in update function (c_state is updated, then communicated to GPU.)
extern void update_camera();
// call in process function (camera state, cc are written to. i_state is read from.)
extern void process_camera();

extern CCameraState * get_camera_state();

extern void set_camera_state(CCameraState * c_state);

extern void set_camera(Camera camera);

extern CameraController * get_cc();

#ifdef __cplusplus
}
#endif


#endif