/*
 *  AppController.h
 *  bullet
 *
 *  Created by Stephan Huber on 21.10.10.
 *  Copyright 2010 stephanmaximilianhuber.com. All rights reserved.
 *
 */

#ifndef CEFIX_BT_APP_CONTROLLER_HEADER
#define CEFIX_BT_APP_CONTROLLER_HEADER

#include "PhysicsController.h"
#include "PhysicsThread.h"
#include <cefix/AppController.h>

namespace cefixbt {

class AppController : public cefix::AppController {
public:
    AppController(bool threaded = true);
    
    PhysicsController* getPhysics() { return _physics; }
    PhysicsThread* getPhysicsThread() { return _physicsThread; }
    
    void addRigidBody(btRigidBody* rigidBody) { _physics->addRigidBody(rigidBody); }
	void addRigidBody(btRigidBody* body, short group, short mask) { _physics->addRigidBody(body, group, mask); }	
    
    void removeRigidBody(btRigidBody* rigidBody) {_physics->removeRigidBody(rigidBody); }
	
    void addSoftBody(btSoftBody* body) {_physics->addSoftBody(body); }		
		
    void addConstraint(btTypedConstraint* constraint, bool disableCollisionsBetweenLinkedBodies = false) { _physics->addConstraint(constraint, disableCollisionsBetweenLinkedBodies); }
    void removeConstraint(btTypedConstraint* constraint) { _physics->removeConstraint(constraint); }
    
    void setGravity(const osg::Vec3& g) { _physics->setGravity(g); }
    
    osg::Node* getDebugDrawNode(int debugMode) 
    {
        return _physics->getDebugDrawNode(debugMode); 
    }
	
	virtual int run();
private:
    osg::ref_ptr<PhysicsController> _physics;
    osg::ref_ptr<PhysicsThread> _physicsThread;


};


}


#endif