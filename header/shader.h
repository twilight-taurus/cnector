#ifndef CSHADER_HEADER
#define CSHADER_HEADER


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