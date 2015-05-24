/*
 *  MotionState.cpp
 *  bullet
 *
 *  Created by Stephan Huber on 17.11.08.
 *  Copyright 2008 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#include "MotionState.h"
#include "PhysicsController.h"

namespace cefixbt {

bool MotionState::computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor*) const
{
    
	if (_referenceFrame==RELATIVE_RF)
    {
        matrix.preMult( _matrix );
    }
    else // absolute
    {
        matrix = _matrix;
    }
    return true;
}

bool MotionState::computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor*) const
{
    const osg::Matrix& inverse = getInverseMatrix();

    if (_referenceFrame==RELATIVE_RF)
    {
        matrix.postMult(inverse);
    }
    else // absolute
    {
        matrix = inverse;
    }
    return true;
}


void MotionState::setWorldTransform(const btTransform& centerOfMassWorldTrans)
{
	m_graphicsWorldTrans = centerOfMassWorldTrans * m_centerOfMassOffset ;
		
	if (_buffer) 
		_buffer->write(_bufferIndex, centerOfMassWorldTrans);
	else 
		setWorldTransformInternal(centerOfMassWorldTrans);
}	

}