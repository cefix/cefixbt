/*
 *  DebugPhysicsDrawer.cpp
 *  bullet
 *
 *  Created by Stephan Huber on 18.11.08.
 *  Copyright 2008 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#include "DebugPhysicsDrawer.h"
#include <cefix/ErrorDispatcher.h>


namespace cefixbt {

DebugPhysicsDrawer::DebugPhysicsDrawer()
:
	btIDebugDraw(),
	osg::Referenced()
{
	cefix::DebugGeometryFactory::enable("physics");	
	_geo = cefix::DebugGeometryFactory::get("physics");	
	_geo->setNormalScale(10.0f);
	_geo->setPointSize(5.0f);
	
}


void DebugPhysicsDrawer::drawLine (const btVector3 &from, const btVector3 &to, const btVector3 &color) 
{
	_geo->addLine(asVec3(from), asVec3(to), asVec4(color));
}
		
void DebugPhysicsDrawer::drawContactPoint (const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) 
{
	_geo->addPoint(asVec3(PointOnB), asVec4(color));
	_geo->addNormal(asVec3(PointOnB), asVec3(normalOnB), asVec4(color));
}

void DebugPhysicsDrawer::reportErrorWarning (const char *warningString) {
	cefix::ErrorDispatcher dispatcher("bullet", cefix::ErrorDispatcher::THREAD_SAFE);
	dispatcher.handle(warningString);
}

void DebugPhysicsDrawer::draw3dText (const btVector3 &location, const char *textString) {
    _geo->addText(asVec3(location), textString, osg::Vec4(1,1,1,0.7));
}


}