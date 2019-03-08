#include "Player.h"

void Player::Player()
{
  boxSceneNode = nullptr;
  box = nullptr;
  Vector3 meshBoundingBox(0.0f,0.0f,0.0f);

  colShape = nullptr;
  btRigidBody = nullptr;
}

void Player::~Player()
{

}

void Player::createMesh(SceneManager* scnMgr)
{
  box = scnMgr->createEntity("cube.mesh");
  thisSceneNode->setScale(1.0f,1.0f,1.0f);
  boundingBoxFromOgre();
}

void Player::attachToNode(Schenenode* parent)
{
  parent->createChildSceneNode()->attachObject(box);
}

void Player::setScale(float x, float y, float z)
{
    thisSceneNode->setScale(x,y,z);
}


void Player::setRotation(Vector3 axis, Radian angle)
{
  //quat from axis angle
  Quaternion quat(rads, axis);
  thisSceneNode->setOrientation(quat);
}

void Player::setPosition(float x, float y, float z)
{
  thisSceneNode->setPosition(x,y,z);
}

void Player::boundingBoxFromOgre()
{
  //get bounding box here.
  thisSceneNode->_updateBounds();
  const AxisAlignedBox& b = thisSceneNode->_getWorldAABB();
  Vector3 temp(b.getSize());
  meshBoundingBox = temp;
}

void Player::createRigidBody(float mass)
{
  colShape = new btBoxShape(btVector3(meshBoundingBox.x/2.0f, meshBoundingBox.y/2.0f, meshBoundingBox.z/2.0f));

  /// Create Dynamic Objects
  btTransform startTransform;
  startTransform.setIdentity();

  Quaternion quat2 = thisSceneNode->_getDerivedOrientation();
  startTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));

  Vector3 pos = thisSceneNode->_getDerivedPosition();
  startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

  btScalar mass(mass);

  //rigidbody is dynamic if and only if mass is non zero, otherwise static
  bool isDynamic = (mass != 0.f);

  btVector3 localInertia(0, 0, 0);
  if (isDynamic)
  {
      // Debugging
      //std::cout << "I see the cube is dynamic" << std::endl;
      colShape->calculateLocalInertia(mass, localInertia);
  }

  //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
  btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
  body = new btRigidBody(rbInfo);

  //Set the user pointer to this object.
  body->setUserPointer((void*)this);
}

void Player::addToCollisionShapes(btAlignedObjectArray<btCollisionShape*> &collisionShapes)
{
  collisionShapes->push_back(colShape);
}

void Player::addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
{
  dynamicsWorld->addRigidBody(body);
}
