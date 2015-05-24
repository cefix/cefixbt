/*
 *  RigidBody.h
 *  bullet
 *
 *  Created by Stephan Huber on 17.11.08.
 *  Copyright 2008 Stephan Maximilian Huber, digital mind. All rights reserved.
 *
 */

#ifndef CEFIXBT_RIGID_BODY_HEADER
#define CEFIXBT_RIGID_BODY_HEADER

#include <osg/Referenced>
#include <cefixbt/Utils.h>
#include <set>


namespace cefixbt {

class ContactCallback : public osg::Referenced {
public:
    typedef std::set<const btCollisionObject*> Contacts;
    
    ContactCallback() : osg::Referenced(), _contacts() {}
    
    void addContactWith(const btCollisionObject* o, btManifoldPoint& point) {
        if (_contacts.find(o) == _contacts.end()) {
            newContactWith(o, point);
            _contacts.insert(o);
        }
    }
    
    void removeContactWith(const btCollisionObject* o) {
        _contacts.erase(o);
        lostContactWith(o);
    }
    
    unsigned int getNumContacts() { return _contacts.size(); }
    Contacts& getContacts() { return _contacts; }
    const Contacts& getContacts() const { return _contacts; }

    virtual void newContactWith(const btCollisionObject* o, const btManifoldPoint& contact_point) = 0;
    virtual void lostContactWith(const btCollisionObject* o) = 0;
    
protected:
    Contacts _contacts;
};

class RigidBody : public btRigidBody, public osg::Referenced {

public:
	RigidBody(const btRigidBodyConstructionInfo& constructionInfo)
	:	btRigidBody(constructionInfo), osg::Referenced() {}

     void setContactCallback(ContactCallback* cb) 
    {
        setCollisionFlags(getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
        _cb = cb; 
        setUserPointer(cb);
    }
    
    void clearContactCallback() {
        setCollisionFlags(getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
        _cb = NULL;
        setUserPointer(NULL);
    }


private:
	RigidBody(	btScalar mass, btMotionState* motionState, btCollisionShape* collisionShape, const btVector3& localInertia=btVector3(0,0,0))
	:	btRigidBody(mass, motionState, collisionShape, localInertia), osg::Referenced() {}
    
    virtual ~RigidBody() {
        // cefix::cefix::log::info("RigidBody") << "d'tor called" << std::endl;
    }
    
    osg::ref_ptr<ContactCallback> _cb;
};

}


#endif