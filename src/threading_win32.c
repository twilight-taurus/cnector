#include "threading_win32.h"


HANDLE hRenderThenProcessEvent      = NULL;

HANDLE hSyncThenUpdateEvent         = NULL;

HANDLE hSUEvent[2]                  = {NULL, NULL};

HANDLE hRPEvent[2]                  = {NULL, NULL};


// static here means, fields will only be visible inside the object file.
static HANDLE g_hRPThreads[THREAD_COUNT / 2];
static HANDLE g_hUSThreads[THREAD_COUNT / 2];

static LPDWORD threadIDs[THREAD_COUNT];


void init_threading() {

    hRenderThenProcessEvent = CreateEventW(
        NULL,
        TRUE,
        FALSE,
        L"RenderThenProcessEvent"
    );

    hSyncThenUpdateEvent = CreateEventW(
        NULL,
        TRUE,
        FALSE,
        L"SyncThenUpdateEvent"
    );

    hSUEvent[0] = CreateEventW(
        NULL,
        TRUE,
        FALSE,
        L"SyncEvent"
    );

    hSUEvent[1] = CreateEventW(
        NULL,
        TRUE,
        FALSE,
        L"UpdateEvent"
    );

    hRPEvent[0] = CreateEventW(
        NULL,
        TRUE,
        FALSE,                   // this will make the update and sync run first.
                                // which then triggers the following events. (render and process)
        L"RenderEvent"
    );

    hRPEvent[1] = CreateEventW(
        NULL,
        TRUE,
        FALSE,                   // this will make the update and sync run first.
                                // which then triggers the following events. (render and process)
        L"ProcessEvent"
    );

    printf("Created Events!\n");
}

void deinit_threading() {
    return;
}

DWORD WINAPI threaded_process(LPVOID lpData) {
    // retrieve sphere dimensions here for drawing.
    printf("Starting threaded process!\n");

    // add condition of some sort that checks whether a mutex has been freed.
    while (1) {
        // wait until render has gotten copy of some data before processing the referenced data in parallel here.
        WaitForSingleObject(hRenderThenProcessEvent, 60000);
//        printf("Running threaded process!\n");

        // process user input
        process_input();
//        printf("processed input!\n");

        process_camera();
//        printf("processed camera!\n");

        // process object interaction after user user input.
//        process_interaction();
        process_control();
//        printf("processed control!\n");

        // process simulation
        process_physics();

//        printf("simulation step done!\n");
        process_gui();
//        printf("processed gui!\n");
        // adjust camera values AFTER simulation step has completed.

        // reset event immediately to unsignaled state for next pass.
        ResetEvent(hRenderThenProcessEvent);
        // set this in main loop.
        SetEvent(hRPEvent[1]);
    }

    return 0;
}


DWORD WINAPI threaded_update(LPVOID lpData) {
    printf("Starting threaded update!\n");

    while(1) {
        // rename to hSyncBeforeUpdateEvent
        WaitForSingleObject(hSyncThenUpdateEvent, 60000);

//        printf("Running threaded update!\n");

        /*

            upload buffer memory to gpu, etc.

        */

       // TODO: retrieve position of sphere object
//        printf("Retrieving transform in threaded update!\n");
        update_physics();

        update_camera();

        update_shader();

        update_gui();

        // at the end of this loop.
        ResetEvent(hSyncThenUpdateEvent);

        // update done. signal this to render()
        SetEvent(hSUEvent[1]);
    }
    // ...
}


DWORD WINAPI threaded_sync_ui(LPVOID lpData) {
    // cast lpData here to desired type.
    // ...
    printf("Starting threaded sync ui!\n");

    while(1) {
        // HINT: alternatively use WaitForSingleObject sequentially for more granular control over the threads.
        WaitForMultipleObjectsEx((DWORD)2, hRPEvent, TRUE, (DWORD)60000, FALSE);

//        printf("Running threaded sync ui!\n");
        /*
            sync components here that need update thread to wait before it can start its pass.
        */

        // -> this will trigger the update pass.
        SetEvent(hSyncThenUpdateEvent);

        /*
            do rest of synchronization here, that is not dependent on update thread.
        */

        // use some kind of mutex so that sync or update are not called repeatedly after this.

        ResetEvent(hRPEvent[0]);
        ResetEvent(hRPEvent[1]);

        // sync_ui done. this event signals this to main thread.
        SetEvent(hSUEvent[0]);
    }
}


int run_threading() {

    g_hUSThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threaded_sync_ui, (LPVOID)0, (DWORD)NULL, threadIDs[3]);
    g_hUSThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threaded_update, (LPVOID)0, (DWORD)NULL, threadIDs[2]);
    g_hRPThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threaded_process, (LPVOID)0, (DWORD)NULL, threadIDs[1]);

    // initialize the multi-threaded loop, by launching dummy events.
    SetEvent(hRPEvent[0]);
    SetEvent(hRPEvent[1]);

    return 1;
}