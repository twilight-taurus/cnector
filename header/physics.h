#ifndef CPHYSICS_H
#define CPHYSICS_H

#include <globals.h>

#include <world.h>

typedef struct CPhysicsState {
    bool simulation;
} CPhysicsState;

#ifdef __cplusplus
extern "C" {
#endif

extern CPhysicsState * PHYSICS_STATE;

extern void init_physics();

extern void deinit_physics();


// used in threaded process
extern void process_physics();

// used in threaded update
extern void update_physics();

#ifdef __cplusplus
}
#endif

#endif