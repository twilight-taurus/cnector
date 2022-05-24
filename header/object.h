#ifndef COBJECT_HEADER
#define COBJECT_HEADER

// if defined, initialization of cobjects and descendants
// limited with functions provided in world.h
#ifdef USE_COBJECTS_API
//...
#endif

#include <globals.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef size_t CObject_ID; // 8 bytes

typedef enum CObjectType {
    OBJECT_TYPE_VISUAL, // renderable 2D/3D objects, not subject to physics simulation
    OBJECT_TYPE_PHYSICAL, // renderable objects, subject to physics simulation
    OBJECT_TYPE_VIRTUAL, // non-renderable objects (game logic fx.)
    OBJECT_TYPE_NONE // undefined object type.
} CObjectType;

// base struct of instantiable objects in the rendered world/scene.
// we use this as the base type in a union.

typedef struct CBaseObject {
    // object_id provides an interface between renderer, physics engine, etc.
    // -- it is defined as the index of CObject array.
    CObject_ID      object_id;
    CObjectType     object_type; // 4 bytes originally : try to use 4 bits max. ( 2 power to 4  -1 = 15 (max value))
    // add padding so down/upcasting can be safely done in contiguous memory.
//    char            _padding[128]; // 128 bytes // -> use unions instead, because this here is undefined behavior.
} CBaseObject;


#ifdef __cplusplus
}
#endif


#endif

// --short-enums : GCC compiler option 