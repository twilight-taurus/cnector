#include <cnector.h>


void CNInitialize() {

    // before all else that OpenGL related, we need to call InitWindow. (it initiates openGL)
    GLFWwindow * gl_win = InitWindow(_SCREEN_WIDTH, _SCREEN_HEIGHT, "cnector");

    if (gl_win == NULL) {
        printf("Failed to initialize window.\n");
        exit(1);
    } else {
        printf("Initialized window.\n");
    }

    glfwMakeContextCurrent(gl_win);

    // initialize GLEW
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to setup GLEW\n");
        exit(1);
    }

    // TODO: find a way to dynamically adjust Target FPS.
    SetTargetFPS(60);

    // state objects (camera, interaction, etc)
    init_state();

    init_threading();

    // initialize physics world object.
    init_physics();

    // asset loading/unloading
    init_assets();
    printf("Created Asset Loading!\n");
    init_shader();

    init_world();

    init_input();
    // init user/player control
    init_control();
    // first process interaction.
    init_interaction();
    // then camera can be adjusted accordingly.
    init_camera();

    // init nuklear
    const char* glsl_version = "#version 330";
    init_gui(gl_win);

    // finally initialize the renderer.
    init_render();

}

void CNDeinitialize() {

    deinit_input();
    deinit_interaction();
    deinit_control();
    deinit_camera();
    deinit_gui();

    deinit_render();

    deinit_world();

    deinit_shader();

    deinit_assets();

    deinit_physics();

    deinit_threading();

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    // Finally, deinitialize the global state structs/variables.
    deinit_state();
}

void CNRun() {

    // start up the threads. (including physics simulation.)
    run_threading();

    // this the loop from our main thread.
    // call rendering-related stuff only here.

    while(!WindowShouldClose()) {

        //////////////////////////////////////////
        // sync_ui and update will run until here...
        //////////////////////////////////////////

        WaitForMultipleObjectsEx((DWORD)2, hSUEvent, TRUE, (DWORD)60000, FALSE);

        // reset events so they can signal next pass.
        ResetEvent(hSUEvent[0]);
        ResetEvent(hSUEvent[1]);

        /*
            begin with rendering related stuff before SetEvent, 
            in the case that process thread may not start before that.
        */

        SetEvent(hRenderThenProcessEvent);  // process thread sets hRPEvent[1]

        /*
            rendering continues here after process thread has begun.
        */

        // draws everything to the frame buffer, and flips it.
        render();
        // rendering complete

        SetEvent(hRPEvent[0]); // set rendering complete event.

//        printf("Starting update_gui!\n");
//        printf("Finished update_gui!\n");
        /*
            sync_ui will start once both hRPEvent handles have signaled,
            and subsequently signal update thread to continue.
            - it will then reset both event handles.
        */
    }

}