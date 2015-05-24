/*
 *  Constraints.h
 *  bullet
 *
 *  Created by Stephan Huber on 15.02.09.
 *  Copyright 2009 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#ifndef CEFIXBT_CONSTRAINTS_HEADER
#define CEFIXBT_CONSTRAINTS_HEADER

#include "RigidBody.h"

namespace cefixbt {
	
	class Point2PointConstraint : public btPoint2PointConstraint, public osg::Referenced {
	
	public:
		
		Point2PointConstraint(btRigidBody* rba, btRigidBody* rbb, const osg::Vec3& pivota, const osg::Vec3& pivotb)
		:	btPoint2PointConstraint(*rba, *rbb, asBtVector3(pivota), asBtVector3(pivotb)),
			osg::Referenced()
		{}
		
		Point2PointConstraint(btRigidBody* rba, btRigidBody* rbb, const btVector3& pivota, const btVector3& pivotb)
		:	btPoint2PointConstraint(*rba, *rbb, pivota, pivotb),
			osg::Referenced()
		{}
		
		Point2PointConstraint(btRigidBody* rb, const osg::Vec3& pivota)
		:	btPoint2PointConstraint(*rb, asBtVector3(pivota)),
			osg::Referenced()
		{}


		Point2PointConstraint(btRigidBody* rb, const btVector3& pivota)
		:	btPoint2PointConstraint(*rb, pivota),
			osg::Referenced()
		{}
		
		~Point2PointConstraint() {
			// cefix::cefix::log::info("Point2PointConstraint") << "d'tor called" << std::endl;
		}
	};
	
	class SliderConstraint : public btSliderConstraint, public osg::Referenced {
	public:
		SliderConstraint(btRigidBody* rba, btRigidBody* rbb, const btTransform& framea,const btTransform& frameb, bool useLinearReferenceFrameB )
		:	btSliderConstraint(*rba, *rbb, framea, frameb, useLinearReferenceFrameB),
			osg::Referenced()
		{}
		
		SliderConstraint(btRigidBody* rba, btRigidBody* rbb, const osg::Matrix& framea,const osg::Matrix& frameb, bool useLinearReferenceFrameB)
		:	btSliderConstraint(*rba, *rbb, asBtTransform(framea), asBtTransform(frameb), useLinearReferenceFrameB),
			osg::Referenced()
		{}
		
		~SliderConstraint() {
			// cefix::cefix::log::info("Point2PointConstraint") << "d'tor called" << std::endl;
		}
	
	};

	class Generic6DofConstraint : public btGeneric6DofConstraint, public osg::Referenced {
	public:
		Generic6DofConstraint(btRigidBody* rba, btRigidBody* rbb, const btTransform& framea,const btTransform& frameb, bool useLinearReferenceFrameB )
		:	btGeneric6DofConstraint(*rba, *rbb, framea, frameb, useLinearReferenceFrameB),
			osg::Referenced()
		{}
		
		Generic6DofConstraint(btRigidBody* rba, btRigidBody* rbb, const osg::Matrix& framea,const osg::Matrix& frameb, bool useLinearReferenceFrameB)
		:	btGeneric6DofConstraint(*rba, *rbb, asBtTransform(framea), asBtTransform(frameb), useLinearReferenceFrameB),
			osg::Referenced()
		{}
		
		~Generic6DofConstraint() {
			// cefix::cefix::log::info("Point2PointConstraint") << "d'tor called" << std::endl;
		}
		
	
	};
	
	class Generic6DofSpringConstraint : public btGeneric6DofSpringConstraint, public osg::Referenced {
	public:
		Generic6DofSpringConstraint(btRigidBody* rba, btRigidBody* rbb, const btTransform& framea,const btTransform& frameb, bool useLinearReferenceFrameB )
		:	btGeneric6DofSpringConstraint(*rba, *rbb, framea, frameb, useLinearReferenceFrameB),
			osg::Referenced()
		{}
		
		Generic6DofSpringConstraint(btRigidBody* rba, btRigidBody* rbb, const osg::Matrix& framea,const osg::Matrix& frameb, bool useLinearReferenceFrameB)
		:	btGeneric6DofSpringConstraint(*rba, *rbb, asBtTransform(framea), asBtTransform(frameb), useLinearReferenceFrameB),
			osg::Referenced()
		{}
		
		~Generic6DofSpringConstraint() {
			// cefix::cefix::log::info("Point2PointConstraint") << "d'tor called" << std::endl;
		}
	};

}


#endif