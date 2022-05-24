// define macro that will enforce use of API functions.
// ....

#ifndef CGLOBALS_HEADER
#define CGLOBALS_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#define CNECTOR_INLINE __forceinline

#define CNECTOR_ALIGN(X) __declspec(align(X))

#define RL_DEFAULT_BATCH_BUFFER_ELEMENTS        8192
#define RL_DEFAULT_BATCH_BUFFERS                1

#define THREAD_COUNT 4

#define _SECOND 10000000
#define _MILSECOND 10000

// is this gonna work???
#define _WINDOW_TITLE               "cnector!"
#define _WINDOW_TITLE_MAX_LENGTH    200

#define _SCREEN_WIDTH   1920
#define _SCREEN_HEIGHT  1080

#define _SIM_STEP       (1.0 / 60.0) // in seconds

/*
extern float            B3_SIMSTEP;

extern int              SCREEN_WIDTH;
extern int              SCREEN_HEIGHT;

// try to allocate memory on heap on initialization here. (200 units)
extern char*             WINDOW_TITLE;
extern unsigned int      WINDOW_TITLE_MAX_LENGTH;
*/

#ifdef __cplusplus
}
#endif


#endif