#include <iostream>

#include "btBulletDynamicsCommon.h"

int main (void)
{
  btDefaultCollisionConfiguration* collisionConf =
    new btDefaultCollisionConfiguration();
  btCollisionDispatcher* dispatcher =
    new btCollisionDispatcher(collisionConf);
  btBroadphaseInterface* overlappingPairCache =
    new btDbvtBroadphase();
  btSequentialImpulseConstraintSolver* solver =
    new btSequentialImpulseConstraintSolver();

  btDiscreteDynamicsWorld* dynamicsWorld =
    new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache,
				solver, collisionConf);
  {
    btCollisionShape* boundShape =
      new btBoxShape(btVector3(btScalar(12.), btScalar(12.), btScalar(1.)));

    btTransform boundTransform;
    boundTransform.setIdentity();
    boundTransform.setOrigin(btVector3(0, 0, -0.5));
    btScalar mass(0.);
    btVector3 localInertia(0, 0, 0);

    btDefaultMotionState* myMotionState =
      new btDefaultMotionState(boundTransform);
    btRigidBody::btRigidBodyConstructionInfo
      rbInfo(mass, myMotionState, boundShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);
    dynamicsWorld->addRigidBody(body);
  }

  {
    btCollisionShape* objShape = new btSphereShape(btScalar(0.001));
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(6, 6, 0));
    btScalar mass(0.);
    btVector3 localInertia(0, 0, 0);

    btDefaultMotionState* myMotionState =
      new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo
      rbInfo(mass, myMotionState, objShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);
    dynamicsWorld->addRigidBody(body);
  }

  for (int i = 0; i < 150; i++)
  {
    dynamicsWorld->stepSimulation(1.f / 60.f, 10);

    //print positions of all objects
    for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
    {
      btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
      btRigidBody* body = btRigidBody::upcast(obj);
      btTransform trans;
      if (body && body->getMotionState())
	    body->getMotionState()->getWorldTransform(trans);
      else
	trans = obj->getWorldTransform();
      printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()),
	     float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
    }
  }

  std::cout << "Hello" << std::endl;
  return 0;
}
