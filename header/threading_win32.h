#ifndef CTHREADING_WIN32_HEADER
#define CTHREADING_WIN32_HEADER

// only include this in windows systems
#if defined(_WIN32)


// headers available in mingw32.
#include <winnt.h>
#include <winbase.h>

//#include <process.h>
//#include <synchapi.h>
//#include <Windows.h>

#include "globals.h"

#include <user_control.h>
#include <interaction.h>
#include <camera.h>

#include <shader.h>

//#include <gui.h> 

#include <physics.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
    event handles accessible from outside.
*/


// hint: might just use WaitForMultipleObjects for the above instead.
extern HANDLE hRenderThenProcessEvent; // shared between render and process for micro synchronization.

// hint: might just use WaitForMultipleObjects for the above instead.
extern HANDLE hSyncThenUpdateEvent; // shared between sync_ui and update for micro synchronization.

// these to  used in main loop, signaled for the respective tuple of threads to start.
// -> use SignalObjectAndWait
extern HANDLE hSUEvent[2]; // when sync_ui and update have both completed a pass.
extern HANDLE hRPEvent[2]; // when render and process have both their completed a pass.

extern void     init_threading();

extern void     deinit_threading();

extern int      run_threading();

DWORD WINAPI    threaded_process(LPVOID lpData);

DWORD WINAPI    threaded_update(LPVOID lpData);

DWORD WINAPI    threaded_sync_ui(LPVOID lpData);


#ifdef __cplusplus
}
#endif


#endif // _WIN32

#endif // # CTHREADING_HEADER