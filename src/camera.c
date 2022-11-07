#include "camera.h"

#include <stdio.h>
#include <string.h>


CameraController * CAMERA_CONTROLLER;

// initially set to true
bool FIRST_MOUSE = true;

int init_camera() {
    // initialize Camera Controller here.
    CAMERA_CONTROLLER = malloc(sizeof(CameraController));

//    SetCameraMode(CAMERA_STATE->camera, CAMERA_PERSPECTIVE);

    return 0;
}

int deinit_camera() {
    free(CAMERA_CONTROLLER);

    return 0;
}

inline void process_mouse() {

    Vector2 mouse_pos = GetMousePosition();
    float sensitivity = 0.1;

    if (FIRST_MOUSE) {
        CAMERA_STATE->lastX = mouse_pos.x;
        CAMERA_STATE->lastY = mouse_pos.y;
        FIRST_MOUSE = false;
    }

    float xoffset = mouse_pos.x - CAMERA_STATE->lastX;
    float yoffset = CAMERA_STATE->lastY - mouse_pos.y;

    CAMERA_STATE->lastX = mouse_pos.x;
    CAMERA_STATE->lastY = mouse_pos.y;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    CAMERA_STATE->yaw += xoffset;
    CAMERA_STATE->pitch += yoffset;

    // ensure user is able to look up to sky, and down to feet
    // but not further.
    if(CAMERA_STATE->pitch > 89.0f)
        CAMERA_STATE->pitch =  89.0f;
    if(CAMERA_STATE->pitch < -89.0f)
        CAMERA_STATE->pitch = -89.0f;

}

// adjust camera position according to a targeted object
void process_target() {
//    printf("processing camera target!\n");
        // get transformation matrix of current interaction object.
    CGLM_ALIGN_MAT mat4 transform;
    b3RigidBody_getTransform( CONTROL_STATE->control_object->physics_body, transform );
//    printf("Got camera target transform!\n");
    // adjust camera values here according to interaction object positional change.
    // use current camera direction, but adjust the distance.
//    memcpy_s( CAMERA_STATE->target, 4* sizeof(float), transform[3], 4 * sizeof(float) );
    CAMERA_STATE->target[0] = transform[3][0];
    CAMERA_STATE->target[1] = transform[3][1];
    CAMERA_STATE->target[2] = transform[3][2];

//    printf("set camera target transform!\n");
        /*

            We calculate the rotation (and direction)in process_rotation. 
            -- here we only process the new values for the camera target.

        */
}
// adjust camera position in accordance with the controlled object's position.
void process_position() {
    CGLM_ALIGN_MAT mat4 transform;
    b3RigidBody_getTransform( CONTROL_STATE->control_object->physics_body, transform );

    CAMERA_STATE->position[0] = transform[3][0];
    CAMERA_STATE->position[1] = transform[3][1];
    CAMERA_STATE->position[2] = transform[3][2];
}

// free camera movement.

// left, right
inline void process_lateral() {

    if (INPUT_STATE->left && INPUT_STATE->right) {
        return;
    }

    if (INPUT_STATE->left) {
        glmc_vec3_sub(CAMERA_STATE->position, CAMERA_STATE->right, CAMERA_STATE->position);   
    } else if (INPUT_STATE->right) {
        glmc_vec3_add(CAMERA_STATE->position, CAMERA_STATE->right, CAMERA_STATE->position);  
    }
}
// up, down
inline void process_vertical() {

    if (INPUT_STATE->up && INPUT_STATE->down) {
        return;
    }

    if (INPUT_STATE->up) {
        // move camera up
        glmc_vec3_add(CAMERA_STATE->position, CAMERA_STATE->up_vector, CAMERA_STATE->position); 

    } else if (INPUT_STATE->down) {
        // move camera down
        glmc_vec3_sub(CAMERA_STATE->position, CAMERA_STATE->up_vector, CAMERA_STATE->position); 
    }
}
// forward,backward
inline void process_horizontal() {

    if (INPUT_STATE->forward && INPUT_STATE->backward) {
        return;
    }

    if (INPUT_STATE->forward) {
        glmc_vec3_add(CAMERA_STATE->position, CAMERA_STATE->direction, CAMERA_STATE->position);
    } else if (INPUT_STATE->backward) {
        glmc_vec3_sub(CAMERA_STATE->position, CAMERA_STATE->direction, CAMERA_STATE->position);
    }
}

static inline void process_directional() {
    process_lateral();
    process_vertical();
    process_horizontal();
}



// rotational movement -> recalculate directional, right, and up vector here.
static inline void process_rotational() {

    Vector2 mouse_pos;
    float xoffset;
    float yoffset;
    const float sensitivity = 0.1f;
    vec3 direction;
    float rad_yaw;
    float rad_pitch;

//    printf("processing camera rotational!\n");
    switch (CAMERA_STATE->mode) {
        case CAMERAMODE_FREE:
            process_mouse();

            // convert angle to radians first.
            direction[0] = cosf(glm_rad(CAMERA_STATE->yaw)) * cosf(glm_rad(CAMERA_STATE->pitch)); // x
            direction[2] = sinf(glm_rad(CAMERA_STATE->yaw)) * cosf(glm_rad(CAMERA_STATE->pitch)); // z
            direction[1] = sinf(glm_rad(CAMERA_STATE->pitch)); // y
            glmc_vec3_normalize_to(direction, CAMERA_STATE->direction);

            // TODO: fix. --> correct now.
            glmc_vec3_add(CAMERA_STATE->position, CAMERA_STATE->direction, CAMERA_STATE->target);

            // process new values for up vector, and right vector ...
            // get right vector by getting cross product up vector and direction vector. (x axis vector)
            glmc_vec3_cross(CAMERA_STATE->up_vector, CAMERA_STATE->direction, CAMERA_STATE->right);
            glmc_vec3_normalize(CAMERA_STATE->right);

            // get the up vector for current camera view.
//                glmc_vec3_cross(CAMERA_STATE->direction, CAMERA_STATE->right, CAMERA_STATE->up_vector);
//                glmc_vec3_normalize(CAMERA_STATE->up_vector);

            break;
        case CAMERAMODE_THIRDPERSON:
            process_mouse();

            rad_yaw = glm_rad(CAMERA_STATE->yaw);
            rad_pitch = glm_rad(CAMERA_STATE->pitch);

            // convert angle to radians first.
            direction[0] = cosf(rad_yaw) * cosf(rad_pitch); // x
            direction[2] = sinf(rad_yaw) * cosf(rad_pitch); // z
            direction[1] = sinf(rad_pitch); // y
            // normalize first (z-axis/ forward vector)
            glmc_vec3_normalize_to(direction, CAMERA_STATE->direction);

            // now multiply direction with preferred distance.
            // and set camera position depending on target of third person camera.
            vec3 distanceVec;
            glmc_vec3_scale(CAMERA_STATE->direction, CAMERA_STATE->distance, distanceVec);

            glmc_vec3_sub(CAMERA_STATE->target, distanceVec, CAMERA_STATE->position);

            // process new values for up vector, and right vector ...
            // get right vector by getting cross product up vector and direction vector. (x axis vector)
            glmc_vec3_cross(CAMERA_STATE->up_vector, CAMERA_STATE->direction, CAMERA_STATE->right);
            glmc_vec3_normalize(CAMERA_STATE->right);

            // get the up vector for current camera view. (y axis vector)
//            glmc_vec3_cross(CAMERA_STATE->direction, CAMERA_STATE->right, CAMERA_STATE->up_vector);
//            glmc_vec3_normalize(CAMERA_STATE->up_vector);
            break;
        case CAMERAMODE_FIRSTPERSON:
            process_mouse();

            rad_yaw = glm_rad(CAMERA_STATE->yaw);
            rad_pitch = glm_rad(CAMERA_STATE->pitch);

            // convert angle to radians first.
            direction[0] = cosf(rad_yaw) * cosf(rad_pitch); // x
            direction[2] = sinf(rad_yaw) * cosf(rad_pitch); // z
            direction[1] = sinf(rad_pitch); // y
            // normalize first (z-axis/ forward vector)
            glmc_vec3_normalize_to(direction, CAMERA_STATE->direction);

            // TODO: apply target/direction to camera object.
            // --> not necessary, since we only care about direction.
//            vec3 distanceVec;
//            glmc_vec3_scale(CAMERA_STATE->direction, CAMERA_STATE->distance, distanceVec);

            // set target with unit direction vector from camera position.
//            glmc_vec3_sub(CAMERA_STATE->target, distanceVec, CAMERA_STATE->position);
            glmc_vec3_add(CAMERA_STATE->position, CAMERA_STATE->direction, CAMERA_STATE->target);

            // process new values for up vector, and right vector ...
            // get right vector by getting cross product up vector and direction vector. (x axis vector)
            glmc_vec3_cross(CAMERA_STATE->up_vector, CAMERA_STATE->direction, CAMERA_STATE->right);
            glmc_vec3_normalize(CAMERA_STATE->right);
            break;

        default:
            return;
    }
}
// zoom in, zoom out (adjust field of view, etc.)
inline void process_zoom() {
    if (INPUT_STATE->wheel_up) {
        if ( !(CAMERA_STATE->distance <= 10.0) ) {
            CAMERA_STATE->distance -= 10.0;
        }
    } else if (INPUT_STATE->wheel_down) {
        if ( !(CAMERA_STATE->distance >= 100.0) ) {
            CAMERA_STATE->distance += 10.0;
        }
    }
}
// third person camera mode.
static inline void process_camera_thirdperson() {
//    printf("processing camera thirdperson!\n");
    switch(CONTROL_STATE->current_mode) {
        // regular 3rd person mode
        case CONTROLMODE_TRAVEL:
//            printf("processing controlmode travel!\n");
            process_target(); // process changes in camera target (depending on physics simulation)
//            printf("processed camera target!\n");
            process_rotational();
            process_zoom();
            break;
        case CONTROLMODE_MENU:
//            printf("processing controlmode menu!\n");
            // camera in menu
            process_rotational();
            process_zoom();
            break;
        default:
            return;
    }

    return;
}

// free camera mode.
static inline void process_camera_free() {
//    printf("processing camera free!\n");
    process_directional();

    process_rotational();
    process_zoom();
}

static inline void process_camera_firstperson() {

    process_position();

    process_rotational();
    process_zoom();
}





void update_camera() {
    // updates camera for currently set mode.
//    UpdateCamera(&CAMERA_STATE->camera);

    CAMERA_STATE->camera.position.x = CAMERA_STATE->position[0];
    CAMERA_STATE->camera.position.y = CAMERA_STATE->position[1];
    CAMERA_STATE->camera.position.z = CAMERA_STATE->position[2];

    CAMERA_STATE->camera.target.x = CAMERA_STATE->target[0];
    CAMERA_STATE->camera.target.y = CAMERA_STATE->target[1];
    CAMERA_STATE->camera.target.z = CAMERA_STATE->target[2];

//    CAMERA_STATE->camera.up = (Vector3){CAMERA_STATE->up_vector[0], CAMERA_STATE->up_vector[1], CAMERA_STATE->up_vector[2]};

    return;
}

void process_camera() {
//    printf("processing camera!\n");
    switch (CAMERA_STATE->mode) {
        case CAMERAMODE_FREE:
//            printf("processing camera free!\n");
            process_camera_free();
            break;
        case CAMERAMODE_THIRDPERSON:
//            printf("processing camera third!\n");
            process_camera_thirdperson();
            break;
        case CAMERAMODE_FIRSTPERSON:
            process_camera_firstperson();
            break;
        default:
            return;
    }

    return;
}

CCameraState * get_camera_state() {
    return CAMERA_STATE;
}

void set_camera_state(CCameraState * c_state) {
    memcpy(CAMERA_STATE, c_state, sizeof(CCameraState));
    return;
}

void set_camera(Camera camera) {
    CAMERA_STATE->camera = camera;
    return;
}

CameraController * get_cc() {
    return CAMERA_CONTROLLER;
}