/*
 *  PhysicsThread.cpp
 *  bullet
 *
 *  Created by Stephan Huber on 21.10.10.
 *  Copyright 2010 stephanmaximilianhuber.com. All rights reserved.
 *
 */

#include "PhysicsThread.h"
#include <cefix/Log.h>
#include <iostream>

namespace cefixbt {

PhysicsThread::PhysicsThread(PhysicsController* ctrl)
:   OpenThreads::Thread(),
    osg::Referenced(),
    _ctrl(ctrl),
    _frames(0),
    _minFrameRate(1000 * 1000 / 30.0)
{
    _finished = false;
    _pausedCount = 0;
    _isRunning = false;
    _ctrl->enableThreading(this);
    _ctrl->setUseFixedTimeSteps(false);
}


void PhysicsThread::run() 
{
    _finished = false;
    _isRunning = true;
    
    osg::Timer_t current_t(_t.tick()), last_t(_t.tick());
    
    while(!_finished) 
    {
        current_t = _t.tick();
        
        bool paused = false;
        {
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_pauseMutex);;
            paused = (_pausedCount > 0);
        }
        if(paused) {
            //cefix::cefix::log::info("PhysicsThread") << "starting pause" << std::endl;
            _pauseGate.block();
            _pauseGate.release();
            //cefix::cefix::log::info("PhysicsThread") << "leaving pause" << std::endl;
        }
        
        _ctrl->updatePhysics();
        _frames++;
        
        
        if (osg::Timer_t dt = _t.delta_u(last_t, current_t) < _minFrameRate) {
            //std::cout << "too fast. sleeping for " << dt << "usec" << std::endl;
            OpenThreads::Thread::microSleep((_minFrameRate - dt));
        }
        last_t = current_t;
        
        if (0) {
            static osg::Timer_t last_fps = _t.tick();
            float dt = _t.delta_s(last_fps, _t.tick());
            if (dt >= 1.0) {
                std::cout << "physics fps: " << _frames * (1.0 / dt) << std::endl;
                _frames = 0;
                last_fps = _t.tick();
            }
        }
        
    }
    cefix::log::info("PhysicsThread") << "finishing thread " << std::endl;
    _isRunning = false;
    _finished = true;
}



void PhysicsThread::setPaused(bool f) 
{
    bool block(false), unblock(false);
    
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_pauseMutex);
        if (f)
        {   
            _pausedCount++;
            block = (_pausedCount == 1);
        }
        else 
        {
            _pausedCount--;
            unblock = (_pausedCount == 0);
        }
    }
    
    if (_isRunning && block) 
    {
        YieldCurrentThread();
        
        while(!isPaused()) {
            OpenThreads::Thread::microSleep( 10 );
        }
    } 
    if (_isRunning && unblock) {
        _pauseGate.release();
    }
} 



void PhysicsThread::stop() {
    if (!_isRunning) 
        return;
        
    _finished = true;
    
    if (isPaused())
       _pauseGate.release();
    
    while (_isRunning) {
        OpenThreads::Thread::microSleep( 10 );
    }
    
}

PhysicsThread::~PhysicsThread() 
{
    stop();
	cefix::log::info("PhysicsThread") << "destructed. " << std::endl;
}


}