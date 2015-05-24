/*
 *  PhysicsController.h
 *  bullet
 *
 *  Created by Stephan Huber on 12.10.08.
 *  Copyright 2008 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#ifndef CEFIX_BT_PHYSICS_CONTROLLER_HEADER
#define CEFIX_BT_PHYSICS_CONTROLLER_HEADER

#include "Utils.h"
#include "DebugPhysicsDrawer.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include <cefix/AnimationBase.h>

#include "MotionState.h"
#include "TransformTripleBuffer.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btDbvtBroadphase;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace cefix {
class AppController;
}

namespace cefixbt {
	
    class PhysicsThread;
    
	class PhysicsController: public osg::Referenced {
		
	public:
    
        class UpdateDebugDrawerCallback : public osg::NodeCallback {
        public:
            UpdateDebugDrawerCallback(PhysicsController* ctrl) : osg::NodeCallback(), _ctrl(ctrl) {}
            
            virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) {
                if (_ctrl.valid()) _ctrl->updateDebugOutput();
            }
        private:
            osg::observer_ptr<PhysicsController> _ctrl;
        };
	
		template<class T>
		class RefWrapper : public osg::Referenced {
		public:
			RefWrapper(T* t) : osg::Referenced(), _t(t) {}
			virtual ~RefWrapper()
			{
				// std::cout << "deleting " << _t << std::endl;
				delete _t;
			}

		private:
			T* _t;
		};
		
		class StoredObject {
			osg::ref_ptr< osg::Referenced>	ptr;
			unsigned int cnt;
		public:
			StoredObject() : ptr(NULL), cnt(0) {}
			StoredObject(osg::Referenced* t) : ptr(t), cnt(1) {}
			
			inline void ref() { ++cnt; }
			inline void unref() { --cnt; }
			
			bool valid() const { return (cnt > 0); }
            ~StoredObject();
		};
			
		typedef std::map<void*, StoredObject  > ObjectStorageMap;
		PhysicsController(bool use_fixed_time_step = true, double time_step = 1/60.0);
		
		inline btSoftRigidDynamicsWorld* getDynamicsWorld() const { return _dynamicsWorld; }
		
		template <typename T> void addStorage(T* r) 
		{ 
			if (r==NULL) return;
			ObjectStorageMap::iterator itr = _storage.find(r);
		
			if (itr == _storage.end()) {
				osg::Referenced* rr = dynamic_cast<osg::Referenced*>(r);
				if (rr) {
					_storage.insert(std::make_pair(r, StoredObject(rr)));
				} else {
					_storage.insert(std::make_pair(r, StoredObject(new RefWrapper<T>(r))));
				}
			} else {
				itr->second.ref();
			}
		}
		
		template <typename T>void removeStorage(T* t) 
		{
			if (!t) return;
			ObjectStorageMap::iterator itr = _storage.find(t);
			if (itr != _storage.end()) {
				itr->second.unref();
				if (!itr->second.valid())
					_storage.erase(itr);
			}
		}
		
		void addRigidBody(btRigidBody* rigidBody) 
        {
			addStorage(rigidBody);
			addStorage(rigidBody->getMotionState());
			
			registerMotionState(dynamic_cast<MotionState*>(rigidBody->getMotionState()));
			
            pause();
			_dynamicsWorld->addRigidBody(rigidBody);
            resume();
		}
        
        void removeRigidBody(btRigidBody* rigidBody);
		
		void addRigidBody(btRigidBody* body, short group, short mask) 
        {
			addStorage(body);
            addStorage( body->getMotionState() );
			
			registerMotionState(dynamic_cast<MotionState*>(body->getMotionState()) );
            
			pause();
			_dynamicsWorld->addRigidBody(body, group, mask);
            resume();
		}
		
		void addSoftBody(btSoftBody* body) 
        {
			addStorage(body);
            
            pause();
			_dynamicsWorld->addSoftBody(body);
            resume();
		}
		
		
		void addConstraint(btTypedConstraint* constraint, bool disableCollisionsBetweenLinkedBodies = false) 
        {
			addStorage(constraint);
			
            pause();
            _dynamicsWorld->addConstraint(constraint, disableCollisionsBetweenLinkedBodies);
            resume();
		}
		
		void removeConstraint(btTypedConstraint* constraint)
		{
			pause();
            _dynamicsWorld->removeConstraint(constraint);
            removeStorage(constraint);
			resume();
            
		}
		
			
		void updatePhysics();
		
		inline void update() {
			if(_transformBuffer.valid())
				updateMotionStates();
			else 
				updatePhysics();
		}
		
		void setDebugDrawer(DebugPhysicsDrawer* drawer) 
        { 
			_debugDrawer = drawer; 
			if (_debugDrawer.valid())
				_dynamicsWorld->setDebugDrawer(_debugDrawer.get());
		}
        
        osg::Node* getDebugDrawNode(int debugMode) 
        {
            if (_debugDrawer.valid()) 
            {
                return _debugDrawer->getNode();
            } 
            else 
            {
                cefixbt::DebugPhysicsDrawer* drawer = new cefixbt::DebugPhysicsDrawer();
				setDebugDrawer(drawer);
				drawer->setDebugMode(debugMode);
                
                drawer->getNode()->setUpdateCallback(new UpdateDebugDrawerCallback(this));
                
				return drawer->getNode();
            }
            
            return NULL;
        }
		
		
        
        void setGravity(const osg::Vec3& g) { _dynamicsWorld->setGravity(asBtVector3(g)); }
        void setGravity(const btVector3& g) { _dynamicsWorld->setGravity(g); }
		
		void resetLastUpdateTimeStamp() {
            _lastUpdate = 0;
        }
        
		osg::NodeCallback* createUpdateCallback();
        cefix::AnimationBase* createAnimation();
        
        void setUseFixedTimeSteps(bool f, float time_step = 1/60.0f) { _fixedTimeStep = f; _timeStep = time_step; }
        void setMaxSubSteps(unsigned int steps) { _maxSubSteps = steps; }
        
        void enableThreading(PhysicsThread* t) 
		{ 
			_thread = t; 
			_transformBuffer = new TransformTripleBuffer(); 
		}
        
        inline void pause() { if (_thread) doPause(true); }
        inline void resume() { if (_thread) doPause(false); }
        
        void addPerformanceStatistics(cefix::AppController* ctrl);
		
		
		void registerMotionState(MotionState* ms) {
			if (_transformBuffer.valid()) {
				pause();
				_transformBuffer->resize(_transformBuffer->size()+1);
				ms->setTripleBuffer(_transformBuffer, _transformBuffer->size()-1);
				_motionStates.push_back(ms);
				resume();
			}
		}
		
		void unregisterMotionState(MotionState* ms) 
		{
			std::vector<MotionState*>::iterator i = std::find(_motionStates.begin(), _motionStates.end(), ms);
			if (i != _motionStates.end()) 
				_motionStates.erase(i);
		}
        
	protected:
    
        void doPause(bool f); 
        
        void updateDebugOutput() 
		{
			if (_debugDrawer.valid()) {
                
				_debugDrawer->clear();
                pause();
				_dynamicsWorld->debugDrawWorld();
                resume();
				_debugDrawer->finish();
			}
		}
		
		void updateMotionStates() 
		{
			if (_transformBuffer.valid() == false) return;
			if (_transformBuffer->beginReading()) {
				for(std::vector<MotionState* >::iterator i = _motionStates.begin(); i!=_motionStates.end();++i) {
					(*i)->updateMotionStateFromBuffer();
				}
				_transformBuffer->endReading();
			}
		}
		
		virtual ~PhysicsController();
		
	private: 
		btDefaultCollisionConfiguration*		_collisionConfiguration;
		btCollisionDispatcher*					_dispatcher;
		btDbvtBroadphase*						_broadphase;
		btSequentialImpulseConstraintSolver*	_solver;
		btSoftRigidDynamicsWorld*				_dynamicsWorld;
		ObjectStorageMap						_storage;
		
		osg::ref_ptr<DebugPhysicsDrawer>		_debugDrawer;
        
        btScalar                                _timeStep;
        bool                                    _fixedTimeStep;
        osg::Timer                              _t;
        osg::Timer_t                            _lastUpdate;
        PhysicsThread*                          _thread;
        
        unsigned int                            _maxSubSteps;
        cefix::AppController*                   _appController;
		osg::ref_ptr<TransformTripleBuffer>		_transformBuffer;
		std::vector<MotionState* >	_motionStates;
    
    friend class UpdateDebugDrawerCallback;
		
		
	};
}



#endif