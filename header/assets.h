#ifndef CASSETS_H
#define CASSETS_H

#include <model.h>
#include <shape.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef union CAsset {
    CAssetBase  base;

    CModel      model;
    CShape      shape;

    // TODO: add CTexture, CMaterial, etc.
} CAsset;

// define containers and functions for loaded models here.

// load models from this function externally
// it will load and add them to the vector of loaded models.


// assets
extern void                 CAsset_removeById(const CAsset_ID asset_id);

extern CAsset               * CAsset_get(const CAsset_ID asset_id);

// models
extern int                  CModel_addFromFile(const char * file_name, CModelType type);

//extern int                  CModel_addFromMesh(Mesh mesh, CModelType type);

extern void                 CModel_remove(const CModel * cmodel);

//extern void                 CModel_removeById(const CModel_ID cmodel_id);

extern CModel               * CModel_get(const CAsset_ID cmodel_id);


// shapes
extern int                  CShape_addSphere(float radius, int rings, int slices);

extern int                  CShape_addCube(float width, int height, int length);

extern int                  CShape_addPlane(float width, int length, int resX, int resZ);


extern void                 CShape_remove(const CShape * shape);


// define map container to store IDs of all imported CModels and respective data.
// -> map, because we only need one object of each individual model.

// -- we could insted define vector.
// -- and use indexes of push_backs as "pseudo" keys
// -> (use _size function -> it references a field, which stores the amount of objects in it 
// -- then use swap function to put objects at the back
// -- and finally pop them off. (hopefully it works in STC)

void CAsset_del(CAsset * casset);

int CAsset_compare(const CAsset * cmodel_1, const CAsset * cmodel_2);

void CAsset_swap(const CAsset * cobj1, const CAsset * cobj2);

// define vector container type that will hold loaded models.
#define i_val       CAsset          // value: REQUIRED
#define i_cmp       CAsset_compare        // three-way compare two i_valraw* : REQUIRED IF i_valraw is a non-integral type
//#define i_del       CModel_del // destroy value func - defaults to empty destruct

//#define i_valraw    // convertion "raw" type - defaults to i_val
//#define i_valfrom   // convertion func i_valraw => i_val - defaults to plain copy
//#define i_valto     // convertion func i_val* => i_valraw - defaults to plain copy
#define i_tag       casset // defaults to i_val
#include <stc/cvec.h>

typedef struct CAssetsState {
    cvec_casset cassets;
} CAssetsState;

extern CAssetsState * ASSETS_STATE;

//extern cvec_cmodel CVEC_CMODEL;

extern void init_assets();

extern void deinit_assets();

#ifdef __cplusplus
}
#endif

#endif