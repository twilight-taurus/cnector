#include <gui.h>


#define NK_PRIVATE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT


#define NK_IMPLEMENTATION
#include <nuklear/nuklear.h>

#define NK_GLFW_GL4_IMPLEMENTATION
#include <nuklear/nuklear_glfw_gl4.h>

struct nk_panel edit_menu, file_menu;


inline void CGui_updateMenuWorld() {
    return;
}

inline void CGui_updateMenuFile() {
    return;
}

inline void CGui_updateMenuEdit() {
    return;
}

inline void CGui_updateMenu() {
    return;
}

// the tab showing modifiers and information on a particular object.
inline void CGui_updateModifiers() {
    return;
}

// the tab showing entity and component hierarchies in the world
inline void CGui_updateWorld() {
    return;
}

// tab showing Logged actions
inline void CGui_updateLogs() {
    return;
}

void init_gui(GLFWwindow * window) {

    // init nuklear.
    GUI_STATE->ctx = nk_glfw3_init(
        window, 
        NK_GLFW3_INSTALL_CALLBACKS,
        RL_DEFAULT_BATCH_BUFFERS,
        RL_DEFAULT_BATCH_BUFFER_ELEMENTS
    );

    printf("Starting font stash.\n");

    float fontSize = 12.0;
    GUI_STATE->font_cfg = nk_font_config(fontSize);

    nk_glfw3_font_stash_begin(&GUI_STATE->font_atlas);
    GUI_STATE->font_cfg.range = nk_font_default_glyph_ranges();
    GUI_STATE->default_font = nk_font_atlas_add_from_file(GUI_STATE->font_atlas, "DroidSans.ttf", fontSize, &GUI_STATE->font_cfg);
    nk_glfw3_font_stash_end();
    printf("Made it after glfw3 font stash!\n");

//    GUI_STATE->default_font = glfw.atlas.default_font;

    // set color state in color picker.
    GUI_STATE->bg_color.r = 0.10f;
    GUI_STATE->bg_color.g = 0.18f;
    GUI_STATE->bg_color.b = 0.24f;
    GUI_STATE->bg_color.a = 1.0f;

    // set transparency of window background
    GUI_STATE->ctx->style.window.fixed_background.data.color.a = (nk_byte)0;
    GUI_STATE->ctx->style.window.background.a = (nk_byte)255;

/*
    GUI_STATE->ctx->style.window.background.a = (nk_byte)0;
    GUI_STATE->ctx->style.window.border_color.a = (nk_byte)0;
    GUI_STATE->ctx->style.menu_button.active.data.color.a = (nk_byte)255;
    GUI_STATE->ctx->style.menu_button.normal.data.color.a = (nk_byte)255;

    GUI_STATE->ctx->style.menu_button.text_active.a = (nk_byte)255;
    GUI_STATE->ctx->style.menu_button.text_background.a = (nk_byte)0;

    GUI_STATE->ctx->style.menu_button.text_normal.r = (nk_byte)255;
    GUI_STATE->ctx->style.menu_button.text_normal.g = (nk_byte)255;
    GUI_STATE->ctx->style.menu_button.text_normal.b = (nk_byte)255;
    GUI_STATE->ctx->style.menu_button.text_normal.a = (nk_byte)255;

    GUI_STATE->ctx->style.text.color.r = (nk_byte)255;
    GUI_STATE->ctx->style.text.color.g = (nk_byte)255;
    GUI_STATE->ctx->style.text.color.b = (nk_byte)255;
    GUI_STATE->ctx->style.text.color.a = (nk_byte)255;
*/
    nk_style_set_font(GUI_STATE->ctx, &GUI_STATE->default_font->handle);

    printf("Made it after style set font (default font)!\n");
}

void deinit_gui() {
    nk_glfw3_shutdown();
}

void process_gui() {

//    printf("Beginning new gui frame!\n");
    nk_glfw3_new_frame();
}

void update_gui() {
    struct nk_context* ctx = GUI_STATE->ctx;

    if (
            nk_begin(
                ctx,
                "cnector gui",
                nk_rect(
                    0,
                    0,
                    (float)_SCREEN_WIDTH,
                    (float)_SCREEN_HEIGHT
                ),
                NK_WINDOW_TITLE|NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR //|NK_WINDOW_MOVABLE
            )
    ) {
        // base
        enum {EASY, HARD};
        static int op = EASY;
        static int property = 20;

//        printf("Beginning menu bar!\n");
        nk_menubar_begin(ctx);

        nk_layout_row_static(ctx, 40, 120, 3);

        if (nk_menu_begin_label(ctx, "FILE", NK_TEXT_CENTERED, nk_vec2(120, 250))) {

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_menu_item_label(ctx, "OPEN", NK_TEXT_LEFT);
            nk_menu_item_label(ctx, "CLOSE", NK_TEXT_LEFT);

            nk_menu_end(ctx);
        }
        if (nk_menu_begin_label(ctx, "EDIT", NK_TEXT_CENTERED, nk_vec2(120, 250))) {

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_menu_item_label(ctx, "COPY", NK_TEXT_CENTERED);
            nk_menu_item_label(ctx, "CUT", NK_TEXT_CENTERED);
            nk_menu_item_label(ctx, "PASTE", NK_TEXT_CENTERED);

            nk_menu_end(ctx);
        }
        if (nk_menu_begin_label(ctx, "WORLD", NK_TEXT_CENTERED, nk_vec2(120, 250))) {

            nk_layout_row_dynamic(ctx, 25, 1);

            nk_menu_item_label(ctx, "CREATE SPHERE", NK_TEXT_CENTERED);
            nk_menu_item_label(ctx, "CREATE CUBE", NK_TEXT_CENTERED);

            if ( nk_menu_item_label(ctx, "RESUME/STOP PHYSICS", NK_TEXT_CENTERED) ) {
                PHYSICS_STATE->simulation = PHYSICS_STATE->simulation ? false : true;
            }

            nk_menu_end(ctx);
        }
        nk_menubar_end(ctx);

        nk_layout_row_dynamic(ctx, 30, 3);

        // NOTE: combo_begin_color appropriates one of the columns specified in the call to nk_layout_row_dynamic above
        if (
            nk_combo_begin_color(ctx, nk_rgb_cf(GUI_STATE->bg_color), nk_vec2(nk_widget_width(ctx),400))
        )
        {
            nk_layout_row_dynamic(ctx, 220, 1);

            // NOTE: nk_color_picker appropriates one column specified in the call to nk_layout_row_dynamic above
            GUI_STATE->bg_color = nk_color_picker(ctx, GUI_STATE->bg_color, NK_RGBA);

//            nk_label(GUI_STATE->ctx, "DOES THIS WORK?", NK_TEXT_ALIGN_MIDDLE);

/*
            nk_layout_row_dynamic(GUI_STATE->ctx, 25, 1);
            GUI_STATE->bg_color.r = nk_propertyf(GUI_STATE->ctx, "#R:", 0, GUI_STATE->bg_color.r, 1.0f, 0.01f,0.005f);
            GUI_STATE->bg_color.g = nk_propertyf(GUI_STATE->ctx, "#G:", 0, GUI_STATE->bg_color.g, 1.0f, 0.01f,0.005f);
            GUI_STATE->bg_color.b = nk_propertyf(GUI_STATE->ctx, "#B:", 0, GUI_STATE->bg_color.b, 1.0f, 0.01f,0.005f);
            GUI_STATE->bg_color.a = nk_propertyf(GUI_STATE->ctx, "#A:", 0, GUI_STATE->bg_color.a, 1.0f, 0.01f,0.005f);
*/
            nk_combo_end(ctx);
        }

//        struct nk_color color_l = {255, 255, 255, 255};
//        nk_label_colored(ctx, "Hello!", NK_TEXT_ALIGN_CENTERED, color_l);

//        nk_label(ctx, "Hello!", NK_TEXT_ALIGN_RIGHT); // empty column

        if (
            nk_combo_begin_color(ctx, nk_rgb_cf(GUI_STATE->bg_color), nk_vec2(nk_widget_width(ctx),400))
        )
        {
            nk_layout_row_dynamic(ctx, 220, 1);

            // NOTE: nk_color_picker appropriates one column specified in the call to nk_layout_row_dynamic above
            GUI_STATE->bg_color = nk_color_picker(ctx, GUI_STATE->bg_color, NK_RGBA);

/*
            nk_layout_row_dynamic(GUI_STATE->ctx, 25, 1);
            GUI_STATE->bg_color.r = nk_propertyf(GUI_STATE->ctx, "#R:", 0, GUI_STATE->bg_color.r, 1.0f, 0.01f,0.005f);
            GUI_STATE->bg_color.g = nk_propertyf(GUI_STATE->ctx, "#G:", 0, GUI_STATE->bg_color.g, 1.0f, 0.01f,0.005f);
            GUI_STATE->bg_color.b = nk_propertyf(GUI_STATE->ctx, "#B:", 0, GUI_STATE->bg_color.b, 1.0f, 0.01f,0.005f);
            GUI_STATE->bg_color.a = nk_propertyf(GUI_STATE->ctx, "#A:", 0, GUI_STATE->bg_color.a, 1.0f, 0.01f,0.005f);
*/
            nk_combo_end(ctx);
        }

//        if (nk_option_label(GUI_STATE->ctx, "easy", op == EASY)) op = EASY;

//        if (nk_option_label(GUI_STATE->ctx, "hard", op == HARD)) op = HARD;

//        nk_label(GUI_STATE->ctx, "background:", NK_TEXT_LEFT);

/*
        nk_layout_row_dynamic(GUI_STATE->ctx, 10, 1);
        if (nk_button_label(GUI_STATE->ctx, "button"))
            fprintf(stdout, "button pressed\n");

        nk_layout_row_dynamic(GUI_STATE->ctx, 10, 2);
        if (nk_option_label(GUI_STATE->ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(GUI_STATE->ctx, "hard", op == HARD)) op = HARD;

        nk_layout_row_dynamic(GUI_STATE->ctx, 10, 1);
        nk_property_int(GUI_STATE->ctx, "Compression:", 0, &property, 100, 10, 1);

        nk_layout_row_dynamic(GUI_STATE->ctx, 10, 1);
        nk_label(GUI_STATE->ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(GUI_STATE->ctx, 10, 1);

        if (
            nk_combo_begin_color(GUI_STATE->ctx, nk_rgb_cf(GUI_STATE->bg_color), nk_vec2(nk_widget_width(GUI_STATE->ctx),400))
        )
        {
            nk_layout_row_dynamic(GUI_STATE->ctx, 120, 1);
            // TODO: call this only if user has made a selection.
            GUI_STATE->bg_color = nk_color_picker(GUI_STATE->ctx, GUI_STATE->bg_color, NK_RGBA);

            nk_layout_row_dynamic(GUI_STATE->ctx, 25, 1);
            GUI_STATE->bg_color.r = nk_propertyf(GUI_STATE->ctx, "#R:", 0, GUI_STATE->bg_color.r, 1.0f, 0.01f,0.005f);
            GUI_STATE->bg_color.g = nk_propertyf(GUI_STATE->ctx, "#G:", 0, GUI_STATE->bg_color.g, 1.0f, 0.01f,0.005f);
            GUI_STATE->bg_color.b = nk_propertyf(GUI_STATE->ctx, "#B:", 0, GUI_STATE->bg_color.b, 1.0f, 0.01f,0.005f);
            GUI_STATE->bg_color.a = nk_propertyf(GUI_STATE->ctx, "#A:", 0, GUI_STATE->bg_color.a, 1.0f, 0.01f,0.005f);

            nk_combo_end(GUI_STATE->ctx);
        }
    }
*/
    } else {
        printf("Failed nk_begin. Quitting program.\n");
        exit(1);
    }
    nk_end(ctx);
}

void draw_gui() {
    nk_glfw3_render_custom(NK_ANTI_ALIASING_OFF);
}