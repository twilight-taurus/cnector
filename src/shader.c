#include "shader.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"


void init_shader() {

//    SHADER_STATE->shader_light = LoadShader("lighting.vs", "lighting.fs");

//    SHADER_STATE->shader_light = LoadShader(TextFormat("lighting.vs", 330), TextFormat("lighting.fs", 330) );
    

    // viewpos vector
//    int viewposLoc = GetShaderLocation(SHADER_STATE->shader_light, "viewPos");
    // Ambient light level (some basic lighting)
//    int ambientLoc = GetShaderLocation(SHADER_STATE->shader_light, "ambient");
//    SetShaderValue(SHADER_STATE->shader_light, ambientLoc, (float[4]){ 1.0f, 1.0f, 1.0f, 0.0f }, SHADER_UNIFORM_VEC4);

    // Using 4 point lights: gold, red, green and blue
/*
    SHADER_STATE->lights[0] = CreateLight(
        LIGHT_POINT,
        (Vector3){ 0.0, 30.0, 0.0 }, 
        (Vector3){ 0.0, -80.0, 0.0 },
        WHITE,
        SHADER_STATE->shader_light
    );
    SHADER_STATE->lights[1] = CreateLight(
        LIGHT_POINT,
        (Vector3){ 2.0, 6.0, 2.0 },
        (Vector3){ 2.0, 6.0, 2.0 },
        RED,
        SHADER_STATE->shader_light
    );
    SHADER_STATE->lights[2] = CreateLight(
        LIGHT_POINT,
        (Vector3){ -10.0, 30.0, 10.0 },
        (Vector3){ -10.0, 30.0, 10.0 },
        GREEN,
        SHADER_STATE->shader_light
    );
    SHADER_STATE->lights[3] = CreateLight(
        LIGHT_POINT,
        (Vector3){ 20.0, 80.0, -2.0 },
        (Vector3){ 20.0, 80.0, -2.0 },
        BLUE,
        SHADER_STATE->shader_light
    );
*/

//    SHADER_STATE->shader_phong = LoadShader("base_lighting.vs", "base_lighting.fs");
    SHADER_STATE->shader_phong = LoadShader("base_shading.vs", "base_shading.fs");

    printf("Loaded custom shaders.\n");
/*
    Color color_wh = WHITE;

    vec4 color_vec = {
        color_wh.r,
        color_wh.g,
        color_wh.b,
        color_wh.a
    };
*/
    // Get Phong shader uniforms
    SHADER_STATE->viewPosLoc = GetShaderLocation(SHADER_STATE->shader_phong, "viewPos");

    SHADER_STATE->lightPosLoc = GetShaderLocation(SHADER_STATE->shader_phong, "lightPos");

    SHADER_STATE->lightColLoc = GetShaderLocation(SHADER_STATE->shader_phong, "lightColor");

    SHADER_STATE->colDiffLoc = GetShaderLocation(SHADER_STATE->shader_phong, "colDiffLoc");

    float cameraPos[3] = {
        CAMERA_STATE->camera.position.x,
        CAMERA_STATE->camera.position.y,
        CAMERA_STATE->camera.position.z
    };

    float lightPos[3] = {
        0.0,
        30.0,
        0.0
    };

    float lightCol[4] = {
        1.0,
        1.0,
        1.0,
        0.7
    };

    float colDiff[4] = {
        1.0,
        0.0,
        1.0,
        1.0
    };

    SetShaderValue(SHADER_STATE->shader_phong, SHADER_STATE->lightColLoc, lightCol, SHADER_UNIFORM_VEC4);

    // Set Phong shading uniforms.
    SetShaderValue(SHADER_STATE->shader_phong, SHADER_STATE->viewPosLoc, cameraPos, SHADER_UNIFORM_VEC3);

    SetShaderValue(SHADER_STATE->shader_phong, SHADER_STATE->lightPosLoc, lightPos, SHADER_UNIFORM_VEC3);

    SetShaderValue(SHADER_STATE->shader_phong, SHADER_STATE->colDiffLoc, colDiff, SHADER_UNIFORM_VEC4);
}

void deinit_shader() {
//    UnloadShader(SHADER_STATE->shader_light);
    UnloadShader(SHADER_STATE->shader_phong);
}

inline void update_shader() {

/*
    UpdateLightValues(SHADER_STATE->shader_light, SHADER_STATE->lights[0]);
    UpdateLightValues(SHADER_STATE->shader_light, SHADER_STATE->lights[1]);
    UpdateLightValues(SHADER_STATE->shader_light, SHADER_STATE->lights[2]);
    UpdateLightValues(SHADER_STATE->shader_light, SHADER_STATE->lights[3]);
*/
    // Update the shader with the camera view vector (points towards { 0.0f, 0.0f, 0.0f })
    float cameraPos[3] = {
        CAMERA_STATE->camera.position.x,
        CAMERA_STATE->camera.position.y,
        CAMERA_STATE->camera.position.z
    };
/*
    float lightPos[3] = {
        0.0,
        70.0,
        0.0
    };

    float lightCol[4] = {
        0.7,
        0.7,
        0.7,
        1.0
    };
*/

//    SetShaderValue(SHADER_STATE->shader_phong, SHADER_STATE->lightColLoc, lightCol, SHADER_UNIFORM_VEC4);

    // Set Phong shading uniforms.
    SetShaderValue(SHADER_STATE->shader_phong, SHADER_STATE->viewPosLoc, cameraPos, SHADER_UNIFORM_VEC3);

//    SetShaderValue(SHADER_STATE->shader_phong, SHADER_STATE->lightPosLoc, lightPos, SHADER_UNIFORM_VEC3);

}