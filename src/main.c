/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

// top-level include for the game library
#include <cnector.h>



int main()
{
    // Initialization of Cnector
    //--------------------------------------------------------------------------------------
    CNInitialize();

    // first object: player-controlled sphere.
    CObject object = WORLD_STATE->cobjects.data[0];
    assign_control(&WORLD_STATE->cobjects.data[0].physical);


    switch(object.physical.info.base_info.physics_type) {
        case CPHYS_SPHERE:
            printf("Found sphere\n");
            break;
        default:
            printf("No sphere found in cvector\n");
            break;
    }

    CNRun();

    // De-Initialization
    CNDeinitialize();

    return 0;
}

/*
///////// for Cimgui. ////////////////
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
/////////////////////////////////////
*/