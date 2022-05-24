// physical objects in the world, that are subject to the physics simulation.

#ifndef CPHYSICS_HEADER
#define CPHYSICS_HEADER

#ifdef __cplusplus
extern "C" {
#endif

// if defined, initialization of cobjects and descendants
// limited with functions provided in world.h
#ifdef USE_COBJECTS_API
// ...
#endif

#include <bulletC_API.h>

#include <object.h> // base object type

#include <assets.h> // containers and functions for dealing with assets.

#include <cglm/cglm.h>
#include <cglm/call.h>

typedef enum CPhysObjectType {
    CPHYS_NONE, // not determined.
    CPHYS_CHARACTER,
    CPHYS_VEHICLE,
    CPHYS_STRUCTURE,
    CPHYS_TERRAIN,
    CPHYS_PARTICLES,
    CPHYS_SPHERE,
    CPHYS_CUBE,
    CPHYS_PLANE
} CPhysObjectType;

typedef struct CPhysCfg {
    b3CollisionShape        * collision_shape;
    b3CollisionShapeType    shape_type; // corresponds to BroadphaseNativeTypes
    float                   mass;
    float                   inertia;
} CPhysCfg;

// base configuration struct for each physical object.
typedef struct CPhysBaseInfo {

    //// base attributes for CPhysObject ////
    CPhysObjectType         physics_type; // used to determine what object config entry to read from or write to.
    CPhysCfg                phys_config; // base configuration options for each physical object
    CGLM_ALIGN_MAT mat4     world_transform; // physics object world transform.

    // extended attributes ...

} CPhysBaseInfo;

// specific object config types (with inheritance from CPhysObjectCfg) START //
typedef struct CCharacterInfo {
    
    //// base attributes for CPhysObject ////
    CPhysObjectType         physics_type; // used to determine what object config entry to read from or write to.
    CPhysCfg                phys_config; // base configuration options for each physical object
    CGLM_ALIGN_MAT mat4     world_transform; // physics object world transform.

    // extended attributes for models
    CAsset_ID               asset_id;
    CModelCfg               model_config; // 4 bytes
    // added configuration options here.
} CCharacterInfo;

typedef struct CVehicleInfo {

    //// base attributes for CPhysObject ////
    CPhysObjectType         physics_type; // used to determine what object config entry to read from or write to.
    CPhysCfg                phys_config; // base configuration options for each physical object
    CGLM_ALIGN_MAT mat4     world_transform; // physics object world transform.

    // extended attributes for models
    CAsset_ID               asset_id;
    CModelCfg               model_config; // 4 bytes
    // added configuration options here.
} CVehicleInfo;

typedef struct CStructureInfo {
    
    //// base attributes for CPhysObject ////
    CPhysObjectType         physics_type; // used to determine what object config entry to read from or write to.
    CPhysCfg                phys_config; // base configuration options for each physical object
    CGLM_ALIGN_MAT mat4     world_transform; // physics object world transform.

    // extended attributes for models
    CAsset_ID               asset_id;
    CModelCfg               model_config; // 4 bytes
    // added configuration options here.
    // ...

} CStructureInfo;

typedef struct CTerrainInfo {

    //// base attributes for CPhysObject ////
    CPhysObjectType         physics_type; // used to determine what object config entry to read from or write to.
    CPhysCfg                phys_config; // base configuration options for each physical object
    CGLM_ALIGN_MAT mat4     world_transform; // physics object world transform.

    // extended attributes for models
    CAsset_ID               asset_id;
    CModelCfg               model_config; // 4 bytes
    // added configuration options here.
} CTerrainInfo;

typedef struct CSphereInfo {
    //// base attributes for CPhysObject ////
    CPhysObjectType         physics_type; // used to determine what object config entry to read from or write to.
    CPhysCfg                phys_config; // base configuration options for each physical object
    CGLM_ALIGN_MAT mat4     world_transform; // physics object world transform.

    // extended attributes for spheres
    bool                    isAsset;
    CAsset_ID               asset_id; // only valid if isAsset is true

    float                   radius;
    int                     rings;
    int                     slices;
} CSphereInfo;

typedef struct CCubeInfo {
    //// base attributes for CPhysObject ////
    CPhysObjectType         physics_type; // used to determine what object config entry to read from or write to.
    CPhysCfg                phys_config; // base configuration options for each physical object
    CGLM_ALIGN_MAT mat4     world_transform; // physics object world transform.

    // extended attributes for spheres
    bool                    isAsset;
    CAsset_ID               asset_id; // only valid if isAsset is true

    float   width;
    float   height;
    float   length;
} CCubeInfo;

typedef struct CPlaneInfo {
    //// base attributes for CPhysObject ////
    CPhysObjectType         physics_type; // used to determine what object config entry to read from or write to.
    CPhysCfg                phys_config; // base configuration options for each physical object
    CGLM_ALIGN_MAT mat4     world_transform; // physics object world transform.

    // extended attributes for spheres
    bool                    isAsset;
    CAsset_ID               asset_id; // only valid if isAsset is true

    float   width;
    float   length;
    int     resX;
    int     resZ;
} CPlaneInfo;

// union object allowing us to select config based on the actual physics object type.
typedef union CPhysObjectInfo {
    // base config. inherited by all descending config types.
    CPhysBaseInfo       base_info;
    // extended configs for models
    CCharacterInfo      character_info;
    CVehicleInfo        vehicle_info;
    CStructureInfo      structure_info;
    CTerrainInfo        terrain_info;

    // configs for shapes
    CSphereInfo         sphere_info;
    CCubeInfo           cube_info;
    CPlaneInfo          plane_info;

} CPhysObjectInfo;

// specific object config types (no inheritance from CPhysObjectCfg) END //

// base type for objects that are (potential) subject to physics. 
// ( representable as physical objects)

typedef struct CPhysObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject:
    CObjectType         object_type; // 4 bytes ; (4 bits)  determines which type to cast to/access via union

    // config object union
    CPhysObjectInfo     info;

    //// extended attributes for CPhysObject //////
    b3RigidBody         * physics_body; // forward declared type. unsure if this will work safely.


////////////////////////////////     CPhysObject end      ///////////////////////////////////////
    // hopefully sufficient padding for casting objects to base type and back.
    // TODO: find way to determine max possible object size (compile or runtime)
    // -> and then allocate accordingly
    // --> solution: use a union!!!
//    char              _padding[128]; // 128 bytes of padding, to accodomate physical object types to cast to and from.
} CPhysObject;


typedef struct CCharacterObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject:
    CObjectType         object_type; // 4 bytes ; (4 bits)  determines which type to cast to/access via union

    // config object union
    CPhysObjectInfo     info;

    //// extended attributes for CPhysObject //////
    b3RigidBody         * physics_body; // forward declared type. unsure if this will work safely.

    RayModel            * model;
////////////////////////////////     CPhysObject end      ///////////////////////////////////////
    //    char              _padding[128]; // 128 bytes of padding, to accodomate physical object types to cast to and from.
} CCharacterObject;


typedef struct CVehicleObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject:
    CObjectType         object_type; // 4 bytes ; (4 bits)  determines which type to cast to/access via union

    // config object union
    CPhysObjectInfo     info;

    //// extended attributes for CPhysObject //////
    b3RigidBody         * physics_body; // forward declared type. unsure if this will work safely.

    RayModel            * model;
////////////////////////////////     CPhysObject end      ///////////////////////////////////////
} CVehicleObject;


// can be a sphere, wall, house, etc. static or dynamic.
// object structure can change through physical forces.

typedef struct CStructureObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject:
    CObjectType         object_type; // 4 bytes ; (4 bits)  determines which type to cast to/access via union

    // config object union
    CPhysObjectInfo     info;

    //// extended attributes for CPhysObject //////
    b3RigidBody         * physics_body; // forward declared type. unsure if this will work safely.

    RayModel            * model;
////////////////////////////////     CPhysObject end      ///////////////////////////////////////
} CStructureObject;

// terrain object. work in progress. static.
// object structure can change through physical forces.

typedef struct CTerrainObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject:
    CObjectType         object_type; // 4 bytes ; (4 bits)  determines which type to cast to/access via union

    // config object union
    CPhysObjectInfo     info;

    //// extended attributes for CPhysObject //////
    b3RigidBody         * physics_body; // forward declared type. unsure if this will work safely.

    RayModel            * model;
////////////////////////////////     CPhysObject end      ///////////////////////////////////////
} CTerrainObject;

typedef struct CSphereObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject:
    CObjectType         object_type; // 4 bytes ; (4 bits)  determines which type to cast to/access via union

    // config object union
    CPhysObjectInfo     info;

    //// extended attributes for CPhysObject //////
    b3RigidBody         * physics_body; // forward declared type. unsure if this will work safely.

    RayModel            * model;
////////////////////////////////     CPhysObject end      ///////////////////////////////////////
} CSphereObject;

typedef struct CCubeObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject:
    CObjectType         object_type; // 4 bytes ; (4 bits)  determines which type to cast to/access via union

    // config object union
    CPhysObjectInfo     info;
    //// extended attributes for CPhysObject //////
    b3RigidBody         * physics_body; // forward declared type. unsure if this will work safely.

    RayModel            * model;
////////////////////////////////     CPhysObject end      ///////////////////////////////////////
} CCubeObject;

typedef struct CPlaneObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject:
    CObjectType         object_type; // 4 bytes ; (4 bits)  determines which type to cast to/access via union

    // config object union
    CPhysObjectInfo     info;

    //// extended attributes for CPhysObject //////
    b3RigidBody         * physics_body; // forward declared type. unsure if this will work safely.

    RayModel            * model;
////////////////////////////////     CPhysObject end      ///////////////////////////////////////
} CPlaneObject;

typedef union CPhysItem {
    CPhysObject         physical; // base for physical objects

    CCharacterObject    character;
    CVehicleObject      vehicle;
    CStructureObject    structure;
    CTerrainObject      terrain;

    CSphereObject       sphere;
    CCubeObject         cube;
    CPlaneObject        plane;
} CPhysItem;

// terrain object. not subject to physics
typedef struct CStaticTerrainObject CStaticTerrainObject;
// terrain object. subject to physics or player interaction.
typedef struct CDynamicTerrainObject CDynamicTerrainObject;

// define vector container type that will hold initialized physical objects.
// like the other containers, the index of the items is not their object or model id.
// they items are simply iterated over, and conditionally added to the world/scene.
/*
extern void CPhysItem_del(CPhysItem * item);

extern int CPhysItem_compare(const CPhysItem * item1, const CPhysItem * item2);

#define i_val       CPhysItem       // value: REQUIRED
#define i_cmp       CPhysItem_compare// three-way compare two i_valraw* : REQUIRED IF i_valraw is a non-integral type
#define i_del       CPhysItem_del // destroy value func - defaults to empty destruct
//#define i_valraw    // convertion "raw" type - defaults to i_val
//#define i_valfrom   // convertion func i_valraw => i_val - defaults to plain copy
//#define i_valto     // convertion func i_val* => i_valraw - defaults to plain copy
#define i_tag       cphysitem// defaults to i_val
#include <stc/cvec.h>

// array to store initialized physical objects.
// they are however not yet a part of the world/scene. they are added to the world/scene through CVEC_COBJECT (world.h)
extern cvec_cphysitem CVEC_CPHYSITEM;
*/
// initializes objects. the add functions then 'create' it in the world.
// use the CPhysObjectCfg to set the physics parameters on RigidBody creation.
extern CPhysObject          CPhysObject_init(const char * file_name);

extern CVehicleObject       CPhysObject_initCVehicle(const char * file_name);

extern CCharacterObject     CPhysObject_initCCharacter(const char * file_name);

/////////////////////////////////////////////////////////////////////////////////
// TODO: create a sphere, or plane with initCStructure as a CStructureObject
// assign it an ID, and object type.
// --> then check, if the same values are retrieved, if we access it as CObject.
/////////////////////////////////////////////////////////////////////////////////

extern CStructureObject     CPhysObject_initCStructure(const char * file_name);

extern void                 CPhysObject_deinit(CPhysObject * cphysobject);

extern void                 CPhysObject_deinitCVehicle(CVehicleObject * vehobject);

extern void                 CPhysObject_deinitCCharacter(CCharacterObject * charobject);

extern void                 CPhysObject_deinitCStructure(CStructureObject * structureobject);


#ifdef __cplusplus
}
#endif


#endif