#include <stdio.h>

#include <render.h>

#include <stdlib.h>

#include "raymath.h"


int init_render() {


    // can be done on processing thread partially. binding ressources may need to be done on main thread.
//    Image img = LoadImage("terrain_textured.png");
/*
    // load terrain model obj file.
    RENDER_STATE->terrain_model.model = LoadModel("terrain_textured.obj");

    if (RENDER_STATE->terrain_model.model.meshCount <= 0) {
        printf("Failed to load model.\n");
    } else {
        printf("Number of model meshes: %d.\n", RENDER_STATE->terrain_model.model.meshCount);
    }
*/
//    Matrix terrain_transform = RENDER_STATE->terrain_model.model.transform;

//    RENDER_STATE->terrain_model.model.transform = MatrixRotateZ(210.0f);
//    RENDER_STATE->terrain_model.model.transform = MatrixRotateX(PI / 2.0);
/*
    if (img.data == NULL) {
        printf("Failed to load image.\n");
    } else {
        printf("image load success.\n");
    }

    RENDER_STATE->render_texture = LoadTextureFromImage(img);
    if (RENDER_STATE->render_texture.id <= 0) {
        printf("Failed to load texture.\n");
    } else {
        printf("tex load success.\n");
    }

    UnloadImage(img);
*/
    printf("Initialized renderer.\n");

    return 1;
}

int deinit_render() {
    
    return 1;
}

void render() {

    BeginDrawing();

        // set world view frame.
        glViewport(
            RENDER_STATE->VIEW_XPOS,
            RENDER_STATE->VIEW_YPOS, 
            RENDER_STATE->VIEW_WIDTH, 
            RENDER_STATE->VIEW_HEIGHT
        );

        ClearBackground( RAYWHITE );

//            DrawText("free fonts included with raylib", 500, 500, 20, RED);

        BeginMode3D(CAMERA_STATE->camera);
/*
            DrawTexture(
                RENDER_STATE->render_texture,
                RENDER_STATE->SCREEN_WIDTH/2 - RENDER_STATE->render_texture.width/2,
                RENDER_STATE->SCREEN_HEIGHT/2 - RENDER_STATE->render_texture.height/2, 
                WHITE
            );
*/
            DrawGrid(10, 1.0);
//                printf("Made it after DrawGrid.\n");

            // TODO: replace with actual plane data from physics engine.
//            DrawPlane( (Vector3){0.0, 0.0, 0.0}, (Vector2){50, 50}, (Color){128, 0, 128, 255} );
//            DrawModel(model_mesh, (Vector3){0.0, 5.0, 0.0}, 1.0f, WHITE);
//            DrawModel(cube_mesh, (Vector3){0.0, 10.0, 0.0}, 1.0f, WHITE);
            // Draw terrain model.
            // TODO: add physics collision object. (triangle meshes)
//            DrawModel( RENDER_STATE->terrain_model.model, terrain_pos, 1.0, WHITE );

            // TODO: DrawSphere and use data from physics engine. (DONE)
//            DrawModel(sphere_mesh, vector3_sphere, 1.0, WHITE);

            // draw models
            for (size_t i = 0; i < cvec_cobject_size(WORLD_STATE->cobjects); ++i) {

                CObject object = WORLD_STATE->cobjects.data[i];

                switch(object.base.object_type) {
                    case OBJECT_TYPE_PHYSICAL:
                        switch(object.physical.info.base_info.physics_type) {
                            case CPHYS_SPHERE:
                                Vector3 loc_sphere = (Vector3){
                                    object.sphere.info.sphere_info.world_transform[3][0],
                                    object.sphere.info.sphere_info.world_transform[3][1],
                                    object.sphere.info.sphere_info.world_transform[3][2]
                                };

                                DrawModel(*object.sphere.model, loc_sphere, 1.0, WHITE);

                                break;
                            case CPHYS_TERRAIN:
                                Vector3 loc_terrain = (Vector3){
                                    object.terrain.info.terrain_info.world_transform[3][0],
                                    object.terrain.info.terrain_info.world_transform[3][1],
                                    object.terrain.info.terrain_info.world_transform[3][2]
                                };

                                DrawModel(*object.terrain.model, loc_terrain, 1.0, WHITE);

                                break;
                            case CPHYS_PLANE:
                                Vector3 loc_plane = (Vector3){
                                    object.plane.info.plane_info.world_transform[3][0],
                                    object.plane.info.plane_info.world_transform[3][1],
                                    object.plane.info.plane_info.world_transform[3][2]
                                };

                                DrawModel(*object.plane.model, loc_plane, 1.0, BLUE);

                                break;
                            default:
                                break;
                        }
                    default:
                        break;
                }
            }
/*
            // Draw markers to show where the lights are
            if ( SHADER_STATE->lights[0].enabled ) {
                DrawSphereEx(SHADER_STATE->lights[0].position, 0.2f, 8, 8, YELLOW);
            } else {
                DrawSphereWires(SHADER_STATE->lights[0].position, 0.2f, 8, 8, ColorAlpha(YELLOW, 0.3f));
            }
            if (SHADER_STATE->lights[1].enabled) {
                DrawSphereEx(SHADER_STATE->lights[1].position, 0.2f, 8, 8, RED);
            } else {
                DrawSphereWires(SHADER_STATE->lights[1].position, 0.2f, 8, 8, ColorAlpha(RED, 0.3f));
            }

            if (SHADER_STATE->lights[2].enabled) {
                DrawSphereEx(SHADER_STATE->lights[2].position, 0.2f, 8, 8, GREEN);
            } else {
                DrawSphereWires(SHADER_STATE->lights[2].position, 0.2f, 8, 8, ColorAlpha(GREEN, 0.3f));
            }

            if (SHADER_STATE->lights[3].enabled) {
                DrawSphereEx(SHADER_STATE->lights[3].position, 0.2f, 8, 8, BLUE);
            } else {
                DrawSphereWires(SHADER_STATE->lights[3].position, 0.2f, 8, 8, ColorAlpha(BLUE, 0.3f));
            }
*/

//                printf("Made it after draw spheres.\n");

            // draw the physics debug world.
//            b3World_debugDrawWorld();

        EndMode3D();

//            printf("Starting draw_gui!\n");
        draw_gui();
//            printf("Finished draw_gui!\n");
        // Deactivated raylib font/text, so this will not work.
        // (deactivated, because conflict with Nuklear UI, which both use stb font)
        // --> Font is available through nuklear.
        DrawFPS(10, 10);

    // current time since start.
    EndDrawing();
}

CNECTOR_INLINE void render_gui() {
    return;
}

CNECTOR_INLINE void render_scene() {
    return;
}

CNECTOR_INLINE void post_sync_ui() {
    return;
}