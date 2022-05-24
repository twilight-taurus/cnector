// objects are not rendered (and therefore not subject)

#ifndef CVIRTUAL_H
#define CVIRTUAL_H

#include <object.h>

// if defined, initialization of cobjects and descendants
// limited with functions provided in world.h
#ifdef USE_COBJECTS_API
//...
#endif

typedef enum CVirtualType {
    CVIRTUAL_USER, // data related to users.
    CVIRTUAL_WORLD, // data related to world status. (not necessarily scene)
    CVIRTUAL_SCENE, // current scene related data.
    CVIRTUAL_GRAPH // graph data (relationships, connections)
} CVirtualType;

// virtual objects
// they define components of game logic.
// they are not physics or otherwise renderable objects.
typedef struct CVirtualCfg {
    int * data;
    CVirtualType virt_type;
} CVirtualCfg;

typedef struct CVirtBaseInfo {
    int * data;
} CVirtBaseInfo;

typedef union CVirtObjectInfo {
    CVirtBaseInfo base_info;
} CVirtObjectInfo;

// object type that is not subject to physicsl simulation (intangible to world)

typedef struct CVirtualObject {
    // object_id provides an interface between renderer, game logic, physics engine, etc.
    // -- it is defined as the index of CObject array.
    CObject_ID object_id; // 4 bytes
    CObjectType  object_type; // 4 bytes (needed with CObject for up/downcasting.)

    ////////// added data //////////
    CVirtObjectInfo object_info; // 4 bytes
} CVirtualObject;

extern CVirtualObject CVirtualObject_init(CVirtualCfg cfg);

extern void CVirtualObject_deinit(CVirtualObject * cvobj);

#endif