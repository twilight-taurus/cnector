#ifndef CCONTROL_HEADER
#define CCONTROL_HEADER

#ifndef RAY_LIB
#define GRAPHICS_API_OPENGL_43
#include "raylib.h"
#endif

#include <stdlib.h>

#include <world.h>
#include <camera.h>

#include <globals.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <bulletC_API.h>

typedef struct CUnitController {
    bool moved;

    bool rotate;
    bool drag;
} CUnitController;

extern void init_control();

extern void deinit_control();

// TODO: assign object id, lock camera on object.
extern void assign_control(CPhysObject * obj);
// TODO: unassign object id, unlock camera.
extern void unassign_control();

extern void process_control();


#ifdef __cplusplus
}
#endif


#endif