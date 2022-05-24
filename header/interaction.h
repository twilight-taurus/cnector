#ifndef INTERACTION_H
#define INTERACTION_H

#ifndef RAY_LIB
#define GRAPHICS_API_OPENGL_43
#include "raylib.h"
#endif

#include <stdlib.h>

#include <world.h>
#include <camera.h>

#include <bulletC_API.h>

#include <globals.h>

#include <state.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CInteractionController {
//    bool move_down;
//    bool move_up;
//    bool move_forward;
//    bool move_backward;
//    bool move_left;
//    bool move_right;

    bool moved;

    bool rotate;
    bool drag;

} CInteractionController;

extern CInteractionController * INTERACTION_CONTROLLER;

extern void init_interaction();

extern void deinit_interaction();
// TODO: assign object id, lock camera on object.
extern void assign_interaction(CPhysObject * object);
// TODO: unassign object id, unlock camera.
extern void unassign_interaction();

extern void process_interaction();

#ifdef __cplusplus
}
#endif

#endif