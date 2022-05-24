#ifndef CSHAPE_H
#define CSHAPE_H

#include <asset.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum CShapeType {
    CSHAPE_SPHERE,
    CSHAPE_CUBE,
    CSHAPE_PLANE
} CShapeType;

typedef struct CShapeBase {
    // asset base
    CAsset_ID   asset_id;
    CAssetType  asset_type;
    // shape base
    CShapeType   shape_type; 

} CShapeBase;


typedef struct CSphere {
    // asset base
    CAsset_ID       asset_id;
    CAssetType      asset_type;
    // shape base
    CShapeType      shape_type; 

    // shape-related
    RayModel        model;

    float           radius;
    int             rings;
    int             slices;
} CSphere;

typedef struct CCube {
    // asset base
    CAsset_ID   asset_id;
    CAssetType  asset_type;
    // shape base
    CShapeType   shape_type; 

    // shape-related
    RayModel        model;

    float   width;
    float   height;
    float   length;
} CCube;

typedef struct CPlane {
    // asset base
    CAsset_ID   asset_id;
    CAssetType  asset_type;
    // shape base
    CShapeType   shape_type; 

    // shape-related
    RayModel        model;

    float   width;
    float   length;
    int     resX;
    int     resZ;
} CPlane;

typedef union CShape {
    CShapeBase  base;

    CSphere     sphere;
    CCube       cube;
    CPlane      plane;
} CShape;


#ifdef __cplusplus
}
#endif

#endif