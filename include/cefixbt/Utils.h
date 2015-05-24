/*
 *  Utils.h
 *  bullet
 *
 *  Created by Stephan Huber on 12.10.08.
 *  Copyright 2008 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#ifndef CEFIXBT_UTILS_HEADER
#define CEFIXBT_UTILS_HEADER

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Matrix>
#include <osg/Quat>
#include <osg/Array>

#include "btBulletDynamicsCommon.h"
#include <cefix/Log.h>

namespace cefixbt {

	template <class T>
	class RefCounted : public osg::Referenced {
		public:
			RefCounted(T* t) : osg::Referenced(), _t(t) {}
		protected:
			virtual ~RefCounted() { if (_t) delete _t; }
		private:
			T* _t;
	};
	
	inline static btVector3 asBtVector3(const osg::Vec3& v) { return btVector3(v[0], v[1], v[2]); }
	inline static osg::Vec3 asVec3(const btVector3& v) { return osg::Vec3(v.getX(), v.getY(), v.getZ()); }
	inline static osg::Vec4 asVec4(const btVector3& v, float alpha=1.0) { return osg::Vec4(v.getX(), v.getY(), v.getZ(), alpha); }
	inline static osg::Quat asQuat(const btQuaternion& q) { return osg::Quat(q.getAngle(), asVec3(q.getAxis() ) ); }
	inline static osg::Matrixf asMatrix(const btTransform& trans) { osg::Matrixf m; trans.getOpenGLMatrix(m.ptr()); return m; }
	inline static btTransform asBtTransform(const osg::Matrixf& m) { btTransform trans; trans.setFromOpenGLMatrix(m.ptr()); return trans; }
	
	btTriangleMesh* computeBtTriMeshCollisionShape(osg::Node* node);
	osg::Vec3Array* getVertices(osg::Node* node);
}
#endif