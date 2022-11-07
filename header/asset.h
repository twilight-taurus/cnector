#ifndef CASSET_H
#define CASSET_H

#ifdef __cplusplus
extern "C" {
#endif

//typedef Model                   RayModel;
//typedef Mesh                    RayMesh;

typedef short                   CAsset_ID;

typedef enum CAssetType {
    CASSET_MODEL,

    CASSET_SHAPE,
    CASSET_TEXTURE,
    CASSET_MATERIAL
} CAssetType;

typedef struct CAssetBase {
    CAsset_ID   asset_id;
    CAssetType  asset_type;
} CAssetBase;


#ifdef __cplusplus
}
#endif


#endif