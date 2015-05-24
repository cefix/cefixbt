/*
 *  TripleTransformBuffer.h
 *  bullet
 *
 *  Created by Stephan Huber on 30.10.10.
 *  Copyright 2010 stephanmaximilianhuber.com. All rights reserved.
 *
 */

#ifndef TRANSFORM_TRIPPLE_BUFFER_HEADER
#define TRANSFORM_TRIPPLE_BUFFER_HEADER

#include <cefix/TripleBuffer.h>
#include "btBulletDynamicsCommon.h"

namespace cefixbt {

class  TransformTripleBuffer : public cefix::TripleBufferRef<btTransform> {};


}


#endif