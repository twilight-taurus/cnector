#include <input.h>

#include <stdio.h>


InputState * INPUT_STATE;


void init_input(void) {
    SetExitKey(KEY_ESCAPE);

    INPUT_STATE = malloc(sizeof(InputState));
    INPUT_STATE->text_input = calloc(100, sizeof(char));
    printf("Allocated memory for text input!\n");

    INPUT_STATE->mode = INPUTMODE_CAMERA_AND_INTERACTION;

    INPUT_STATE->wheel_up = false;
    INPUT_STATE->wheel_down = false;
    INPUT_STATE->click_left = false;
    INPUT_STATE->click_right = false;
    INPUT_STATE->click_middle = false;

    // dont show cursor in app
//    HideCursor();
    // lock the cursor to the center of the window.
//    DisableCursor();

    return;
}

void deinit_input(void) {
    free(INPUT_STATE->text_input);
    free(INPUT_STATE);
}

InputState * get_input_state() {
    return INPUT_STATE;
}

void switch_input_mode(InputMode mode) {
    INPUT_STATE->mode = mode;
}

static inline void process_keys() {

    //GetKeyPressed()

    switch (INPUT_STATE->mode) {
        case INPUTMODE_CAMERA_AND_INTERACTION:
            // down
            if ( IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
                printf("W pressed, in standard mode!\n");
                INPUT_STATE->forward = true;
            // up
            } else /*if ( IsKeyReleased(KEY_W) || IsKeyReleased(KEY_UP) ) */{
                printf("W released, in standard mode!\n");
                INPUT_STATE->forward = false;
            }
            // ...
            if ( IsKeyPressed(KEY_S) ||  IsKeyPressed(KEY_DOWN) ) {
                INPUT_STATE->backward = true;
            } else /*if ( IsKeyReleased(KEY_S)  || IsKeyReleased(KEY_DOWN) )*/ {
                INPUT_STATE->backward = false;
            }

            if ( IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT) ) {
                INPUT_STATE->left = true;
            } else /*if ( IsKeyReleased(KEY_A) ||IsKeyReleased(KEY_LEFT) )*/ {
                INPUT_STATE->left = false;
            }
            
            if ( IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT) ) {
                INPUT_STATE->right = true;
            } else /*if ( IsKeyReleased(KEY_D) || IsKeyReleased(KEY_RIGHT) ) */ {
                INPUT_STATE->right = false;
            }

            if ( IsKeyPressed(KEY_Q) ) {
                INPUT_STATE->up = true;
            } else /*if ( IsKeyReleased(KEY_D) )*/ {
                INPUT_STATE->up = false;
            }

             if ( IsKeyPressed(KEY_E) ) {
                INPUT_STATE->down = true;
            } else /*if ( IsKeyReleased(KEY_E) )*/ {
                INPUT_STATE->down = false;
            }

/*
            if (IsKeyPressed(KEY_ENTER)) {
                INPUT_STATE->mode = INPUTMODE_TEXT;
                // sets the first 6 members of InputState (key directions) to false.
                // -> this way any current input instructions are
                memset( INPUT_STATE, false, 6*sizeof(bool) );

                printf("Input values after memset: \n");

                printf("%s\n", INPUT_STATE->forward ? "true" : "false");
                printf("%s\n", INPUT_STATE->backward ? "true" : "false");
                printf("%s\n", INPUT_STATE->left ? "true" : "false");
                printf("%s\n", INPUT_STATE->right ? "true" : "false");
                printf("%s\n", INPUT_STATE->up ? "true" : "false");
                printf("%s\n", INPUT_STATE->down ? "true" : "false");
            }
*/
            return;

        case INPUTMODE_MENU:
            return;
            // TODO: process keyboard shortcuts.
        case INPUTMODE_TEXT:
            // TODO: process chars.
            // int res = GetCharPressed();

            // check for input in text, and process.
            // otherwise do the below straight away.
/*
            if (IsKeyPressed(KEY_ENTER)) {
                INPUT_STATE->mode = INPUTMODE_CAMERA;

            }
*/
            return;

        default:
            return;
    }
}

inline void process_btns() {
    if ( IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ) {
        // get camera direction, then spawn a sphere and launch it in the camera direction.
        // specify the --mass-- of the object or the specific gravitional force on it (if possible)
        // --> so that it may fly in a straight line (mostly), and not immediately bounce off the ground.
    } else /*if ( IsMouseButtonReleased(MOUSE_BUTTON_LEFT) ) */{

    }

    if ( IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {

    } else if ( IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) ) {

    }

    if ( IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) ) {

    } else if ( IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE) ) {

    }

    return;
}

inline void process_mousemove() {
    return;
    // ...
}

inline void process_mousewheel() {
    float move = GetMouseWheelMove(); // Get mouse wheel movement Y

    if (move > 0.0) {
//        printf("Positive Mouse scroll offset Y: %.6f \n", move);
        INPUT_STATE->wheel_up = true;
    } else if (move < 0.0) {
//        printf("Negative Mouse scroll offset Y: %.6f \n", move);
        INPUT_STATE->wheel_down = true;
    } else {
        if (INPUT_STATE->wheel_up) {
            INPUT_STATE->wheel_up = false;
        }
        if (INPUT_STATE->wheel_down) {
            INPUT_STATE->wheel_down = false;
        }
    }
}

inline void process_chars() {
    return;
    // ...
}

inline void process_input() {
    process_keys();
    process_btns();
    process_mousemove();
    process_mousewheel();

    return;
}