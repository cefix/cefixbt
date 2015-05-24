/*
 *  BoxShape.h
 *  bullet
 *
 *  Created by Stephan Huber on 17.11.08.
 *  Copyright 2008 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#ifndef CEFIXBT_BOX_SHAPE_HEADER
#define CEFIXBT_BOX_SHAPE_HEADER

#include <osg/Shape>
#include "Utils.h"

namespace cefixbt {

class BoxShape : public btBoxShape, public osg::Box {
	
	public:
		BoxShape(const osg::Vec3& halfExtends) 
		:	btBoxShape( asBtVector3(halfExtends) ), 
			osg::Box( osg::Vec3(0,0,0), 2*halfExtends[0], 2*halfExtends[1], 2*halfExtends[2]) 
		{
		}
		
		BoxShape(const btVector3& halfExtends) 
		:	btBoxShape( halfExtends ), 
			osg::Box( osg::Vec3(0,0,0), 2*halfExtends[0], 2*halfExtends[1], 2*halfExtends[2]) 
		{
		}
};


class StaticPlaneShape : public btStaticPlaneShape, public osg::InfinitePlane {

	public:
		StaticPlaneShape(const osg::Vec3& planeNormal,float planeConstant)
		:	btStaticPlaneShape(asBtVector3(planeNormal), (btScalar)planeConstant),
			osg::InfinitePlane()
		{
			set(planeNormal, planeConstant);
		}
		
		StaticPlaneShape(const btVector3& planeNormal,btScalar planeConstant)
		:	btStaticPlaneShape(planeNormal, planeConstant),
			osg::InfinitePlane()
		{
			set(asVec3(planeNormal), planeConstant);
		}
};

class SphereShape : public btSphereShape, public osg::Sphere {
	public:
		SphereShape(float radius) : btSphereShape((btScalar)radius), osg::Sphere(osg::Vec3(0,0,0), radius) {}
};

class BvhTriangleMeshShape : public btBvhTriangleMeshShape, osg::TriangleMesh {
	public:
		BvhTriangleMeshShape(btStridingMeshInterface* meshInterface, bool useQuantizedAabbCompression, bool buildBvh = true)
		:
			btBvhTriangleMeshShape(meshInterface, useQuantizedAabbCompression, buildBvh),
			osg::TriangleMesh()
		{
		}
		
		BvhTriangleMeshShape(osg::Node* node, bool useQuantizedAabbCompression, bool buildBvh = true)
		:
			btBvhTriangleMeshShape(computeBtTriMeshCollisionShape(node), useQuantizedAabbCompression, buildBvh),
			osg::TriangleMesh()
		{
		}
};

class ConvexTriangleMeshShape : public btConvexTriangleMeshShape, public osg::TriangleMesh {
	public:
		ConvexTriangleMeshShape(btStridingMeshInterface* meshInterface, bool calcAabb = true)
		:
			btConvexTriangleMeshShape(meshInterface, calcAabb),
			osg::TriangleMesh()
		{
		}
		
		ConvexTriangleMeshShape(osg::Node* node, bool calcAabb = true)
		:
			btConvexTriangleMeshShape(computeBtTriMeshCollisionShape(node), calcAabb),
			osg::TriangleMesh()
		{
		}
};



class ConvexHullShape : public btConvexHullShape, public osg::TriangleMesh {
	public:
		ConvexHullShape(const btScalar * 	points = 0,int 	numPoints = 0, int 	stride = sizeof(btVector3))
		:
			btConvexHullShape(points, numPoints, stride),
			osg::TriangleMesh()
		{
		}
		
		ConvexHullShape(osg::Node* node, bool simplify = true);		
		ConvexHullShape(osg::Vec3Array* points)
		:
			btConvexHullShape(),
			osg::TriangleMesh()
		{
			addPoints(points);
		}

				
		void addPoints(osg::Vec3Array* points) {
			for(osg::Vec3Array::iterator i = points->begin(); i != points->end(); ++i) {
				addPoint(asBtVector3(*i));
			}
		}
};


}

#endif