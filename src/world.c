#include <world.h>

#include <bulletC_API.h>


CWorldState * WORLD_STATE;

void init_world() {
    // initialize global vector of CObjects (allocate heap memory)
    WORLD_STATE = malloc(sizeof(CWorldState));
    WORLD_STATE->cobjects = cvec_cobject_init();

    // NOTE: !!! use references when getting data from the vectors !!!
    CAsset * sphere_shape = &ASSETS_STATE->cassets.data[0];

    CAsset * terrain_model = &ASSETS_STATE->cassets.data[1];

    CAsset * plane = &ASSETS_STATE->cassets.data[2];

    vec3 sphere_loc = {0.0, 30.0, 0.0};
    CObject_addPhysicalSphereAsset(&ASSETS_STATE->cassets.data[0].shape.sphere, sphere_loc);

    CObject_addPhysicalModel(&ASSETS_STATE->cassets.data[1].model);

     // can be done on processing thread partially. binding ressources may need to be done on main thread.
//    Image img = LoadImage("terrain_textured03.png");

    Image img1 = LoadImage("terrain_textured.png");
    Image img2 = LoadImage("wood_texture.jpg");

//    Matrix terrain_transform = RENDER_STATE->terrain_model.model.transform;

//    RENDER_STATE->terrain_model.model.transform = MatrixRotateZ(210.0f);
//    RENDER_STATE->terrain_model.model.transform = MatrixRotateX(PI / 2.0);

    if ((img1.data == NULL)) {
        printf("Failed to load image.\n");
    } else {
        printf("image load success.\n");
    }

    CObject * terrain = &WORLD_STATE->cobjects.data[1];

//    ImageRotateCCW(&img);
//    ImageRotateCW(&img);

    terrain->terrain.model->materials[0].maps[0].texture = LoadTextureFromImage(img1);

    terrain->terrain.model->materials[1].maps[0].texture = LoadTextureFromImage(img2);
    terrain->terrain.model->materials[2].maps[0].texture = LoadTextureFromImage(img2);

    UnloadImage(img1);
    UnloadImage(img2);

//    CObject_addPhysicalPlaneAsset(&ASSETS_STATE->cassets.data[2].shape.plane);

    //  Physical objects are now added to the vector holding objects.
    //  Now we can iterate over that vector in our rendering and physics handling loop.
    // Render the objects by accessing the CAssets vector by asset ID and
    // getting the model/shape data from there, in combination with
    // the world transform and other config data as part of the CObject.
    // Handle the physics by directly acessing the b3RigidBody field and PhysCfg
    // that is part of the object.

    return;
}

void deinit_world() {
    // _del automatically invokes the _clear method, which calls CObject_del on each object
    // it then calls free() on the container, if it has capacity, deallocating the data.
    cvec_cobject_del(&WORLD_STATE->cobjects);

    free(WORLD_STATE);
}

// deletes values/references of an object in the container
// -- is called when object is removed from the container.
// -> deal with destructing attributes of CObject here.
void CObject_del(CObject * object) {
    // ... call the deinit functions here.
    if (object != NULL) {

        switch(object->base.object_type) {
            case OBJECT_TYPE_VISUAL:
                break;
            case OBJECT_TYPE_PHYSICAL:
                // NOTE: might this cause errors, incase data is corrupted?
                b3World_removeRigidBody(object->physical.physics_body);
                break;
            case OBJECT_TYPE_VIRTUAL:
                break;

            case OBJECT_TYPE_NONE:
                break;

            default:
                // no accepted game object -> not allowed! (should not be the case atm.)
                printf("Error! Not a valid object type.");
                break;
            // error!
        }

    } else {
        // error!
        printf("Error! CObject_del\n");
    }
}

// swap objects. (selected with the last one)
// -> so the selected one can be removed cheaply
//                swapped element  last element (before swap)
void CObject_swap(const CObject * cobj1, const CObject * cobj2) {
    const CObject * temp = cobj1;
    cobj1 = cobj2;
    cobj2 = temp;
}

int CObject_compare(const CObject * cobj1, const CObject * cobj2) {
    int c = c_default_compare(&cobj1->base.object_id, &cobj2->base.object_id);
    return c; // lesser value is returned. (1: cmodel_1 is lesser; -1: cmodel_2 is lesser. ; 0: neither is lesser.)
}

void CObject_remove(const CObject * cobj) {
    // retrieve by index.
    const CObject * object = cvec_cobject_at(&WORLD_STATE->cobjects, (size_t)cobj->base.object_id);

    // last object gets object id of object to be removed.
    CObject * last = cvec_cobject_back(&WORLD_STATE->cobjects);
    last->base.object_id = cobj->base.object_id;

    // put object adress to the end
    CObject_swap(object, last);

    // pop off ( triggers CObject_del )
    cvec_cobject_pop_back(&WORLD_STATE->cobjects);
}

void CObject_removeById(const CObject_ID object_id) {
    // retrieve by index.
    const CObject * object = cvec_cobject_at(&WORLD_STATE->cobjects, (size_t)object_id);
    // HINT: STC will do an assert whether index exists.

    // last object gets object id of object to be removed.
    CObject * last = cvec_cobject_back(&WORLD_STATE->cobjects);
    last->base.object_id = object_id;

    // put object adress to the end
    CObject_swap(object, last);
    // pop off
    cvec_cobject_pop_back(&WORLD_STATE->cobjects);
}

void CObject_removePhysical(const CPhysObject * phys) {
    // HINT: STC will do an assert whether index exists.
    const CObject * object = cvec_cobject_at( &WORLD_STATE->cobjects, (size_t)phys->object_id );

    // last object gets object id of object to be removed.
    CObject * last = cvec_cobject_back(&WORLD_STATE->cobjects);
    last->base.object_id = phys->object_id;

    // put object adress to the end
    CObject_swap(object, last);
    // pop off
    cvec_cobject_pop_back(&WORLD_STATE->cobjects);
}

void CObject_removeVirtual(const CVirtualObject * virt) {
    // HINT: STC will do an assert whether index exists.
    const CObject * object = cvec_cobject_at(&WORLD_STATE->cobjects, (size_t)virt->object_id);

    // last object gets object id of object to be removed.
    CObject * last = cvec_cobject_back(&WORLD_STATE->cobjects);
    last->base.object_id = virt->object_id;

    // put object adress to the end
    CObject_swap(object, last);
    // pop off
    cvec_cobject_pop_back(&WORLD_STATE->cobjects);
}

// create object. -> only "extended" CObjets
// -- create game objects from this function.
// -- hint: pass reference to a created CPhys/CLogiCObject (created on stack)
unsigned int CObject_addPhysicalModel(CModel * cmodel) {
    CObject object;
    object.base.object_type = OBJECT_TYPE_PHYSICAL;
    glm_mat4_identity(object.physical.info.base_info.world_transform);

    switch (cmodel->type) {
        case CMODEL_CHARACTER:
            object.character.info.character_info.physics_type = CPHYS_CHARACTER;
            glm_mat4_identity(object.character.info.character_info.world_transform);
            object.character.info.character_info.asset_id = cmodel->asset_id;
            object.character.model = &cmodel->model;
            object.character.model->materials[0].shader = SHADER_STATE->shader_phong;
            break;
        case CMODEL_VEHICLE:
            object.vehicle.info.vehicle_info.physics_type = CPHYS_VEHICLE;
            glm_mat4_identity(object.vehicle.info.vehicle_info.world_transform);
            object.vehicle.info.vehicle_info.asset_id = cmodel->asset_id;
            object.vehicle.model = &cmodel->model;
            object.vehicle.model->materials[0].shader = SHADER_STATE->shader_phong;
            break;
        case CMODEL_STRUCTURE:
            object.structure.info.structure_info.physics_type = CPHYS_STRUCTURE;
            glm_mat4_identity(object.structure.info.structure_info.world_transform);
            object.structure.info.structure_info.asset_id = cmodel->asset_id;
            object.structure.model = &cmodel->model;
            object.structure.model->materials[0].shader = SHADER_STATE->shader_phong;
            break;
        case CMODEL_TERRAIN:
            object.terrain.info.terrain_info.physics_type = CPHYS_TERRAIN;
            glm_mat4_identity(object.terrain.info.terrain_info.world_transform);
            object.terrain.info.terrain_info.asset_id = cmodel->asset_id;
            object.terrain.model = &cmodel->model;
            object.terrain.model->materials[0].shader = SHADER_STATE->shader_phong;
            break;
        case CMODEL_PARTICLES:
            break;
        case CMODEL_NONE:
            object.base.object_type = OBJECT_TYPE_NONE;
            break;
        default:
            break;
    }

    // TODO: consider creating b3RigidBody body here 
    // NOTE: decide whether to b3World_addRigidBody() here,
    // or later if we decide not to simulate physics before user has given the command.
    CPhysObject_createRigidBody(&object);

    b3World_addRigidBody(object.physical.physics_body);

    unsigned int index_of_added = (unsigned int)cvec_cobject_size(WORLD_STATE->cobjects);
    object.base.object_id = index_of_added;

    // this copies the values to the vector.
    cvec_cobject_push_back(&WORLD_STATE->cobjects, object);

    // HINT: cobj is created on stack, so no need to free() it.

    return index_of_added;
}

unsigned int CObject_addPhysicalFromCInfo(CPhysObjectInfo * desc) {
    CObject object;

    // choose cobject type
    object.base.object_type = OBJECT_TYPE_PHYSICAL;

    glm_mat4_identity( object.physical.info.base_info.world_transform );

    // add physical object description.
    switch (desc->base_info.physics_type) {
        case CPHYS_STRUCTURE:
            object.structure.model = &ASSETS_STATE->cassets.data[desc->structure_info.asset_id].model.model;
            memcpy(&object.structure.info.structure_info, &desc->structure_info, sizeof(CStructureInfo));
            break;
        case CPHYS_CHARACTER:
            object.character.model = &ASSETS_STATE->cassets.data[desc->character_info.asset_id].model.model;
            memcpy(&object.character.info.character_info, &desc->character_info, sizeof(CCharacterInfo));
            break;
        case CPHYS_VEHICLE:
            object.vehicle.model = &ASSETS_STATE->cassets.data[desc->vehicle_info.asset_id].model.model;
            memcpy(&object.vehicle.info.vehicle_info, &desc->vehicle_info, sizeof(CVehicleInfo));
            break;
        case CPHYS_TERRAIN:
            object.terrain.model = &ASSETS_STATE->cassets.data[desc->terrain_info.asset_id].model.model;
            memcpy(&object.terrain.info.terrain_info, &desc->terrain_info, sizeof(CTerrainInfo));
            break;
        default:
            printf("Continue...\n");
    }

    // TODO: consider creating b3RigidBody body here 
    // or later if we decide to run physics simulation.

    // finally add object id!
    unsigned int index_of_added = (unsigned int)cvec_cobject_size(WORLD_STATE->cobjects);
    object.base.object_id = index_of_added;

    // will allocate new memory for the CObject.
    cvec_cobject_push_back(&WORLD_STATE->cobjects, object);

    return index_of_added;
}

unsigned int CObject_addVirtual(CVirtualObject * cobj) {
    CObject object;
    memcpy(&object.virtual, cobj, sizeof(CVirtualObject));

    unsigned int index_of_added = (unsigned int)cvec_cobject_size(WORLD_STATE->cobjects);
    object.base.object_id = index_of_added;

    // will allocate new memory for the CObject.
    cvec_cobject_push_back(&WORLD_STATE->cobjects, object);

    // HINT: cobj is created on stack, so no need to free() it.

    return index_of_added;
}

unsigned int CObject_addPhysicalSphere(float radius, int rings, int slices) {
    return 0;
}

unsigned int CObject_addPhysicalSphereAsset(CSphere * sphere, vec3 translation) {
    CObject object;
    object.base.object_type = OBJECT_TYPE_PHYSICAL;

    // base info (NOTE: stored data hopefully also found in sphere_info )
    object.physical.info.base_info.physics_type = CPHYS_SPHERE;
    glm_mat4_identity(object.physical.info.base_info.world_transform);

    object.physical.info.base_info.world_transform[3][0] = translation[0];
    object.physical.info.base_info.world_transform[3][1] = translation[1];
    object.physical.info.base_info.world_transform[3][2] = translation[2];

    // asset related data
    object.sphere.info.sphere_info.asset_id = sphere->asset_id;
    object.sphere.info.sphere_info.isAsset = true;

    // body data
    object.sphere.info.sphere_info.radius = sphere->radius;
    object.sphere.info.sphere_info.rings = sphere->rings;
    object.sphere.info.sphere_info.slices = sphere->slices;

    object.sphere.model = &sphere->model;

//    object.sphere.model->materials[0].maps[MATERIAL_MAP_DIFFUSE].color = (Color){0, 0, 0, 255};
    object.sphere.model->materials[0].shader = SHADER_STATE->shader_phong;

    ////////////////////////////////////////
    // TODO: add physics rigid body here.
    ////////////////////////////////////////
    CPhysObject_createRigidBody(&object);

    b3World_addRigidBody(object.sphere.physics_body);

    unsigned int index_of_added = (unsigned int)cvec_cobject_size(WORLD_STATE->cobjects);
    object.base.object_id = index_of_added;

    cvec_cobject_push_back(&WORLD_STATE->cobjects, object);

    // HINT: cobj is created on stack, so no need to free() it.
    return index_of_added;
}

unsigned int CObject_addPhysicalCube(float width, float height, float length) {
    return 0;
}

unsigned int CObject_addPhysicalCubeAsset(CCube * cube) {
    CObject object;
    object.base.object_type = OBJECT_TYPE_PHYSICAL;

    // base info (NOTE: stored data hopefully also found in sphere_info )
    object.physical.info.base_info.physics_type = CPHYS_CUBE;
    glm_mat4_identity(object.physical.info.base_info.world_transform);

    // asset related data
    object.cube.info.cube_info.asset_id = cube->asset_id;
    object.cube.info.cube_info.isAsset = true;

    // body data
    object.cube.info.cube_info.width = cube->width;
    object.cube.info.cube_info.length = cube->length;
    object.cube.info.cube_info.height = cube->height;

    object.cube.model = &cube->model;

    ////////////////////////////////////////
    // TODO: add physics rigid body here.
    ////////////////////////////////////////
    CPhysObject_createRigidBody(&object);

    unsigned int index_of_added = (unsigned int)cvec_cobject_size(WORLD_STATE->cobjects);
    object.base.object_id = index_of_added;

    cvec_cobject_push_back(&WORLD_STATE->cobjects, object);

    // HINT: cobj is created on stack, so no need to free() it.
    return index_of_added;
}

unsigned int CObject_addPhysicalPlane(float width, float length, int resX, int resZ) {
    return 0;
}

unsigned int CObject_addPhysicalPlaneAsset(CPlane * plane) {
    CObject object;
    object.base.object_type = OBJECT_TYPE_PHYSICAL;

    // base info (NOTE: stored data hopefully also found in sphere_info )
    object.physical.info.base_info.physics_type = CPHYS_PLANE;
    glm_mat4_identity(object.physical.info.base_info.world_transform);

    object.physical.info.base_info.world_transform[3][0] = 0.0;
    object.physical.info.base_info.world_transform[3][1] = 0.0;
    object.physical.info.base_info.world_transform[3][2] = 0.0;

    // asset related data
    object.plane.info.plane_info.asset_id = plane->asset_id;
    object.plane.info.plane_info.isAsset = true;

    // body data
    object.plane.info.plane_info.width = plane->width;
    object.plane.info.plane_info.length = plane->length;
    object.plane.info.plane_info.resX = plane->resX;
    object.plane.info.plane_info.resZ = plane->resZ;

    object.plane.model = &plane->model;

    object.plane.model->materials[0].shader = SHADER_STATE->shader_phong;
    ////////////////////////////////////////
    // TODO: add physics rigid body here.
    ////////////////////////////////////////
    CPhysObject_createRigidBody(&object);

    b3World_addRigidBody(object.plane.physics_body);

    unsigned int index_of_added = (unsigned int)cvec_cobject_size(WORLD_STATE->cobjects);
    object.base.object_id = index_of_added;

    cvec_cobject_push_back(&WORLD_STATE->cobjects, object);

    // HINT: cobj is created on stack, so no need to free() it.
    return index_of_added;
}


unsigned int CObject_addBase(const CBaseObject * cobj) {
    CObject object;
    memcpy(&object.base, cobj, sizeof(CBaseObject));

    unsigned int index_of_added = (unsigned int)cvec_cobject_size(WORLD_STATE->cobjects);
    object.base.object_id = index_of_added;

    cvec_cobject_push_back(&WORLD_STATE->cobjects, object);

    // HINT: cobj is created on stack, so no need to free() it.

    return index_of_added;
}

void CPhysObjectInfo_initFromCModelDefault(const CModel * cmodel, CPhysObjectInfo * desc) {

    // init CPhysInfo from CModel data.

    switch (cmodel->type) {
        case CMODEL_CHARACTER:
            desc->character_info.physics_type = CPHYS_CHARACTER;
            glm_mat4_identity(desc->character_info.world_transform);
            desc->character_info.asset_id = cmodel->asset_id;
            break;
        case CMODEL_VEHICLE:
            desc->vehicle_info.physics_type = CPHYS_VEHICLE;
            glm_mat4_identity(desc->vehicle_info.world_transform);
            desc->vehicle_info.asset_id = cmodel->asset_id;
            break;
        case CMODEL_STRUCTURE:
            desc->structure_info.physics_type = CPHYS_STRUCTURE;
            glm_mat4_identity(desc->structure_info.world_transform);
            desc->structure_info.asset_id = cmodel->asset_id;
            break;
        case CMODEL_TERRAIN:
            desc->terrain_info.physics_type = CPHYS_TERRAIN;
            glm_mat4_identity(desc->terrain_info.world_transform);
            desc->terrain_info.asset_id = cmodel->asset_id;
            break;
        case CMODEL_PARTICLES:
//          TODO ...
            break;
        case CMODEL_NONE:
            desc->base_info.physics_type = CPHYS_NONE;
            glm_mat4_identity(desc->base_info.world_transform);
//            desc->base_info.asset_id = cmodel->asset_id;
            break;
        default:
            return;
    }

    return;
}

void CSphereObject_createRigidBody(CSphereObject * object) {

    vec3 origin = {
                        object->info.character_info.world_transform[3][0],
                        object->info.character_info.world_transform[3][1],
                        object->info.character_info.world_transform[3][2]
    };

    vec3 inertia = { 2.0, 2.0, 2.0 };

    object->physics_body = b3RigidBody_initSphere(origin, object->info.sphere_info.radius, inertia, 5.0);

    return;
}

void CCubeObject_createRigidBody(CCubeObject * object) {

    return;
}

void CPlaneObject_createRigidBody(CPlaneObject * object) {

    printf("Creating Plane RigidBody\n");

    vec3 origin = {
                        object->info.character_info.world_transform[3][0],
                        object->info.character_info.world_transform[3][1],
                        object->info.character_info.world_transform[3][2]
    };

    object->physics_body = b3RigidBody_initPlane(origin);

    printf("Created Plane RigidBody\n");

    return;
}

CNECTOR_INLINE void CCharacterObject_createRigidBody(CCharacterObject * object) {

    return;
}

CNECTOR_INLINE void CVehicleObject_createRigidBody(CVehicleObject * object) {

    return;
}

CNECTOR_INLINE void CStructureObject_createRigidBody(CStructureObject * object) {

    return;
}

CNECTOR_INLINE void CTerrainObject_createRigidBody(CTerrainObject * object) {

    int triangle_count = (object->model->meshes + 0)->triangleCount;
    int vertex_count = (object->model->meshes + 0)->vertexCount;

    int * indices = (int*)(object->model->meshes + 0)->indices;

    int vertexStride = 3 * sizeof(float);
    int indexStride = 3 * sizeof(int);

    object->physics_body = b3RigidBody_initBvhTriangleMesh(
        triangle_count,
        indices,
        indexStride,
        vertex_count,
        object->model->vertices,
        vertexStride
    );

    return;
}

inline void CPhysObject_createRigidBody(CObject * object) {

    if (object->base.object_type == OBJECT_TYPE_PHYSICAL) {

        switch (object->physical.info.base_info.physics_type) {

            case CPHYS_CHARACTER:
                CCharacterObject_createRigidBody(&object->character);
                break;
            case CPHYS_VEHICLE:
                CVehicleObject_createRigidBody(&object->vehicle);
                break;
            case CPHYS_STRUCTURE:
                CStructureObject_createRigidBody(&object->structure);
                break;
            case CPHYS_TERRAIN:
                CTerrainObject_createRigidBody(&object->terrain);
                break;
            case CPHYS_SPHERE:
                CSphereObject_createRigidBody(&object->sphere);
                break;
            case CPHYS_CUBE:
                CCubeObject_createRigidBody(&object->cube);
                break;
            case CPHYS_PLANE:
                CPlaneObject_createRigidBody(&object->plane);
                break;
            default:
                break;
        }
    }

    return;
}