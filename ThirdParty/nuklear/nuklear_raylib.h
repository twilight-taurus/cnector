#include <nuklear.h>

#ifndef NUKLEAR_RAYLIB_H
#define NUKLEAR_RAYLIB_H

/*
    https://github.com/RobLoach/raylib-nuklear/blob/master/include/raylib-nuklear.h
    -- 16.02.2022
*/

#ifndef RAYLIB_NUKLEAR_DEFAULT_FONTSIZE
/**
 * The default font size that is used when a font size is not provided.
 */
#define RAYLIB_NUKLEAR_DEFAULT_FONTSIZE 10
#endif  // RAYLIB_NUKLEAR_DEFAULT_FONTSIZE


NK_API struct nk_context*
InitNuklearContext(struct nk_user_font* userFont)
{
    struct nk_context* ctx = (struct nk_context*)MemAlloc(sizeof(struct nk_context));

    // Clipboard
    ctx->clip.copy = nk_raylib_clipboard_copy;
    ctx->clip.paste = nk_raylib_clipboard_paste;
    ctx->clip.userdata = nk_handle_ptr(0);

    // Create the nuklear environment.
    if (nk_init_default(ctx, userFont) == 0) {
        TraceLog(LOG_ERROR, "NUKLEAR: Failed to initialize nuklear");
        return NULL;
    }
    TraceLog(LOG_INFO, "NUKLEAR: Initialized GUI");

    return ctx;
}

NK_API nk_context*
InitNuklearDefault() {
    // User font.
    struct nk_user_font* userFont = (struct nk_user_font*)MemAlloc(sizeof(struct nk_user_font));

    // Use the default font size if desired.
    if (fontSize <= 0) {
        fontSize = RAYLIB_NUKLEAR_DEFAULT_FONTSIZE;
    }

    userFont->height = (float)fontSize;
    userFont->width = nk_raylib_font_get_text_width;
    userFont->userdata = nk_handle_ptr(0);

    // Nuklear context.
    return InitNuklearContext(userFont);
}

NK_API nk_context*
InitNuklear(Font font) {
    // Copy the font to a new raylib font pointer.
    struct Font* newFont = (struct Font*)MemAlloc(sizeof(struct Font));

    newFont->baseSize = font.baseSize;
    newFont->glyphCount = font.glyphCount;
    newFont->glyphPadding = font.glyphPadding;
    newFont->glyphs = font.glyphs;
    newFont->recs = font.recs;
    newFont->texture = font.texture;

    // Create the nuklear user font.
    struct nk_user_font* userFont = (struct nk_user_font*)MemAlloc(sizeof(struct nk_user_font));
    userFont->userdata = nk_handle_ptr(newFont);
    userFont->height = (float)RAYLIB_NUKLEAR_DEFAULT_FONTSIZE;
    userFont->width = nk_raylib_font_get_text_width_user_font;

    // Nuklear context.
    return InitNuklearContext(userFont);
}

NK_API nk_context*
InitNuklearEx(Font font, float fontSize) {
    // Copy the font to a new raylib font pointer.
    struct Font* newFont = (struct Font*)MemAlloc(sizeof(struct Font));

    // Use the default font size if desired.
    if (fontSize <= 0.0f) {
        fontSize = (float)RAYLIB_NUKLEAR_DEFAULT_FONTSIZE;
    }
    newFont->baseSize = font.baseSize;
    newFont->glyphCount = font.glyphCount;
    newFont->glyphPadding = font.glyphPadding;
    newFont->glyphs = font.glyphs;
    newFont->recs = font.recs;
    newFont->texture = font.texture;

    // Create the nuklear user font.
    struct nk_user_font* userFont = (struct nk_user_font*)MemAlloc(sizeof(struct nk_user_font));
    userFont->userdata = nk_handle_ptr(newFont);
    userFont->height = fontSize;
    userFont->width = nk_raylib_font_get_text_width_user_font;

    // Nuklear context.
    return InitNuklearContext(userFont);
}

NK_API void
DeinitNuklear(struct nk_context * ctx) {

    struct nk_user_font* userFont;

    // Skip unloading if it's not set.
    if (ctx == NULL) {
        return;
    }

    // Unload the font.
    userFont = (struct nk_user_font*)ctx->style.font;
    if (userFont != NULL) {
        // Clear the raylib Font object.
        void* fontPtr = userFont->userdata.ptr;
        if (fontPtr != NULL) {
            MemFree(fontPtr);
        }

        // Clear the user font.
        MemFree(userFont);
        ctx->style.font = NULL;
    }

    // Unload the nuklear context.
    nk_free(ctx);
    TraceLog(LOG_INFO, "NUKLEAR: Unloaded GUI");
}

NK_API void
DrawNuklear(struct nk_context * ctx) {

}


#endif // RAYLIB_NUKLEAR_H