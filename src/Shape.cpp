/*
 *  BoxShape.cpp
 *  bullet
 *
 *  Created by Stephan Huber on 17.11.08.
 *  Copyright 2008 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#include "Shape.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

namespace cefixbt {

ConvexHullShape::ConvexHullShape(osg::Node* node, bool simplify)
:
	btConvexHullShape(),
	osg::TriangleMesh()
{
	osg::ref_ptr<ConvexTriangleMeshShape> tmpConvexShape = new ConvexTriangleMeshShape(node);
        
	//create a hull approximation
	btShapeHull* hull = new btShapeHull(tmpConvexShape.get());
	btScalar margin = tmpConvexShape->getMargin();
	hull->buildHull(margin);
	tmpConvexShape->setUserPointer(hull);
	
	for (unsigned int i=0;i<hull->numVertices();i++)
	{
			addPoint(btVector3(hull->getVertexPointer()[i]));   
	}

	tmpConvexShape = NULL;
	delete hull;
}

}