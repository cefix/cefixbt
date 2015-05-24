/*
 *  DebugPhysicsDrawer.h
 *  bullet
 *
 *  Created by Stephan Huber on 18.11.08.
 *  Copyright 2008 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#ifndef CEFIX_BT_DEBUG_PHYSICS_DRAWER_HEADER
#define CEFIX_BT_DEBUG_PHYSICS_DRAWER_HEADER


#include "Utils.h"
#include <cefix/DebugGeometryFactory.h>

namespace cefixbt {
	
	class DebugPhysicsDrawer : public btIDebugDraw, public osg::Referenced {
	
	public:
		DebugPhysicsDrawer();
		
		virtual void drawLine (const btVector3 &from, const btVector3 &to, const btVector3 &color) ;		
		virtual void drawContactPoint (const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);
		virtual void reportErrorWarning (const char *warningString);
		virtual void draw3dText (const btVector3 &location, const char *textString);
		virtual void setDebugMode (int debugMode) { _debugMode = debugMode; }
		virtual int getDebugMode () const { return _debugMode; }
		
		
		inline void clear() {
			_geo->clear();
		}
		
		inline void finish() {
			_geo->finish();
		}
		
		osg::Node* getNode() { return _geo->getOrCreateNode(); }
	
	private:
		osg::ref_ptr<cefix::DebugOutputBase>	_geo;
		int _debugMode;
	};
}

#endif