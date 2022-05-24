#ifndef CSHADER_HEADER
#define CSHADER_HEADER

#include <stdio.h>
#include <stdlib.h>


#ifndef RAY_LIB
#define GRAPHICS_API_OPENGL_43
#include <raylib.h>
#endif

#include "state.h"


#ifdef __cplusplus
extern "C" {
#endif

extern void init_shader();

extern void process_shader();

extern void update_shader();

extern void deinit_shader();


#ifdef __cplusplus
}
#endif


#endif