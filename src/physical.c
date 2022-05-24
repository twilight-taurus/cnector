#include <physical.h>

/*
cvec_cphysitem CVEC_CPHYSITEM;


void CPhysItem_del(CPhysItem * item) {

}

int CPhysItem_compare(const CPhysItem * item1, const CPhysItem * item2) {
    //TODO: maybe consider changing, since should be the cmp value in vector of models.
    int c = c_default_compare(
        &item1->physical.info.base_info.model_id, 
        &item2->physical.info.character_info.
    );

    return c; // lesser value is returned. (1: cmodel_1 is lesser; -1: cmodel_2 is lesser. ; 0: neither is lesser.)
}
*/

/*
CPhysObject CPhysObject_init(const char * file_name) {
    CPhysObject obj;
    obj.object_type = OBJECT_TYPE_PHYSICAL;

    // add model to a vector of models in cmodel_load
//    CModel model = cmodel_load(file_name);

    int ret = CModel_add(file_name);
    if (ret < 0) {
        printf("Failed to add model.\n");
    }

    obj.info.base_info.physics_type = CPHYS_NONE;

    CModelCfg model_cfg;
    // add fields to model_cfg, although it should not be necessary since Raylib Model type should contain all necessary data.
    model_cfg.data = NULL;

    // the core physics configuration. although possibly not necessary, since btRigidBody should contain all necessary data.
    CPhysCfg phys_cfg;
    phys_cfg.inertia = 0.0;
    phys_cfg.mass = 20.0;

    CPhysBaseInfo base_cfg;
    // determine model id by adding it to a vector containing loaded models.

    base_cfg.phys_config = phys_cfg;

    CPhysObjectInfo obj_cfg;
    obj_cfg.base_info = base_cfg;

    obj.info = obj_cfg;

    // determine object id by adding the object to the vector of objects.
    obj.object_id = 0;

    // create a btRigidBody, and store it in this field.
//    obj.physics_body = //...

// Creation of basic physics object finished.
////////////////////////////////////////////////////////////////
    return obj;
}

CVehicleObject CPhysObject_initCVehicle(const char * file_name) {
    CVehicleObject veh_obj;
    return veh_obj;
}

CCharacterObject CPhysObject_initCCharacter(const char * file_name) {
    CCharacterObject character_obj;
    return character_obj;
}

CStructureObject CPhysObject_initCStructure(const char * file_name) {
//    CPhysObject base_obj = CPhysObject_init(file_name);

    CStructureObject structure_obj;
    return structure_obj;
}

*/