#include <cnector.h>

#include <stdio.h>
#include <stdlib.h>

#include <time.h>


inline int run_singlethreaded() {

    process_input();

    process_camera();

    process_control();

    process_physics();

    update_physics();

    update_camera();

    update_shader();

    render();
}

inline int run_multithreaded() {

    // draws everything to the frame buffer, and flips it.
    render();

    #if defined(CTHREADING_WIN32_HEADER)
        // rendering complete
        SetEvent(hRPEvent[0]); // render complete event.

        // signal threaded sync_ui to commence, which in turns signals threaded update,
        // once both threaded process and render complete.

        WaitForMultipleObjectsEx((DWORD)2, hSUEvent, TRUE, (DWORD)60000, FALSE);

        // reset events so they can signal next pass.
        ResetEvent(hSUEvent[0]);
        ResetEvent(hSUEvent[1]);

        // threaded_process starts alongside render()
        // if we set this before calling render(), both can run simultaneously.
        SetEvent(hRenderThenProcessEvent);  // process thread sets hRPEvent[1]

    #elif defined(CTHREADING_HEADER)
        // linux, macos threading stuff.
    #endif

    /*
        begin with rendering related stuff before SetEvent(hRPEvent[0]), 
        in the case that process thread may not start before that.
    */

    /*
        rendering continues here after process thread has begun.
    */

}

void CNInitialize() {
    //#define GLEW_OK 0

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

    // Change working directory to app directory. (so ressources can be accessed, etc.)
//    const char * current_wd = GetApplicationDirectory();
//    ChangeDirectory(current_wd);

    // TODO: find a way to dynamically adjust Target FPS.
    SetTargetFPS(60);

    // state objects (camera, interaction, etc)
    init_state();

    #ifndef CN_DISABLE_MULTITHREADING
        init_threading();
    #endif

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
//    init_gui(gl_win);

    // finally initialize the renderer.
    init_render();

}

void CNDeinitialize() {

    deinit_input();
    deinit_interaction();
    deinit_control();
    deinit_camera();
//    deinit_gui();

    deinit_render();

    deinit_world();

    deinit_shader();

    deinit_assets();

    deinit_physics();

    #ifndef CN_DISABLE_MULTITHREADING
        deinit_threading();
    #endif
    //--------------------------------------------------------------------------------------
    RCloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    // Finally, deinitialize the global state structs/variables.
    deinit_state();
}

void CNRun() {

    // start up the threads. (including physics simulation.)
    #ifndef CN_DISABLE_MULTITHREADING
        start_threading();
    #endif

    // this the loop from our main thread.
    // call rendering-related stuff only here.

    while( !WindowShouldClose() ) {
/*
        process_input();

        process_camera();

        process_control();

        process_physics();

        update_physics();

        update_camera();

        update_shader();
*/
        //////////////////////////////////////////
        // sync_ui and update will run until here...
        //////////////////////////////////////////

        ////////
        // TODO: this is win32 only. make sure to put the below functionality into threading_win32.c
        ///////
        #ifndef CN_DISABLE_MULTITHREADING
            run_multithreaded();
        #else
            run_singlethreaded();
        #endif

//        printf("Starting update_gui!\n");
//        printf("Finished update_gui!\n");
        /*
            sync_ui will start once both hRPEvent handles have signaled,
            and subsequently signal update thread to continue.
            - it will then reset both event handles.
        */
    }

}