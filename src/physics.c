#include <physics.h>

CPhysicsState * PHYSICS_STATE;


void init_physics() {
    PHYSICS_STATE = malloc(sizeof(CPhysicsState));

    b3World_default();

    vec3 vector_grav = { 0.0f, -10.0f, 0.0f };
    b3World_setGravity(vector_grav);

    PHYSICS_STATE->simulation = true;
}

void deinit_physics() {
    PHYSICS_STATE->simulation = false;
    b3World_deinit();

    free(PHYSICS_STATE);
}

void process_physics() {

    if ( PHYSICS_STATE->simulation ) {
        b3World_stepSimulation(_SIM_STEP);
    }
}

void update_physics() {

    if ( PHYSICS_STATE->simulation ) {

        for (size_t i = 0 ; i < cvec_cobject_size(WORLD_STATE->cobjects) ; i++) {

            // NOTE: !!! use references when modifying items in the containers !!!
            // Otherwise we are changing only the copy! Not the object itself!
            CObject * object = &WORLD_STATE->cobjects.data[i];

            switch(object->base.object_type) {
                case OBJECT_TYPE_PHYSICAL:
    //                    printf("Found Physical object in cobjects.\n");
                    switch(object->physical.info.base_info.physics_type) {
                        case CPHYS_SPHERE:
    //                            printf("Found Sphere. Getting transform.\n");
                            b3RigidBody_getTransform(
                                object->physical.physics_body,
                                object->physical.info.sphere_info.world_transform
                            );
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}