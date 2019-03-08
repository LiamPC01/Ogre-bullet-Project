#ifndef PLAYER_H_
#define PLAYER_H_

/* Ogre3d Graphics*/
#include "Ogre.h"

/* Bullet3 Physics */
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

using namespace Ogre;

class Player
{
private:
  SceneNode* boxSceneNode;
  Entity* box;
  Vector3 meshBoundingBox;

  btCollisionShape* colShape;
  btRigidBody* body;

public:
  Player();
  ~Player();


  void createMesh(SceneManager* scnMgr);
  void attachToNode(Schenenode* parent);

  void setScale(float x, float y, float z);
  void setRotation(Vector3 axis, Radian angle);
  void setPosition(float x, float y, float z);
  void boundingBoxFromOgre();

  void createRigidBody()
  void addToCollisionShapes(btAlignedObjectArray<btCollisionShape*> &collisionShapes);
  void addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
  void setMass(float mass);

}


#endif
