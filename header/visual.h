// objects that are rendered in the world,
// but are not subject to the physics simulation.
// ( billboards, skybox, nametags, user interface, etc. ..)

#ifndef CVISUAL_H
#define CVISUAL_H

#include <object.h>

// base identifier for type of CVisualObject
typedef enum CVisualType {
    CVISUAL_GUI, // menu controls, etc
    CVISUAL_SCENE_3D, // perspective-based, scene-related visual objects. (world-based)
    CVISUAL_SCENE_2D // orthographic scene-related visual objects (name tags fx.)
} CVisualType;

typedef enum CGuiType {
    /*
        Raylib GUI controls.
    */

    // container/seperator controls
    CGUI_WindowBox,
    CGUI_GroupBox,
    CGUI_Line,
    CGUI_Panel,

    // basic controls
    CGUI_Label,
    CGUI_Button,
    CGUI_LabelButton,
    CGUI_Toggle,
    CGUI_ToggleGroup,
    CGUI_CheckBox,
    CGUI_ComboBox,
    CGUI_DropdownBox,
    CGUI_TextBox,
    CGUI_TextBoxMulti,
    CGUI_ValueBox,
    CGUI_Spinner,
    CGUI_Slider,
    CGUI_SliderBar,
    CGUI_ProgressBar,
    CGUI_StatusBar,
    CGUI_ScrollBar,
    CGUI_ScrollPanel,
    CGUI_DummyRec,
    CGUI_Grid,

    // Advance Controls
    CGUI_ListView,
    CGUI_ColorPicker,
    CGUI_MessageBox,
    CGUI_TextInputBox
} CGuiType;

typedef enum CScene3dType {
    CSCENE3D_SKYBOX,
    // ...
} CScene3dType;

typedef enum CScene2dType {
    CSCENE2D_Text, // nametags (usable for 3D)
    CSCENE2D_Sprite
    // ...
} CScene2dType;

typedef struct CVisualObjectCfg {
    CVisualType         visual_type;
} CVisualObjectCfg;


// base type for visual objects

typedef struct CVisualObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject: (here: VIRTUAL)
    CObjectType         object_type; // 4 bytes

    // added ////
    CVisualObjectCfg    visual_config;

    // ....
} CVisualObject;

// extended types ...

typedef struct CGuiObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject: (here: VIRTUAL)
    CObjectType         object_type; // 4 bytes

    // inherited ////
    CVisualObjectCfg    visual_config;

    // added
} CGuiObject;


typedef struct CScene2dObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject: (here: VIRTUAL)
    CObjectType         object_type; // 4 bytes

    // inherited ////
    CVisualObjectCfg    visual_config;

    // added
} CScene2dObject;


typedef struct CScene3dObject {
    // inherited from CObject:
    CObject_ID          object_id;   // 4 bytes
    // inherited from CObject: (here: VIRTUAL)
    CObjectType         object_type; // 4 bytes

    // inherited ////
    CVisualObjectCfg    visual_config;

    // added
} CScene3dObject;

#endif