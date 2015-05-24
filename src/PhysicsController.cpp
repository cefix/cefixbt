/*
 *  PhysicsController.cpp
 *  bullet
 *
 *  Created by Stephan Huber on 12.10.08.
 *  Copyright 2008 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#include "PhysicsController.h"
#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "PhysicsThread.h"
#include <cefix/AppController.h>
#include "RigidBody.h"

namespace cefixbt {

struct ContactPair {
    ContactPair(const btCollisionObject* in_a, const btCollisionObject* in_b) : a(in_a), b(in_b) {}
    
    const btCollisionObject *a, *b;
};


bool customContactDestroyedCallback( void* userData ) 
{
    ContactPair* cp = static_cast<ContactPair*>(userData);
   
    ContactCallback *cb1(NULL), *cb2(NULL);
    
    if (cp->a->getUserPointer()) {
        cb1 = static_cast<ContactCallback*>(cp->a->getUserPointer());
        cb1->removeContactWith(cp->b);
    }
    
    if (cp->b->getUserPointer()) {
        cb2 = static_cast<ContactCallback*>(cp->b->getUserPointer());
        cb2->removeContactWith(cp->a);
    } 
   
   delete cp;
   return false;
}

bool customContactAddedCallback( btManifoldPoint& cp, const btCollisionObject* colObj0,int partId0,
                              int index0,const btCollisionObject* colObj1,int partId1,int index1 ) {
    
    if (cp.m_userPersistentData)
        return false;
    
    ContactCallback *cb1(NULL), *cb2(NULL);
    
    if (colObj0->getUserPointer()) {
        cb1 = static_cast<ContactCallback*>(colObj0->getUserPointer());
        cb1->addContactWith(colObj1, cp);
    }
    if (colObj1->getUserPointer()) {
        cb2 = static_cast<ContactCallback*>(colObj1->getUserPointer());
        cb2->addContactWith(colObj0, cp);
    } 
    
    cp.m_userPersistentData = new ContactPair(colObj0, colObj1);
    
    return false;
}

	
PhysicsController::StoredObject::~StoredObject() {
    ptr = NULL;
    // cefix::cefix::log::info("StoredObject") << "d'tor called" << std::endl;
}

PhysicsController::PhysicsController(bool use_fixed_time_step, double time_step)
:	osg::Referenced(),
    _timeStep(time_step),
    _fixedTimeStep(use_fixed_time_step),
    _thread(NULL),
    _maxSubSteps(1),
    _appController(NULL)
{
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    _dispatcher = new	btCollisionDispatcher(_collisionConfiguration);
    
    _broadphase = new btDbvtBroadphase();
    
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    _solver = new btSequentialImpulseConstraintSolver;
    _dynamicsWorld = new btSoftRigidDynamicsWorld(_dispatcher,_broadphase,_solver,_collisionConfiguration);
    
    _dynamicsWorld->setGravity(btVector3(0,0,-10)); 
    
    _dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_RANDMIZE_ORDER | SOLVER_USE_WARMSTARTING;
    gContactDestroyedCallback = customContactDestroyedCallback;
    gContactAddedCallback = customContactAddedCallback;

   
    _lastUpdate = 0;
}

void PhysicsController::updatePhysics()
{
    osg::Timer_t current_time = _t.tick();
    if ((!_fixedTimeStep) && (_lastUpdate == 0)) {
        _lastUpdate = current_time;
    }
	
	if (_transformBuffer.valid() && _transformBuffer->beginWriting()) {
		_dynamicsWorld->stepSimulation(_fixedTimeStep ? _fixedTimeStep : _t.delta_s(_lastUpdate, current_time), _maxSubSteps);
		_transformBuffer->endWriting();
	} else {
		_dynamicsWorld->stepSimulation(_fixedTimeStep ? _fixedTimeStep : _t.delta_s(_lastUpdate, current_time), _maxSubSteps);
	}
    
	if (_appController) {
        _appController->addCustomPerformanceStatisticsValue("physics (f/s)", 1/ _t.delta_s(_lastUpdate, current_time));
    }
    _lastUpdate = current_time;
}

void PhysicsController::removeRigidBody(btRigidBody* rigidBody) 
{
    pause();
	
	_dynamicsWorld->removeRigidBody(rigidBody);
    
	unregisterMotionState(dynamic_cast<MotionState*>(rigidBody->getMotionState()) );
	
    removeStorage(rigidBody->getMotionState());
    removeStorage(rigidBody);
	
	resume();
}


void PhysicsController::addPerformanceStatistics(cefix::AppController* ctrl)
{
    _appController = ctrl;
    _appController->registerCustomPerformanceStatisticsValue("physics (f/s)");
}


void PhysicsController::doPause(bool f) 
{
    _thread->setPaused(f); 
}


PhysicsController::~PhysicsController() 
{
    _thread = NULL;
    
    //remove the rigidbodies from the dynamics world and delete them
    for (int i = _dynamicsWorld->getNumCollisionObjects()-1; i >= 0 ;i--)
    {
        btCollisionObject* obj = _dynamicsWorld->getCollisionObjectArray()[i];
        _dynamicsWorld->removeCollisionObject( obj );
        
    }
    
    _storage.clear();
    
    delete _dynamicsWorld;
    delete _solver;
    delete _broadphase;		
    delete _dispatcher;		
    delete _collisionConfiguration;
    
}


class PhysicsUpdateCallback : public osg::NodeCallback {
public:
    PhysicsUpdateCallback(PhysicsController* ctrl) : osg::NodeCallback(), _ctrl(ctrl) {}
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) {
        _ctrl->update();
        traverse(node, nv);
    }
private:
    osg::ref_ptr<PhysicsController> _ctrl;
};


class PhysicsAnimation: public cefix::AnimationBase {
public:
    PhysicsAnimation(PhysicsController* ctrl) : cefix::AnimationBase(0), _ctrl(ctrl) {}
    virtual void animate(float elapsed) {
        _ctrl->update();
    }
private:
    osg::ref_ptr<PhysicsController> _ctrl;
};
	

osg::NodeCallback* PhysicsController::createUpdateCallback() {
    return new PhysicsUpdateCallback(this);
}


cefix::AnimationBase* PhysicsController::createAnimation() {
    return new PhysicsAnimation(this);
}

	
	
	
			
} // end of namespace