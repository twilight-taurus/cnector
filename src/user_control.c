#include <user_control.h>

#include <state.h>
#include <input.h>


void init_control() {
    return;
}

void deinit_control() {
    return;
}

void assign_control(CPhysObject * obj) {
    obj->info.base_info.controlled = true;
    CONTROL_STATE->control_object = obj;
}

void unassign_control() {
    CONTROL_STATE->control_object = ((CPhysObject*)NULL);
}

static inline void control_travel() {
            // free camera mode. move around and rotate freely. no selected target.
    if(INPUT_STATE->left) {
        vec3 new_vec = {
            CAMERA_STATE->right[0] * 10.0f, // x
            CAMERA_STATE->right[1] * 10.0f, // y
            CAMERA_STATE->right[2] * 10.0f // z
        };
        
        b3RigidBody_setVelocity(
            CONTROL_STATE->control_object->physics_body,
            new_vec
        );
    } else {
    }
    if(INPUT_STATE->right) {
        vec3 new_vec = {
            CAMERA_STATE->right[0] * 10.0f, // x (negate the x-axis on lateral movement.)
            CAMERA_STATE->right[1] * 10.0f, // y
            CAMERA_STATE->right[2] * 10.0f // z
        };

        glmc_vec3_negate(new_vec);

        b3RigidBody_setVelocity(
            CONTROL_STATE->control_object->physics_body,
            new_vec
        );
    } else {
    }
    if(INPUT_STATE->forward) {
        // move along world z-axis, irrespective of camera direction
        vec3 new_vec = {
            CAMERA_STATE->direction[0] * 10.0f,
            CAMERA_STATE->direction[1] * 10.0f,
            CAMERA_STATE->direction[2] * 10.0f
        };

        b3RigidBody_setVelocity(
            CONTROL_STATE->control_object->physics_body,
            new_vec
        );
    } else {
    }
    if(INPUT_STATE->backward) {
        vec3 new_vec = {
            CAMERA_STATE->direction[0] * 10.0f,
            CAMERA_STATE->direction[1] * 10.0f,
            CAMERA_STATE->direction[2] * 10.0f // (negate the z-axis on horizontal movement.)
        };
        glmc_vec3_negate(new_vec);

        b3RigidBody_setVelocity(
            CONTROL_STATE->control_object->physics_body,
            new_vec
        );
    } else {
    }
    if(INPUT_STATE->up) {

        vec3 new_vec = {
            CAMERA_STATE->up_vector[0] * 10.0f,
            CAMERA_STATE->up_vector[1] * 10.0f,
            CAMERA_STATE->up_vector[2] * 10.0f
        };

        b3RigidBody_setVelocity(
            CONTROL_STATE->control_object->physics_body,
            new_vec
        );
    } else {
    }
    if(INPUT_STATE->down) {

        vec3 new_vec = {
            CAMERA_STATE->up_vector[0] * 10.0f,
            CAMERA_STATE->up_vector[1] * 10.0f,
            CAMERA_STATE->up_vector[2] * 10.0f
        };
        glmc_vec3_negate(new_vec);

        b3RigidBody_setVelocity(
            CONTROL_STATE->control_object->physics_body,
            new_vec
        );
    } else {
    }

    // free travel mode. move camera around freely with mouse or designated buttons.
    // physics simulation active. 
    // interaction object/character/vehicle will not react to camera direction changes.
    // --> only to object direction changes via designated buttons.
}

static inline void control_shoot() {

}

static inline void control_menu() {
    return;
}

void process_control() {
//    printf("processing control!\n");
    if ( CONTROL_STATE->control_object != ((CPhysObject*)NULL) ) {
//        printf("processing control! control object exists.\n");
        switch (CONTROL_STATE->current_mode) {
            case CONTROLMODE_TRAVEL:
                control_travel();
                control_shoot();
//                printf("processed controlmode travel!\n");
                return;
            case CONTROLMODE_MENU:
                control_menu();
//                printf("processed controlmode menu!\n");
                return;
            default:
                return;
        }
    } else {
        // free or spectator or ghost camera mode.
    }
}