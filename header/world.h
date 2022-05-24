// holding all the renderable objects, 
// or other objects related to scene/game logic.

#ifndef CWORLD_H
#define CWORLD_H

//#include <stc/forward.h>

#include <virtual.h> // virtual world components (game logic: rules, constraints)
#include <physical.h> // physical world components (...)
#include <visual.h> // visual world components (skybox, UI, name tags, etc)

#include <shader.h>



#ifdef __cplusplus
extern "C" {
#endif


// union allows us to safely switch between types when accessing a container, array or any other sort of memory location.
// -- the same memory location can store multiple types of data
// -- memory will be large enough to accomodate the largest member of the union
// --> always use a union, when a type needs to be cast up or down otherwise we get undefined behaviour. 
// (in a union compiler must ensure that all types are aligned the same way.)

typedef union CObject {
    // the base object type -> use this one to safely check the type of object,
    // when accessing the extended object types.
    CBaseObject         base;

    // broad object types
    CPhysObject         physical; // the base physical object type
    CVirtualObject      virtual; // the base virtual object type.
    CVisualObject       visual; // the base visual object type.

    // extended physical object types.
    CCharacterObject    character;
    CVehicleObject      vehicle;
    CStructureObject    structure; 
    CTerrainObject      terrain;

    // shape objects
    CSphereObject       sphere;
    CCubeObject         cube;
    CPlaneObject        plane;

    // extended visual object types
    CGuiObject          CGuiObject;
    CScene2dObject      CScene2dObject;
    CScene3dObject      CScene3dObject;

    // extended virtual object types.
    // ...

} CObject;

// HINT: we dont have to create CObjectItem for every object.
// ---> it only needs to be defined as the value in the cvec container !

//#define CObjectItem

// callback: deletes values/references of an item in the container
// -- is called when item is removed from the container.
// -> deal with destructing attributes of CObject here.
// HINT: delete and remove physics rigid body primarily here.
void CObject_del(CObject * cobj);

// swap objects. (selected with the last one)
// -> so the selected one can be removed cheaply
//                swapped element  last element (before swap)
void CObject_swap(const CObject * cobj1, const CObject * cobj2);

int CObject_compare(const CObject * cobj1, const CObject * cobj2);

// called externally if we want to remove an object from the world.
extern void CObject_remove(const CObject * cobj);

extern void CObject_removeById(const CObject_ID object_id);

extern void CObject_removePhysical(const CPhysObject * obj);

extern void CObject_removeVirtual(const CVirtualObject * obj);

extern void CObject_removeVisual(const CVisualObject * obj);

// add object. -> only "extended" CObjects allowed.
// -- create game objects from this function.
// -- hint: pass reference to a created CPhys/CLogicObject
extern unsigned int CObject_addPhysicalModel(CModel * cmodel);

extern unsigned int CObject_addPhysicalFromCInfo(CPhysObjectInfo * in_desc);

extern unsigned int CObject_addVirtual(CVirtualObject * cobj);

extern unsigned int CObject_addVisual(CVisualObject * cobj);

// physical objects
extern unsigned int CObject_addPhysicalSphere(float radius, int rings, int slices);

extern unsigned int CObject_addPhysicalSphereAsset(CSphere * sphere, vec3 translation);


extern unsigned int CObject_addPhysicalCube(float width, float height, float length);

extern unsigned int CObject_addPhysicalCubeAsset(CCube * cube);


extern unsigned int CObject_addPhysicalPlane(float width, float length, int resX, int resZ);

extern unsigned int CObject_addPhysicalPlaneAsset(CPlane * plane);



extern unsigned int CObject_addBase(const CBaseObject * cobj);


// physical object description initializers.
extern void CPhysObjectInfo_initBaseDefault(CPhysObjectInfo * out_desc);

extern void CPhysObjectInfo_initFromCModelDefault(const CModel * in_cmodel, CPhysObjectInfo * out_desc);

extern void CPhysObjectInfo_initFromCShapeDefault(const CShape * in_cshape, CPhysObjectInfo * out_desc);

// use function when adding Physical Objects, to simplify the 
// creation of Rigid Bodies.
extern void CPhysObject_createRigidBody(CObject * object);



//void CSphereObject_createRigidBody(CSphereObject * object);

//void CCubeObject_createRigidBody(CCubeObject * object);

//void CPlaneObject_createRigidBody(CPlaneObject * object)


/*
    TODO: CObject get, set
*/

// define vector container type that will hold CObjects
#define i_val       CObject       // value: REQUIRED
#define i_cmp       CObject_compare   // three-way compare two i_valraw* : REQUIRED IF i_valraw is a non-integral type
#define i_del       CObject_del // destroy value func - defaults to empty destruct

//#define i_valraw    // convertion "raw" type - defaults to i_val
//#define i_valfrom   // convertion func i_valraw => i_val - defaults to plain copy
//#define i_valto     // convertion func i_val* => i_valraw - defaults to plain copy
#define i_tag       cobject// defaults to i_val
#include <stc/cvec.h>

// world objects, world logic, etc
typedef struct CWorldState {
    cvec_cobject    cobjects;
    bool            physics_sim;
} CWorldState;


extern CWorldState * WORLD_STATE;

//extern cvec_CObject CVEC_CObject;

// initialize containers here.
// possibly also physics engine.
extern void init_world();

// ...
extern void deinit_world();


#ifdef __cplusplus
}
#endif


#endif