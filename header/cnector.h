// holding all the renderable objects, 
// or other objects related to scene/game logic.

#ifndef CNECTOR_H
#define CNECTOR_H

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

//#define dSINGLE // single: float precision ; dDOUBLE: double precision (we have single version library linked.)
//#include <ode.h>

#ifndef RAY_LIB
#define GRAPHICS_API_OPENGL_43
#include "raylib.h"
#endif

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "render.h"
#include "shader.h"

#include "threading_win32.h"

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