/*
 *  MotionState.h
 *  bullet
 *
 *  Created by Stephan Huber on 17.11.08.
 *  Copyright 2008 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#ifndef CEFIXBT_MOTION_STATE_HEADER 
#define CEFIXBT_MOTION_STATE_HEADER

#include "btBulletDynamicsCommon.h"
#include <osg/Transform>
#include "Utils.h"
#include "TransformTripleBuffer.h"

namespace cefixbt {

class MotionState : public btDefaultMotionState, public osg::Transform {

public:
	MotionState(const osg::Matrix& transform, const osg::Matrix& centerOfMass = osg::Matrix::identity())
	:
		btDefaultMotionState(asBtTransform(transform),asBtTransform(centerOfMass)),
		osg::Transform(),
		_matrix(transform),
		_inverseDirty(true),
		_buffer(NULL)
	{
	}
	
	MotionState(const btTransform& transform, const btTransform& centerOfMass = btTransform::getIdentity())
	:
		btDefaultMotionState(transform,centerOfMass),
		osg::Transform(),
		_matrix(asMatrix(transform)),
		_inverseDirty(true),
		_buffer(NULL)
	{
	}
	
	void setMatrix(const osg::Matrix& m)
	{
		m_graphicsWorldTrans = asBtTransform(m);
		_inverseDirty = true;
		dirtyBound();
	}
	
	virtual bool computeLocalToWorldMatrix (osg::Matrix &matrix, osg::NodeVisitor *) const;
	virtual bool computeWorldToLocalMatrix (osg::Matrix &matrix, osg::NodeVisitor *) const;
	
    
	virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans);
	
	
	/** Get the inverse matrix. */
	inline const osg::Matrix& getInverseMatrix() const
	{
		if (_inverseDirty)
		{
			_inverse.invert(_matrix);
			_inverseDirty = false;
		}
		return _inverse;
	}
	
	inline void updateMotionStateFromBuffer() {
		btTransform t;
		_buffer->read(_bufferIndex,t);
		setWorldTransformInternal(t);
	}
	
	void setTripleBuffer(TransformTripleBuffer* buffer, std::size_t index) { _buffer = buffer; _bufferIndex = index; }
    
    virtual ~MotionState() {
        // cefix::cefix::log::info("MotionState") << "d'tor called" << std::endl;
    }
private:

	inline void setWorldTransformInternal(const btTransform& centerOfMassWorldTrans)
	{
		_matrix = asMatrix(m_graphicsWorldTrans);
		_inverseDirty = true;
		dirtyBound();
	}
	
	osg::Matrix _matrix;
	mutable osg::Matrix _inverse;
	mutable bool _inverseDirty;
	TransformTripleBuffer* _buffer;
	std::size_t _bufferIndex;
	
};
}

#endif