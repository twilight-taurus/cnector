cmake_minimum_required(VERSION 2.4.3)

set(PROJECT_NAME cnector)

project (${PROJECT_NAME} LANGUAGES C CXX)


set(PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(OPENGL_VERSION 3.3)
###############
# find_package
#necessary? X11, Winnt, libdev etc.
# If not found --> inform user.
###############

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )

#set(RAYLIB_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/raylib)
#set(BULLET_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/bullet3)
#set(CGLM_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/cglm)
#set(GLEW_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/glew)

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/raylib)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/bullet3)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/cglm)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glew/build/cmake)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/header
    ${CMAKE_CURRENT_SOURCE_DIR}/header/api/bullet3
#    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty

    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/raylib/src
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/raylib/src/external/glfw/include

    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/bullet3/src
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/cglm/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glew/include

    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/STC/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/nuklear
)

# add Bullet3Collision to main target.
#add_library(Bullet3CollisionLib STATIC IMPORTED)
#set_property(TARGET Bullet3CollisionLib PROPERTY IMPORTED_LOCATION ${LIB_OUTPUT}/libBullet3Collision.a)

# add Bullet3Common to main target.
#add_library(Bullet3CommonLib STATIC IMPORTED)
#set_property(TARGET Bullet3CommonLib PROPERTY IMPORTED_LOCATION ${LIB_OUTPUT}/libBullet3Common.a)

# add Bullet3Dynamics to main target.
#add_library(Bullet3DynamicsLib STATIC IMPORTED)
#set_property(TARGET Bullet3DynamicsLib PROPERTY IMPORTED_LOCATION ${LIB_OUTPUT}/libBullet3Dynamics.a)

# add Bullet3Geometry to main target.
#add_library(Bullet3GeometryLib STATIC IMPORTED)
#set_property(TARGET Bullet3GeometryLib PROPERTY IMPORTED_LOCATION ${LIB_OUTPUT}/libBullet3Geometry.a)

# add Bullet3Serialize to main target.
#add_library(Bullet3SerializeLib STATIC IMPORTED)
#set_property(TARGET Bullet3SerializeLib PROPERTY IMPORTED_LOCATION ${LIB_OUTPUT}/libBullet3Serialize.a)

# add BulletCollision to main target.
#add_library(BulletCollisionLib STATIC IMPORTED)
#set_property(TARGET BulletCollisionLib PROPERTY IMPORTED_LOCATION ${LIB_OUTPUT}/libBulletCollision.a)

# add BulletDynamics to main target.
#add_library(BulletDynamicsLib STATIC IMPORTED)
#set_property(TARGET BulletDynamicsLib PROPERTY IMPORTED_LOCATION ${LIB_OUTPUT}/libBulletDynamics.a)

# add BulletInverseDynamics to main target.
#add_library(BulletInverseDynamicsLib STATIC IMPORTED)
#set_property(TARGET BulletInverseDynamicsLib PROPERTY IMPORTED_LOCATION ${LIB_OUTPUT}/libBulletInverseDynamics.a)

# add LinearMath to main target.
#add_library(LinearMathLib STATIC IMPORTED)
#set_property(TARGET LinearMathLib PROPERTY IMPORTED_LOCATION ${LIB_OUTPUT}/libLinearMathLib.a)

# add LinearMath to main target.
#add_library(Raylib STATIC IMPORTED)
#set_property(TARGET Raylib PROPERTY IMPORTED_LOCATION ${LIB_OUTPUT}/libraylib.a)

# add cglm static lib to main target.
#add_library(libCglm STATIC IMPORTED)
#set_property(TARGET libCglm PROPERTY IMPORTED_LOCATION ${LIB_OUTPUT}/libcglm.dll.a)


add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}/main.c

    ${PROJECT_SOURCES}/api/bulletC_api.cpp

    ${PROJECT_SOURCES}/assets.c
    ${PROJECT_SOURCES}/camera.c
    ${PROJECT_SOURCES}/globals.c
    #${PROJECT_SOURCES}/gui.c
    ${PROJECT_SOURCES}/input.c
    ${PROJECT_SOURCES}/interaction.c
    ${PROJECT_SOURCES}/model.c
    ${PROJECT_SOURCES}/physical.c
    ${PROJECT_SOURCES}/physics.c
    ${PROJECT_SOURCES}/render.c
    ${PROJECT_SOURCES}/shader.c
    ${PROJECT_SOURCES}/state.c
    ${PROJECT_SOURCES}/threading_win32.c # if linux, threading_pthreads
    ${PROJECT_SOURCES}/user_control.c
    ${PROJECT_SOURCES}/world.c
    ${PROJECT_SOURCES}/cnector.c
)

# link all libraries to final target seperately
# use the name of the folder containing the CMakeLists.txt that you want to add as library.
### IMPORTANT HINT ###
# --> Make sure of CORRECT order of libraries. Otherwise 'undefined' error will appear.
######################
target_link_libraries(${PROJECT_NAME}
    #raylib
    raylib_static

    Bullet3Dynamics
    Bullet3Collision
    Bullet2FileLoader
    Bullet3Common
    
    BulletDynamics
    BulletCollision
    BulletInverseDynamics
    LinearMath

    cglm
    glew_s # static library (glew without s is shared library)

    #TODO: link depending on OS
    # windows-specific
    libopengl32.a
    libmsvcrt.a
    libgdi32.a
    libwinmm.a
    libcrtdll.a
    libuser32.a
    libkernel32.a

)
##### those should be added.
# -lopengl32 -lgdi32 -lwinmm -lglew32.dll -luser32 -lkernel32
#####

if(FALSE) # fake a block comment

add_dependencies(${PROJECT_NAME}
    RaylibLib
    CglmLib
    Bullet3CollisionLib
    Bullet3CommonLib
    Bullet3DynamicsLib
    Bullet3SerializeLib
    BulletCollisionLib
    BulletDynamicsLib
    BulletInverseDynamicsLib
    LinearMathLib
)

endif()

# restore repository to state of last commit, and remove all files that are untracked,
# except those that are "ignored"
# git reset --hard && git clean -fd

# add lib path build/lib

# add main src/ directory.
# add include paths include\gl and ThirdParty/

# specify binary output path ../bin/ and copy .dll/.so (Shared libs) files there. (post-build)