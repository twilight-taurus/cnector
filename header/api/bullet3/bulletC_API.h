#ifndef BULLET_C_API
#define BULLET_C_API

#include <cglm/vec3.h>
#include <cglm/mat4.h>

#include <model.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int b3CollisionShapeType;

////////////////////////////////////////////
// forward struct and class declarations //
////////////////////////////////////////////

// enumerates the collision types.
//typedef enum BroadphaseNativeTypes BroadphaseNativeTypes;

//typedef struct b3DynamicsWorld btDynamicsWorld;

// relevant collision shapes

        // C++ class equivalent             // alias for inclusion in C code.
typedef struct btCollisionShape             b3CollisionShape; // base collision type we will interface with C code.

typedef struct btBoxShape                   b3BoxShape;

typedef struct btCapsuleShape               b3CapsuleShape;

typedef struct btConvexHullShape            b3ConvexHullShape;

typedef struct btConeShapeX                 b3ConeShapeX; // cone shape around the x axis
typedef struct btConeShapeZ                 b3ConeShapeZ; // around the z axis

typedef struct btSphereShape                b3SphereShape; // sphere shape, centered around local origin with radius.

typedef struct btEmptyShape                 b3EmptyShape; // empty shape, no collision. can be replaced with other shape during runtime.

// simulates a heightfield terrain.
typedef struct btHeightfieldTerrainShape    b3HeightfieldTerrainShape;
// holds a mesh of triangles.
typedef struct btBvhTriangleMeshShape       b3BvhTriangleMeshShape;



// convience class used to store vertices and indices.
//typedef struct btTriangleIndexVertexArray   b3TriangleIndexVertexArray;
///The btStaticPlaneShape simulates an infinite non-moving (static) collision plane.
//typedef struct btStaticPlaneShape           b3StaticPlaneShape;

/////// End of Collision Shapes ///////

// used in the C++ code class initialization of btRigidBody.
//typedef struct btRigidBodyConstructionInfo  b3RigidBodyConstructionInfo;


// Not used atm. (optional param for btRigidBody)
typedef struct btMotionState                b3MotionState;

typedef struct btDefaultMotionState         b3DefaultMotionState;

// the rigid body. (can be dynamic, static, or kinematic)
typedef struct btRigidBody                  b3RigidBody;

extern float b3SphereShape_getRadius(b3RigidBody * body);

extern int b3RigidBody_default();

extern void b3World_deinit();

extern b3RigidBody * b3RigidBody_initPlane(vec3 origin);

extern b3RigidBody * b3RigidBody_initSphere(vec3 origin, float radius, vec3 inertia, float mass);

extern b3RigidBody * b3RigidBody_initBvhTriangleMesh(
	int numTriangles,
    int * triangleIndexBasePtr,
    int indexStride,
    int numVertices,
    float * vertexBasePtr,
    int vertexStride
);

extern b3RigidBody * b3RigidBody_initConvexTriangleMesh(
    int numTriangles,
    int * triangleIndexBasePtr,
    int indexStride,
    int numVertices,
    float * vertexBasePtr,
    int vertexStride
);

/*
btTriangleIndexVertexArray * b3TriangleIndexVertexArray_init(
    int numTriangles,
    int * triangleIndexBasePtr,
    int indexStride,
    int numVertices,
    float * vertexBasePtr,
    int vertexStride
);

btBvhTriangleMeshShape * b3CollisionShape_BvhTriangleMesh_init(
    b3TriangleIndexVertexArray * meshInterface,
    bool useQuantizedAabbCompression,
    bool buildBvh
);
*/
extern b3RigidBody * b3RigidBody_initHeightfieldTerrainMesh(
    void * heightfield_data,
	int width,
    int length
);

extern b3RigidBody * b3RigidBody_initScaledBvhTriangleShape();

extern b3DefaultMotionState * btRigidBodyConstructionInfo_genDefaultMotionState(
    vec3 origin // origin of transform
);

//extern btRigidBodyConstructionInfo * b3RigidBody_genConstructionInfo(
//    // TODO: add more params.
//    btMotionState motion_state
//);

extern void b3RigidBody_getTransform(
    const b3RigidBody * body_in, 
    mat4 mat_out
);

extern b3CollisionShapeType b3RigidBody_getCollisionShapeType(
    const b3RigidBody * body_in
);

extern void b3RigidBody_setVelocity(
    b3RigidBody * body_in,
    const vec3 velocity
);

extern void b3RigidBody_getVelocity(
    const b3RigidBody * body_in,
    vec3 velocity_out
);


///////////////////////////////////////////
// RigidBody, CollisionShape END //
////////////////////////////////////////////


// forward declaration of our extended class
typedef struct btDiscreteDynamicsWorldC btDiscreteDynamicsWorldC;

typedef struct {
    btDiscreteDynamicsWorldC * world;
    float fixedTimeStep;
    float localTime;

    float latencyMotionStateInterpolation;

} b3DiscreteDynamicsWorld, *b3DiscreteDynamicsWorldHandle;


extern b3DiscreteDynamicsWorld * b3World_default();

extern b3RigidBody ** b3World_getRigidBodiesPtr();

extern void b3World_updateAabbs();

extern void b3World_updateSingleAabb(b3RigidBody * body);

// this is the function with which we advance the physics simulation every frame.
// fixedTimeStep has default value of 60FPS (1 / 60)
extern int b3World_stepSimulation(float timeStep);

extern void b3World_debugDrawWorld();

extern void b3World_setGravity(vec3 in);
// x, y, z
extern void b3World_getGravity(vec3 out);

extern void b3World_addRigidBody(b3RigidBody * body);

extern void b3World_removeRigidBody(b3RigidBody * body);

extern void b3World_destruct();


#ifdef __cplusplus
}
#endif

#endif