#ifndef PLAYER_H_
#define PLAYER_H_

/* Ogre3d Graphics*/
#include "Ogre.h"

/* Bullet3 Physics */
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

using namespace Ogre;

/** Example player class.
* Written to illistrate the connection of Ogre/Bullet.
* Essentially just a wrapper around the cube object setup code.
*/
class Player
{
private:
  SceneNode* boxSceneNode;     /**< Scene graph node */
  Entity* box;                 /**< Mesh entity */
  Vector3 meshBoundingBox;     /**< Size of the bounding mesh from ogre */

  btCollisionShape* colShape;  /**< Collision shape, describes the collision boundary */
  btRigidBody* body;           /**< Rigid Body */
  btDiscreteDynamicsWorld* dynamicsWorld;  /**< physics/collision world */

public:
  Player();
  ~Player();

  /**
  * Creates the mesh.
  * @param scnMgr the Ogre SceneManager. 
  */
  void createMesh(SceneManager* scnMgr);
  void attachToNode(SceneNode* parent);

  void setScale(float x, float y, float z);
  void setRotation(Vector3 axis, Radian angle);
  void setPosition(float x, float y, float z);
  void boundingBoxFromOgre();

  void createRigidBody(float mass);
  void addToCollisionShapes(btAlignedObjectArray<btCollisionShape*> &collisionShapes);
  void addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
  void setMass(float mass);

  void update();

};


#endif
