/*
 *  Utils.cpp
 *  bullet
 *
 *  Created by Stephan Huber on 12.10.08.
 *  Copyright 2008 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#include "Utils.h"
#include <osg/NodeVisitor>
#include <osg/Geometry>
#include <osg/TriangleFunctor>
#include <osg/Geode>

namespace cefixbt {

struct CollectVertices 
{
	void set(btTriangleMesh* mesh, osg::Vec3Array* vertices, const osg::Matrix& m) {_mesh = mesh; _vertices = vertices; _m = m;}
	
	inline void operator()(const osg::Vec3& p1, const osg::Vec3& p2, const osg::Vec3& p3, bool treatVertexDataAsTemporary) {
		if (_mesh)
			_mesh->addTriangle( asBtVector3(_m*p1), asBtVector3(_m*p2), asBtVector3(_m*p3) );
			
		if (_vertices) {
			_vertices->push_back(_m*p1);
			_vertices->push_back(_m*p2);
			_vertices->push_back(_m*p3);
		}
	}
	
	btTriangleMesh* _mesh;
	osg::Vec3Array* _vertices;
	osg::Matrix		_m;
};


class CollectVerticesVisitor : public osg::NodeVisitor {
	public:
		
		CollectVerticesVisitor(btTriangleMesh* mesh) : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), _mesh(mesh), _vertices(NULL) {}
		CollectVerticesVisitor(osg::Vec3Array* vertices) : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), _mesh(NULL), _vertices(vertices) {}
		
		void processGeometry(osg::Geometry* geo, const osg::Matrix& m) {
			osg::TriangleFunctor<CollectVertices> visitor;
			visitor.set(_mesh, _vertices, m);
			geo->accept(visitor);
		}

		void apply( osg::Geode &node )
		{
			osg::Matrix m = osg::computeLocalToWorld(getNodePath());
			unsigned int count = node.getNumDrawables();
			for ( unsigned int i = 0; i < count; ++i )
			{
				osg::Geometry *g = node.getDrawable( i )->asGeometry();
				if ( g != NULL )
				{
					processGeometry(g,m);				
				}
			}
		}
	private:
		btTriangleMesh* _mesh;
		osg::Vec3Array* _vertices;
};



btTriangleMesh* computeBtTriMeshCollisionShape(osg::Node* node) 
{
	btTriangleMesh* mesh = new btTriangleMesh;
	CollectVerticesVisitor visitor(mesh);
	
	node->accept( visitor );
	// std::cout << "computeBtTriMeshCollisionShape " << mesh->getNumTriangles() << std::endl;
	return mesh;
	
}


osg::Vec3Array* getVertices(osg::Node* node) 
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
	CollectVerticesVisitor visitor( vertices.get() );
	
	node->accept( visitor );
	
	return vertices.release();

}


}