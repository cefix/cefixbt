/*
 *  PhysicsThread.h
 *  bullet
 *
 *  Created by Stephan Huber on 21.10.10.
 *  Copyright 2010 stephanmaximilianhuber.com. All rights reserved.
 *
 */

#ifndef CEFIX_BT_PHYSICS_THREAD
#define CEFIX_BT_PHYSICS_THREAD

#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#include <osg/Referenced>
#include <osg/Timer>
#include "PhysicsController.h"

namespace cefixbt {


class PhysicsThread : public OpenThreads::Thread, public osg::Referenced {
public:
    PhysicsThread(PhysicsController* ctrl);
    
    virtual void run();
    
    void setPaused(bool p);
    
    bool isPaused() {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_pauseMutex);
        return( _pauseGate.numThreadsCurrentlyBlocked() > 0 );
    }
    
    void stop();
    
    /** set minimum frame druation in secs */
    void setMinimalFrameDuration(double dt) { _minFrameRate = dt * 1000 * 1000; }
    
    ~PhysicsThread();

private:
    osg::Timer _t;
    osg::observer_ptr<PhysicsController> _ctrl;
    bool _finished, _isRunning;
    int  _pausedCount;
    OpenThreads::Barrier _pauseGate;
    OpenThreads::Mutex _pauseMutex;
    unsigned int _frames;
    osg::Timer_t _minFrameRate;
};

}

#endif
