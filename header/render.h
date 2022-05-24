#ifndef CRENDER_HEADER
#define CRENDER_HEADER

#ifndef RAY_LIB
#define GRAPHICS_API_OPENGL_43
#include <raylib.h>
#endif

#include <state.h>

#include <user_control.h>
#include <interaction.h>
#include <camera.h>

#include <gui.h>

/*

    TODO: use Bullet3 as physics engine instead of ODE (much faster.)

*/

//#include <processthreadsapi.h>
//#include <process.h>
//#include <synchapi.h>


#ifdef __cplusplus
extern "C" {
#endif


extern int init_render();

extern int deinit_render();

// runs inside update() function. to sync results of resource-intensive stuff (like physics to Ui before render() )
// after the originary sync_ui() has completed.
// this way sync_ui can begin while process() function is still calculating physics, etc.
extern void post_sync_ui();

// runs before update. syncs UI to processed world values. (WorldState not mutated here!)
// ( can run multiple threads itself.)
//DWORD WINAPI sync_ui(UiState * ui_state, WorldState * w_state, Camera * camera); // I.

// updates render state from world state, ui and camera. (WorldState, UiState not mutated here!)
//DWORD WINAPI update(RenderState * r_state, WorldState * w_state, UiState * ui_state, Camera * camera); // I.
// (can run multiple threads itself, but unlikely.)

// takes copy to RenderState and UiState (last update function result).
// renders both. sequentially (first world screen, then Ui on top.)
// IMPORTANT: render function needs copy of RenderState 
// before process function modifies WorldState. (structs might share memory.)
// same for UiState! ( it might share memory with WorldState aswell.)

extern void render(); // II.

// takes reference to world state, and mutates it if necessary.
// by running physics, checking inputs, etc. 
//(can run multiple threads itself.)
//DWORD WINAPI process(WorldState * w_state); // II.

#ifdef __cplusplus
}
#endif


#endif

/*
    Concurrency plan:

    -thread_main: ...
    -thread_2: update (from result of process)
    -thread_3: sync_ui (from result of process)
    - join both.
    -thread_main: ...
    - thread2: render (from result of update and sync_ui)
    - thread3 : process (physics, input, etc.)
    -> join both.
    -> repeat.
*/


// ( DEPRECATED PLAN )

//  I.          II.
//  \/          \/
//  render      process 1. (IMPORTANT: render needs copy of some data in memory before process begins to modify it.)
//  |           |
//  |           |
//  sync_ui     |       2. ( sync_ui can start synchronizing data to ui, before process function has finished.)
//  |          /
//  |        /
//  |      /
//  update              3. ( here resulting data from process function is uploaded to GPU)
//  |                       and other resource-intensive (delegated) operations like below.
//  sync_physics_to_ui  3a. (rest of sync'ing is done within update function, for resource-intensive stuff like physics )
//  |
//  \/                  4. repeat.




// ( NEW PLAN )

    //main      //task1              //task2
//  \/          \/                  \/
//  |           process             |
//  render      |                   |
//  |           |                   |
//  |           |                   sync_ui (1. phase sync_ui)
//  |           |                  /
//  |          sync_physics_to_ui   (2.and phase sync ui)
//  |         /
//  |       /
//  update  _ _ _ _ _ _ _ _ _ _ _ _   ( update buffers, timers, game logic, etc.)
//  |     \                         \
//  |      \                        |
//  |       \                       |
//  |        \                      |
//  |         \                     |
//  |         |                     |