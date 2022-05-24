#include <assets.h>

CAssetsState * ASSETS_STATE;

// on removed from cvec
void CAsset_del(CAsset * casset) {

    switch (casset->base.asset_type) {
        case CASSET_MODEL:
            cmodel_unload( &casset->model );
            break;
        case CASSET_SHAPE:
            break;
        default:
            break;
    }

    return;
}

int CAsset_compare(const CAsset * casset_1, const CAsset * casset_2) {
    int c = c_default_compare(&casset_1->base.asset_id, &casset_2->base.asset_id);
    return c; // lesser value is returned. (1: cmodel_1 is lesser; -1: cmodel_2 is lesser. ; 0: neither is lesser.)
}

void CAsset_swap(const CAsset * cobj1, const CAsset * cobj2) {
    const CAsset * temp = cobj1;
    cobj1 = cobj2;
    cobj2 = temp;
}


void CAsset_removeById(const CAsset_ID asset_id) {
    // retrieve by index.
    const CAsset * asset = cvec_casset_at(&ASSETS_STATE->cassets, (size_t)asset_id);

    // last model gets id of object to be removed. (NOTE: might cause errors)
    CAsset * last = cvec_casset_back(&ASSETS_STATE->cassets);
    last->base.asset_id = asset->base.asset_id;

    CAsset_swap(asset, last);

    // pop off
    cvec_casset_pop_back(&ASSETS_STATE->cassets);

    return;
}

CAsset * CAsset_get(const CAsset_ID asset_id) {

    return &ASSETS_STATE->cassets.data[asset_id];
}

int CModel_addFromFile(const char * file_name, CModelType type) {
    CAsset casset;

    casset.model = cmodel_load(file_name);

    if (casset.model.model.meshCount <= 0) {
        printf("Failed to load model.\n");
        return -1;
    }

    int index_of_added = (int)cvec_casset_size(ASSETS_STATE->cassets);

    casset.base.asset_id = (CAsset_ID)index_of_added; // set index as model id
    casset.base.asset_type = CASSET_MODEL;

    casset.model.type = type;

    // this copies the values to the vector.
    cvec_casset_push_back(&ASSETS_STATE->cassets, casset);

    return index_of_added;
}


void CModel_remove(const CModel * cmodel) {
    const CAsset * model_to_rm = cvec_casset_at(&ASSETS_STATE->cassets, (size_t)cmodel->asset_id);

    // last model gets id of object to be removed. (NOTE: might cause errors)
    CAsset * last = cvec_casset_back(&ASSETS_STATE->cassets);
    last->base.asset_id = model_to_rm->base.asset_id;

    CAsset_swap( model_to_rm, last );

    // pop off
    cvec_casset_pop_back(&ASSETS_STATE->cassets);
}


int CShape_addSphere(float radius, int rings, int slices) {
    CAsset casset;

    int index_of_added = (int)cvec_casset_size(ASSETS_STATE->cassets);

    casset.shape.sphere.model = LoadModelFromMesh( GenMeshSphere(radius, rings, slices) );
    casset.shape.sphere.shape_type = CSHAPE_SPHERE;
    casset.base.asset_type = CASSET_SHAPE;
    casset.base.asset_id = (size_t)index_of_added;

    casset.shape.sphere.radius = radius;
    casset.shape.sphere.rings = rings;
    casset.shape.sphere.slices = slices;

    // this copies the values to the vector.
    cvec_casset_push_back(&ASSETS_STATE->cassets, casset);

    return index_of_added;
}

int CShape_addCube(float width, int height, int length) {
    CAsset casset;

    int index_of_added = (int)cvec_casset_size(ASSETS_STATE->cassets);

    casset.shape.sphere.model = LoadModelFromMesh( GenMeshCube(width, height, length) );
    casset.shape.sphere.shape_type = CSHAPE_CUBE;
    casset.base.asset_type = CASSET_SHAPE;
    casset.base.asset_id = (size_t)index_of_added;

    casset.shape.cube.width = width;
    casset.shape.cube.height = height;
    casset.shape.cube.length = length;

    // this copies the values to the vector.
    cvec_casset_push_back(&ASSETS_STATE->cassets, casset);

    return index_of_added;
}

int CShape_addPlane(float width, int length, int resX, int resZ) {
    CAsset casset;

    int index_of_added = (int)cvec_casset_size(ASSETS_STATE->cassets);

    casset.shape.plane.model = LoadModelFromMesh( GenMeshPlane(width, length, resX, resZ) );
    casset.shape.plane.shape_type = CSHAPE_PLANE;
    casset.base.asset_type = CASSET_SHAPE;
    casset.base.asset_id = (size_t)index_of_added;

    casset.shape.plane.width = width;
    casset.shape.plane.length = length;
    casset.shape.plane.resX = resX;
    casset.shape.plane.resZ = resZ;

    // this copies the values to the vector.
    cvec_casset_push_back(&ASSETS_STATE->cassets, casset);

    return index_of_added;
}


void CShape_remove(const CShape * shape) {
    const CAsset * asset = cvec_casset_at(&ASSETS_STATE->cassets, (size_t)shape->base.asset_id);

    // last model gets id of object to be removed. (NOTE: might cause errors)
    CAsset * last = cvec_casset_back(&ASSETS_STATE->cassets);
    last->base.asset_id = asset->base.asset_id;

    CAsset_swap( asset, last );

    // pop off
    cvec_casset_pop_back(&ASSETS_STATE->cassets);
}


void init_assets() {
    ASSETS_STATE = malloc(sizeof(CAssetsState));

    ASSETS_STATE->cassets = cvec_casset_init();

    // init/load base assets.

    // controlled sphere
    CShape_addSphere(2.0, 30, 30);

    // terrain model
//    CModel_addFromFile("terrain_textured03.obj", CMODEL_TERRAIN);

    CModel_addFromFile("level_arena.obj", CMODEL_TERRAIN);

    // plane
    CShape_addPlane(500, 500, 1, 1);

    return;
}

void deinit_assets() {
//    cvec_cmodel_del(&CVEC_CMODEL);
    cvec_casset_del(&ASSETS_STATE->cassets);

    free(ASSETS_STATE);

    return;
}