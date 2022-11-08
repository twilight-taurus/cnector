#include <state.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


CWindowState * WINDOW_STATE;

CCameraState * CAMERA_STATE;

CControlState * CONTROL_STATE;

CInteractionState * INTERACTION_STATE;

CShaderState * SHADER_STATE;

CRenderState * RENDER_STATE;

//CGuiState * GUI_STATE;


void init_state() {
    WINDOW_STATE = malloc(sizeof(CWindowState));

    // init window state data.
    WINDOW_STATE->WINDOW_TITLE = malloc(_WINDOW_TITLE_MAX_LENGTH * sizeof(char));
    WINDOW_STATE->WINDOW_TITLE_LENGTH = sizeof(_WINDOW_TITLE);

    strncpy(
        WINDOW_STATE->WINDOW_TITLE,
        _WINDOW_TITLE,
        WINDOW_STATE->WINDOW_TITLE_LENGTH
    );
    /*
    strncpy_s(
        WINDOW_STATE->WINDOW_TITLE, 
        WINDOW_STATE->WINDOW_TITLE_LENGTH,
        _WINDOW_TITLE,
        _WINDOW_TITLE_MAX_LENGTH
    );
    */

    SHADER_STATE = malloc(sizeof(CShaderState));

    RENDER_STATE = malloc(sizeof(CRenderState));

    RENDER_STATE->SCREEN_WIDTH = _SCREEN_WIDTH;
    RENDER_STATE->SCREEN_HEIGHT = _SCREEN_HEIGHT;

    RENDER_STATE->VIEW_WIDTH = _SCREEN_WIDTH / 1.5;
    RENDER_STATE->VIEW_HEIGHT = _SCREEN_HEIGHT / 1.35;

    RENDER_STATE->VIEW_XPOS = RENDER_STATE->VIEW_WIDTH / 4;
    RENDER_STATE->VIEW_YPOS = RENDER_STATE->VIEW_HEIGHT / 3.25;

//    GUI_STATE = malloc(sizeof(CGuiState));

    // init control
    CONTROL_STATE = malloc( sizeof(CControlState) );
    CONTROL_STATE->object_id = 0; // ID of current user controlled object.
    CONTROL_STATE->current_mode = CONTROLMODE_TRAVEL;
    CONTROL_STATE->control_object = ((CPhysObject*)NULL);

    // init interaction
    INTERACTION_STATE = malloc( sizeof(CInteractionState) );
    INTERACTION_STATE->object_id = 0;
    INTERACTION_STATE->current_mode = INTERACTIONMODE_NONE;
    INTERACTION_STATE->bodies_affected = 0;
    INTERACTION_STATE->interaction_object = ((CPhysObject*)NULL);

    // init camera
    CAMERA_STATE = malloc(sizeof(CCameraState));
    Camera camera = { 0 };
    // HINT: up vector needs to make sense between camera position and target.
    camera.position = (Vector3){ 0.0f, 10.0f, 50.0f };
    camera.target = (Vector3){ 0.0f, 10.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // TODO: make sure the values are -only- copied, -not- stored as memory locations!
    CAMERA_STATE->up_vector[0] = camera.up.x;
    CAMERA_STATE->up_vector[1] = camera.up.y;
    CAMERA_STATE->up_vector[2] = camera.up.z;

    CAMERA_STATE->position[0] = camera.position.x;
    CAMERA_STATE->position[1] = camera.position.y;
    CAMERA_STATE->position[2] = camera.position.z;

    CAMERA_STATE->target[0] = camera.target.x;
    CAMERA_STATE->target[1] = camera.target.y;
    CAMERA_STATE->target[2] = camera.target.z;

    // get directional vector (forward vector). (non-absolute)
    CAMERA_STATE->direction[0] = camera.target.x - camera.position.x;
    CAMERA_STATE->direction[1] = camera.target.y - camera.position.y;
    CAMERA_STATE->direction[2] = camera.target.z - camera.position.z;

    glmc_vec3_normalize(CAMERA_STATE->direction);

    // get right vector by getting cross product up vector and direction vector. (x axis vector)
    glmc_vec3_cross(CAMERA_STATE->up_vector, CAMERA_STATE->direction, CAMERA_STATE->right);
    glmc_vec3_normalize(CAMERA_STATE->right);

    // get the up vector for current camera view.
    glmc_vec3_cross(CAMERA_STATE->direction, CAMERA_STATE->right, CAMERA_STATE->up_vector);
    glmc_vec3_normalize(CAMERA_STATE->up_vector);
    // set yaw for camera to point towards negative z-axis by default.
    CAMERA_STATE->yaw = -90.0f;

    CAMERA_STATE->lastX = 400;
    CAMERA_STATE->lastY = 300;

    // set default distance from an object to target and follow.
    // --> radius.
    CAMERA_STATE->distance = 50.0f;

    CAMERA_STATE->camera = camera;

    CAMERA_STATE->mode = CAMERAMODE_FIRSTPERSON;
    SetCameraMode(camera, CAMERA_FIRST_PERSON);

}

void deinit_state() {
    free(WINDOW_STATE->WINDOW_TITLE);

    free(INTERACTION_STATE);
    free(CAMERA_STATE);

//    free(GUI_STATE);

    free(SHADER_STATE);
    free(RENDER_STATE);
}

