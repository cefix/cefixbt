/*
 *  AppController.cpp
 *  bullet
 *
 *  Created by Stephan Huber on 21.10.10.
 *  Copyright 2010 stephanmaximilianhuber.com. All rights reserved.
 *
 */

#include "AppController.h"
#include <cefix/AnimationFactory.h>

namespace cefixbt {

AppController::AppController(bool threaded)
:   cefix::AppController(),
    _physics(new PhysicsController())
{
    if (threaded) {
        _physicsThread = new PhysicsThread(_physics);
		_physicsThread->setMinimalFrameDuration(1/60.0f);
    }
    
	cefix::AnimationFactory::instance()->add(_physics->createAnimation());

}


int AppController::run() {
	if (_physicsThread.valid())
		_physicsThread->start();
		
	return cefix::AppController::run();
}


}