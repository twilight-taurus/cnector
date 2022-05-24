#include <bulletC_API.h>

// use single precision floats. (faster, less precise)
#define BT_USE_SINGLE_PRECISION

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>

#include<BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include<BulletCollision/CollisionShapes/btSphereShape.h>

#include<BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h>
#include<BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include<BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
//#include<BulletCollision/CollisionShapes/btCollisionShape.h>

#include <vector>
#include <set>
#include <map>

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

#include "btBulletCollisionCommon.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
///The btRigidBody is the main class for rigid body objects.
///derived from btCollisionObject

///There are 3 types of rigid bodies:
///- A) Dynamic rigid bodies, with positive mass. 
// Motion is controlled by rigid body dynamics.
///- B) Fixed objects with zero mass. (Static)
///They are not moving (basically collision objects)
///- C) Kinematic objects, 
///which are objects without mass, but the user can move them. There is one-way interaction, and Bullet calculates a velocity based on the timestep and previous and current world transform.


///Bullet automatically deactivates dynamic rigid bodies, 
///when the velocity is below a threshold for a given time.
///Deactivated (sleeping) rigid bodies don't take any processing time, 
///except a minor broadphase collision detection impact 
///(to allow active objects to activate/wake up sleeping objects)

class b3DebugDraw : public btIDebugDraw {
    int m_debugMode;

    public:
    b3DebugDraw();
//    virtual ~b3DebugDraw(); 

//    virtual void    drawLine(const btVector3& from,const btVector3& to,const btVector3&  fromColor, const btVector3& toColor);

    virtual void    drawLine(const btVector3& from,const btVector3& to,const btVector3& color);

//    virtual void    drawSphere (const btVector3& p, btScalar radius, const btVector3& color);

//    virtual void    drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);

//    virtual void    drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);

    virtual void    reportErrorWarning(const char* warningString);

    virtual void    draw3dText(const btVector3& location,const char* textString);

    virtual void    setDebugMode(int debugMode);

    virtual int     getDebugMode() const { return m_debugMode;}

    virtual void    drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

};

b3DebugDraw::b3DebugDraw() {

}

void b3DebugDraw::setDebugMode(int debugMode) {
    m_debugMode = debugMode;
}

void b3DebugDraw::reportErrorWarning(const char* warningString) {
    return;
}

void b3DebugDraw::draw3dText(const btVector3& location,const char* textString) {
    return;
}

void b3DebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
    return;
}

/*
b3DebugDraw::~b3DebugDraw() {
}
*/

// draw a 3D line for the physics debugger.
void b3DebugDraw::drawLine(const btVector3& from,const btVector3& to,const btVector3& color) {
    Vector3 from_gl = (Vector3){from.getX(), from.getY(), from.getZ()};
    Vector3 to_gl = (Vector3){to.getX(), to.getY(), to.getZ()};
//    Color color_gl = (Color){ (char)color.getX(), (char)color.getY(), (char)color.getZ(), 255 };
    Color color_gl = (Color){ 0, 0, 0, 255 };

    DrawLine3D( from_gl, to_gl, color_gl);
}


float b3SphereShape_getRadius(btRigidBody * body) {
    if (body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE) {

        // will nullptr if not a btSphereShape
        btSphereShape * sphere = dynamic_cast<btSphereShape*>( body->getCollisionShape() );
        printf("Got radius of sphere: %.6f", sphere->getRadius());
        return (float)sphere->getRadius();
    }

    return 0.0f;
}

int b3RigidBody_default() {
    return 1;
}

b3RigidBody * b3RigidBody_initPlane(vec3 origin) {
    btTransform t;
    t.setIdentity();
    // initial transform of rigid body.
    t.setOrigin( btVector3(0.0, 0.0, 0.0) );

    // add static plane                                         // 1. y is upward (plane is aligned along x-z axes.)
                                                                // 2. offset from origin

    btStaticPlaneShape * plane = new btStaticPlaneShape( btVector3(0.0, 1.0, 0.0), 0.0 );

//    btVector3 bt_inertia(1.0, 1.0, 1.0);
//    plane->calculateLocalInertia(0.0, bt_inertia);
    btDefaultMotionState * motion = new btDefaultMotionState(t);

    btRigidBody::btRigidBodyConstructionInfo info_plane(0.0, motion, plane);

    // create rigid body from plane collision shape and other params.
    b3RigidBody * body = new btRigidBody(info_plane);

    return body;
}

b3RigidBody * b3RigidBody_initSphere(vec3 origin, float radius, vec3 inertia, float mass) {
    btTransform t;
    t.setIdentity();
                // 0.0, 30.0, 0.0
    t.setOrigin( btVector3(origin[0], origin[1], origin[2]) );

    btDefaultMotionState * motion = new btDefaultMotionState(t);

    // add sphere
    btScalar rad = (btScalar)radius;
                                    // 2.0, 2.0, 2.0
    btVector3 bt_inertia(inertia[0], inertia[1], inertia[2]);

    btSphereShape * sphere = new btSphereShape(rad);
    sphere->calculateLocalInertia(mass, bt_inertia);
                                                     // mass   location  Coll. obj
    btRigidBody::btRigidBodyConstructionInfo info_sphere(mass, motion, sphere);

    b3RigidBody* body_sphere = new btRigidBody(info_sphere);

    // add rigid body to world, and put reference to vector.
    return body_sphere;
}

// store pointers to the vertices and indices of the model/shape that is rendered here.
// this object is not going to copy the data. so make sure the pointers here match the lifetime of the graphical model/shape
btTriangleIndexVertexArray * b3TriangleIndexVertexArray_init(
    int numTriangles,
    int * triangleIndexBasePtr,
    int indexStride,
    int numVertices,
    float * vertexBasePtr,
    int vertexStride
) {

    btTriangleIndexVertexArray * obj = new btTriangleIndexVertexArray(
        numTriangles,
        triangleIndexBasePtr,
        indexStride,
        numVertices,
        vertexBasePtr,
        vertexStride
    );

    return obj;
}

// the triangle collision shape that corresponds to our graphics model/shape.
btBvhTriangleMeshShape * b3CollisionShape_BvhTriangleMesh_init(
    btTriangleIndexVertexArray * meshInterface,
    bool useQuantizedAabbCompression,
    bool buildBvh
) {
    btBvhTriangleMeshShape * obj = new btBvhTriangleMeshShape(
        meshInterface,
        useQuantizedAabbCompression,
        buildBvh
    );

    return obj;
}

btConvexTriangleMeshShape * b3CollisionShape_ConvexTriangleMeshShape_init(
    btTriangleIndexVertexArray * meshInterface,
    bool calcAabb = true
)   {
    btConvexTriangleMeshShape * obj = new btConvexTriangleMeshShape(
        meshInterface,
        calcAabb
    );

    return obj;
}

b3RigidBody * b3RigidBody_initConvexTriangleMesh(
    int numTriangles,
    int * triangleIndexBasePtr,
    int indexStride,
    int numVertices,
    float * vertexBasePtr,
    int vertexStride
) {

    btTransform t;
    t.setIdentity();
    t.setOrigin( btVector3(0.0, 0.0, 0.0) );
    printf("Set Transform.\n");
    btDefaultMotionState * motion = new btDefaultMotionState(t);

    btTriangleIndexVertexArray * mesh_data = b3TriangleIndexVertexArray_init(
        numTriangles,
        triangleIndexBasePtr,
        indexStride,
        numVertices,
        vertexBasePtr,
        vertexStride
    );

    printf("Index Vertex Array set.\n");

    btConvexTriangleMeshShape * shape = b3CollisionShape_ConvexTriangleMeshShape_init(
    mesh_data,
    false // calc Aabb
    );
    printf("Collision Shape initialized.\n");

//    btVector3 scaling = btVector3(0.02, 0.02, 0.02);
//    shape->setLocalScaling( scaling );
//    printf("Set local scaling.\n");

    btRigidBody::btRigidBodyConstructionInfo info_mesh(0.0, motion, shape);
    printf("Construction Info created.\n");
    b3RigidBody * triangle_mesh = new btRigidBody(info_mesh);
    printf("RigidBody created.\n");

    return triangle_mesh;
}

b3RigidBody * b3RigidBody_initHeightfieldTerrainMesh(
    void * heightfield_data,
    int width, // width of the heightfield (meters)
    int length // length of the heightfield (meters)
) {

    btHeightfieldTerrainShape * shape = new btHeightfieldTerrainShape(
        width,
        length,
        heightfield_data,
        0.0,
        1000.0,
        1, // -- up-axis: Y
        false
    );

    btVector3 scaling = btVector3(0.02, 0.02, 0.02);
    shape->setLocalScaling( scaling );

    printf("Set local scaling.\n");

    btTransform t;
    t.setIdentity();
    t.setOrigin( btVector3(0.0, 0.0, 0.0) );

    btDefaultMotionState * motion = new btDefaultMotionState(t);

    // fixed body.
    btRigidBody::btRigidBodyConstructionInfo info_sphere(0.0, motion, shape);

    b3RigidBody* heightfield_collider = new btRigidBody(info_sphere);

    return heightfield_collider;
}

b3RigidBody * b3RigidBody_initBvhTriangleMesh(
    int numTriangles,
    int * triangleIndexBasePtr,
    int indexStride,
    int numVertices,
    float * vertexBasePtr, // make sure the stride is 4*float !!! (btVector3 stride)
    int vertexStride
) {

    btTransform t;
    t.setIdentity();
    t.setOrigin( btVector3(0.0, 0.0, 0.0) );
//    printf("Set Transform.\n");
    btDefaultMotionState * motion = new btDefaultMotionState(t);


    btTriangleIndexVertexArray * iv_array = new btTriangleIndexVertexArray(
        numTriangles,
        triangleIndexBasePtr,
        indexStride,
        numVertices,
        vertexBasePtr,
        vertexStride
    );

    printf("Index Vertex Array set.\n");

    btBvhTriangleMeshShape * shape = new btBvhTriangleMeshShape(
        iv_array,
        false,
        true
    );

    printf("Collision Shape initialized.\n");
/*
    btVector3 scaling = btVector3(0.01, 0.01, 0.01);
    shape->setLocalScaling( scaling );
    printf("Set local scaling.\n");
*/
    btRigidBody::btRigidBodyConstructionInfo info_mesh(0.0, motion, shape);
//    printf("Construction Info created.\n");
    b3RigidBody * triangle_mesh = new btRigidBody(info_mesh);
//    printf("RigidBody created.\n");

    return triangle_mesh;
}

void b3RigidBody_deinit(b3RigidBody * body) {
    // call C++ destructor (which hopefully destructs any dependents)
    body->~btRigidBody();
    // deallocate memory.
    delete body;
}

// try inlining this function. see if it still works as a C API then.
void b3RigidBody_getTransform(const btRigidBody * body_in, mat4 mat_out) {
//    printf("Getting rigid body transform!\n");
    btTransform t;
    body_in->getMotionState()->getWorldTransform(t);

    // mat_out is modified here. ((hopefully correctly)
    float * floats_ptr = ((float*)mat_out);
    t.getOpenGLMatrix( floats_ptr );
}

b3CollisionShapeType b3RigidBody_getCollisionShapeType(const btRigidBody * body_in) {
    int type = body_in->getCollisionShape()->getShapeType();
    return type;
}

void b3RigidBody_setVelocity(
    btRigidBody * body_in,
    const vec3 velocity
)   {
    body_in->setLinearVelocity( btVector3(velocity[0], velocity[1], velocity[2]) );
}

void b3RigidBody_getVelocity(
    const btRigidBody * body_in,
    vec3 velocity_out
)   {
    const btVector3 vec_bt = body_in->getLinearVelocity();

    velocity_out[0] = vec_bt.x();
    velocity_out[1] = vec_bt.y();
    velocity_out[2] = vec_bt.z();
}


class btDiscreteDynamicsWorldC : public btDiscreteDynamicsWorld
{
    public:
        btDiscreteDynamicsWorldC(btDispatcher * dispatcher, btBroadphaseInterface * pairCache, btConstraintSolver * constraintSolver, btCollisionConfiguration * collisionConfiguration)
        : btDiscreteDynamicsWorld(dispatcher, pairCache, constraintSolver, collisionConfiguration)
        {

        }
        ~btDiscreteDynamicsWorldC()
        {

        }
        /*
        void debugDrawWorld()
        {
            BT_PROFILE("debugDrawWorld");

            //  TODO: non static reference problem.
            //	btCollisionWorld::debugDrawWorld();

            bool drawConstraints = false;
            if (g_b3World->getDebugDrawer())
            {
                int mode = g_b3World->getDebugDrawer()->getDebugMode();
                if (mode & (btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawConstraintLimits))
                {
                    drawConstraints = true;
                }
            }
            if (drawConstraints)
            {
                for (int i = g_b3World->getNumConstraints() - 1; i >= 0; i--)
                {
                    btTypedConstraint* constraint = g_b3World->getConstraint(i);
                    g_b3World->debugDrawConstraint(constraint);
                }
            }

            if (g_b3World->getDebugDrawer() && (g_b3World->getDebugDrawer()->getDebugMode() & (btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb | btIDebugDraw::DBG_DrawNormals)))
            {
                int i;

                if (g_b3World->getDebugDrawer() && g_b3World->getDebugDrawer()->getDebugMode())
                {
                    for (i = 0; i < m_actions.size(); i++)
                    {
                        m_actions[i]->debugDraw(m_debugDrawer);
                    }
                }
            }
            if (g_b3World->getDebugDrawer())
                g_b3World->getDebugDrawer()->flushLines();
        }
        */
/*
        void saveKinematicState(btScalar timeStep)
        {
            ///would like to iterate over m_nonStaticRigidBodies, but unfortunately old API allows
            ///to switch status _after_ adding kinematic objects to the world
            ///fix it for Bullet 3.x release
            for (int i = 0; i < m_collisionObjects.size(); i++)
            {
                btCollisionObject* colObj = m_collisionObjects[i];
                btRigidBody* body = btRigidBody::upcast(colObj);
                if (body && body->getActivationState() != ISLAND_SLEEPING)
                {
                    if (body->isKinematicObject())
                    {
                        //to calculate velocities next frame
                        body->saveKinematicState(timeStep);
                    }
                }
            }
        }
*/
/*
        void internalSingleStepSimulation(btScalar timeStep)
        {
            BT_PROFILE("internalSingleStepSimulation");

            if (0 != m_internalPreTickCallback)
            {
                (*m_internalPreTickCallback)(this, timeStep);
            }

            ///apply gravity, predict motion
            predictUnconstraintMotion(timeStep);

            btDispatcherInfo& dispatchInfo = getDispatchInfo();

            dispatchInfo.m_timeStep = timeStep;
            dispatchInfo.m_stepCount = 0;
            dispatchInfo.m_debugDraw = getDebugDrawer();

            createPredictiveContacts(timeStep);

            ///perform collision detection
            performDiscreteCollisionDetection();

            calculateSimulationIslands();

            getSolverInfo().m_timeStep = timeStep;

            ///solve contact and other joint constraints
            solveConstraints(getSolverInfo());

            ///CallbackTriggers();

            ///integrate transforms

            integrateTransforms(timeStep);

            ///update vehicle simulation
            updateActions(timeStep);

            updateActivationState(timeStep);

            if (0 != m_internalTickCallback)
            {
                (*m_internalTickCallback)(this, timeStep);
            }
        }
*/
};

// static Class objects -> the "building blocks" of the physics engine.
btDispatcher * g_b3Dispatcher;

btCollisionConfiguration * g_b3CollisionConfig;

btBroadphaseInterface * g_b3Broadphase;

btConstraintSolver * g_b3Solver;
// Inheriting class to allow external access to functions.
//btDiscreteDynamicsWorldC * g_b3World;

b3DiscreteDynamicsWorld * g_b3World;

b3DebugDraw * g_b3DebugDraw;


btDefaultMotionState * btRigidBodyConstructionInfo_genDefaultMotionState(
    vec3 origin // origin of transform
    // ...
)   {
    btVector3 vec = btVector3(origin[0], origin[1], origin[2]);

    btTransform trans;
    trans.setIdentity();
    // set positional element of transformation matrix
    trans.setOrigin(vec);
    btDefaultMotionState * state = new btDefaultMotionState;
    *state = btDefaultMotionState(trans);
    // return ptr to conform to C API.
    return state;
}

/*
btRigidBody ** b3World_getRigidBodiesPtr() {
    return rBodies.data();
}
*/

b3DiscreteDynamicsWorld * b3World_default() {

    g_b3World = new b3DiscreteDynamicsWorld;

    g_b3CollisionConfig = new btDefaultCollisionConfiguration();
    g_b3Dispatcher = new btCollisionDispatcher(g_b3CollisionConfig);

    g_b3Broadphase = new btDbvtBroadphase();
    g_b3Solver = new btSequentialImpulseConstraintSolver();

    // create world c++ construct here from arguments.
    g_b3World->world = new btDiscreteDynamicsWorldC(g_b3Dispatcher, g_b3Broadphase, g_b3Solver, g_b3CollisionConfig);

    g_b3DebugDraw = new b3DebugDraw();

    printf("Setting Debug drawer.\n");
    g_b3World->world->setDebugDrawer(g_b3DebugDraw);
    printf("Debug drawer set.\n");

//    printf("Setting Debug mode.\n");
    // initialize debug drawer.
    // assign debug drawer.
    g_b3World->world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    printf("Debug mode set.\n");

    // return struct.
    return g_b3World;
}

void b3World_deinit() {
    // call c++ destructor (which hopefully invokes the inherited destructors.)
//    g_b3World->world->~btDiscreteDynamicsWorldC();
    // Alternative: invoke inherited destructor. (inherited class has no destructor atm)
    g_b3World->world->btDiscreteDynamicsWorld::~btDiscreteDynamicsWorld();

//    delete g_b3World->world;
    delete g_b3World;
}

void b3World_debugDrawWorld() {
    g_b3World->world->debugDrawWorld();
}

void b3World_updateAabbs() {
  g_b3World->world->updateAabbs();
}

void b3World_updateSingleAabb(b3RigidBody * body) {
    g_b3World->world->updateSingleAabb(body);
}


int b3World_stepSimulation(float timeStep) {

    g_b3World->world->stepSimulation(timeStep);
/*
    int numSimulationSubSteps = 0;

    if (maxSubSteps)
	{
		//fixed timestep with interpolation
		g_b3World->fixedTimeStep = fixedTimeStep;
		g_b3World->localTime += timeStep;

		if (g_b3World->localTime >= fixedTimeStep)
		{
			numSimulationSubSteps = int(g_b3World->localTime / fixedTimeStep);
			g_b3World->localTime -= numSimulationSubSteps * fixedTimeStep;
		}
	}
	else
	{
		//variable timestep
		g_b3World->fixedTimeStep = timeStep;
		g_b3World->localTime = g_b3World->latencyMotionStateInterpolation ? 0 : timeStep;
		g_b3World->fixedTimeStep = (float)0;
		if (btFuzzyZero(timeStep))
		{
			numSimulationSubSteps = 0;
			maxSubSteps = 0;
		}
		else
		{
			numSimulationSubSteps = 1;
			maxSubSteps = 1;
		}
	}

    //process some debugging flags
	if (g_b3World->world->getDebugDrawer())
	{
		btIDebugDraw* debugDrawer = g_b3World->world->getDebugDrawer();
		gDisableDeactivation = (debugDrawer->getDebugMode() & btIDebugDraw::DBG_NoDeactivation) != 0;
	}
	if (numSimulationSubSteps)
	{
		//clamp the number of substeps, to prevent simulation grinding spiralling down to a halt
		int clampedSimulationSteps = (numSimulationSubSteps > maxSubSteps) ? maxSubSteps : numSimulationSubSteps;
        
        // cannot call this method. TODO: try invoking it through function pointers embedded in the wrapper struct.
		g_b3World->world->saveKinematicState(fixedTimeStep * clampedSimulationSteps);

		g_b3World->world->applyGravity();

		for (int i = 0; i < clampedSimulationSteps; i++)
		{   
            // cannot call this method. TODO: try invoking it through function pointers embedded in the wrapper struct.
			g_b3World->world->internalSingleStepSimulation(fixedTimeStep);
			g_b3World->world->synchronizeMotionStates();
		}
	}
	else
	{
		g_b3World->world->synchronizeMotionStates();
	}

	g_b3World->world->clearForces();
*/
    return 1;
}

// x, y, z
void b3World_setGravity(vec3 in) {
    g_b3World->world->setGravity( btVector3(in[0], in[1], in[2]) );
}

void b3World_getGravity(vec3 out) {
    btVector3 vec = g_b3World->world->getGravity();
    out[0] = vec.x();  out[1] = vec.y(); out[2] = vec.z();
}

void b3World_addRigidBody(btRigidBody * body) {
    g_b3World->world->addRigidBody(body);
}

void b3World_removeRigidBody(btRigidBody * body) {
    g_b3World->world->removeRigidBody(body);

}