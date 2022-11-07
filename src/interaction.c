#include <interaction.h>


#include <stdlib.h>


CInteractionController * INTERACTION_CONTROLLER;


void init_interaction() {
    // init interaction controller here.
}

void deinit_interaction() {
    // TODO: check if can be safely zero'ed.
    INTERACTION_STATE->object_id = 0;
    INTERACTION_STATE->current_mode = INTERACTIONMODE_NONE;
    INTERACTION_STATE->bodies_affected = 0;
    INTERACTION_STATE->interaction_object = ((CPhysObject*)NULL);

    free(INTERACTION_STATE);
}

void assign_interaction(CPhysObject * object) {
//    INTERACTION_STATE->rigid_body = body;
    INTERACTION_STATE->interaction_object = object;
}

void unassign_interaction() {
//    INTERACTION_STATE->rigid_body = ((b3RigidBody*)NULL);
    INTERACTION_STATE->interaction_object = ((CPhysObject*)NULL);
}

// TOOD: dont assign to interaction controller. just control movement straight from the InputState object.
inline void process_interaction() {
    vec3 my_vec;
//    printf("Starting process_interaction!\n");

    if (
            ( INPUT_STATE->mode ==  (InputMode)INPUTMODE_INTERACTION ) ||
                ( INPUT_STATE->mode == (InputMode)INPUTMODE_CAMERA_AND_INTERACTION ) &&
                ( INTERACTION_STATE->interaction_object->physics_body != ((b3RigidBody*)NULL) )
    )   {
            // free camera mode. move around and rotate freely. no selected target.
            switch (INTERACTION_STATE->current_mode) {
                // move interaction object around with designated buttons. 
                // physics simulation -not- active.
                // camera will neither follow nor target it.
                // move direction of object depends on angle of camera direction relative to the object. (0, 90, 180, 270)
                case INTERACTIONMODE_MOVE:
                {
                    // TODO: check for current InteractionMode
                    if(INPUT_STATE->left) {
                    } else {
                    }
                    if(INPUT_STATE->right) {
                    } else {
                    }
                    if(INPUT_STATE->forward) {
                        // move along world z-axis, irrespective of camera direction.
                        Vector3 look_vec = CAMERA_STATE->camera.target;
                        vec3 new_vec = {0.0f, 0.0f, -10.0f};

                        b3RigidBody_setVelocity(
                            INTERACTION_STATE->interaction_object->physics_body,
                            new_vec
                        );
                    } else {
                    }
                    if(INPUT_STATE->backward) {
                    } else {
                    }
                    if(INPUT_STATE->up) {
                    } else {
                    }
                    if(INPUT_STATE->down) {
                    } else {
                    }
                    break;
                }
                // free travel mode. move camera around freely with mouse or designated buttons.
                // physics simulation active. 
                // interaction object/character/vehicle will not react to camera direction changes.
                // --> only to object direction changes via designated buttons.
                case INTERACTIONMODE_ROTATE:
                {
                    break;
                }
                default:
                    return;
                // ...
            }
    }

//    printf("Finished process_interaction!\n");
}