#ifndef CMODEL_H
#define CMODEL_H

#include <globals.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

// header-only
#include <asset.h>

#include <raylib_guard.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef Mesh RayMesh;

typedef Model RayModel;

typedef size_t                  CModel_ID;

typedef enum CModelType {
    CMODEL_NONE, // not determined.
    CMODEL_CHARACTER,
    CMODEL_VEHICLE,
    CMODEL_STRUCTURE,
    CMODEL_TERRAIN,
    CMODEL_PARTICLES,
    CMODEL_SPHERE
} CModelType;


typedef struct CMesh {
    RayMesh mesh;
    CModel_ID id;
} CMesh;

typedef struct CModel {
    // base data
    CAsset_ID               asset_id;
    CAssetType              asset_type;

    // extended data
    RayModel                model;
    CModel_ID               id; // TODO: replace with CAsset_ID
    CModelType              type;
    char                    path[64];
    CGLM_ALIGN_MAT mat4     transform;
    int *                   indices;
    float *                 vertices;
} CModel;



// TODO: implementations for components of objects, configuration descriptions, etc.
typedef struct CComponent CComponent;
typedef struct CGadget CGadget;

// config used in created objects.
typedef struct CModelCfg {
    int * data;
} CModelCfg;

extern CModel cmodel_load(const char * file_name);

extern void cmodel_unload(CModel * cmodel);


#ifdef __cplusplus
}
#endif


#endif