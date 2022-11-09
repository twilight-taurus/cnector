// holding all the renderable objects, 
// or other objects related to scene/game logic.

#ifndef CNECTOR_H
#define CNECTOR_H


#include <GL/glew.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "render.h"
#include "shader.h"

#if defined(_WIN32)
    #include "threading_win32.h"
#else // add condition for linux, macos, etc.
    #include "threading.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


extern void CNInitialize();

extern void CNDeinitialize();

extern void CNRun();


#ifdef __cplusplus
}
#endif


#endif