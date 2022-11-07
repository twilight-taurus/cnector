#ifndef CTHREADING_HEADER
#define CTHREADING_HEADER

#include <stc/threads.h>

#include "globals.h"

#include <user_control.h>
#include <interaction.h>
#include <camera.h>

#include <shader.h>

//#include <gui.h> 

#include <physics.h>


#ifdef __cplusplus
extern "C" {
#endif


extern mtx_t thrdMutex;

// conditions for managing concurrent operations
extern cnd_t cndRendering;

extern cnd_t cndSyncing;


// thread ready conditions.
extern cnd_t cndUpdated;

extern cnd_t cndSynced;

extern cdn_t cndProcessed;


// threads
extern thrd_t thrd_update;

extern thrd_t thrd_sync;

extern thrd_t thrds_process;

// ....


#ifdef __cplusplus
}
#endif


#endif