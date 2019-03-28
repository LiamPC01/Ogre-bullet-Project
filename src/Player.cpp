#include "Player.h"

Player::Player()
{
  boxSceneNode = nullptr;
  box = nullptr;
  Vector3 meshBoundingBox(0.0f,0.0f,0.0f);

  colShape = nullptr;
  dynamicsWorld = nullptr;
}

Player::~Player()
{

}

void Player::createMesh(SceneManager* scnMgr)
{
  box = scnMgr->createEntity("cube.mesh");
}

void Player::attachToNode(SceneNode* parent)
{
  boxSceneNode = parent->createChildSceneNode();
  boxSceneNode->attachObject(box);
  boxSceneNode->setScale(1.0f,1.0f,1.0f);
  boundingBoxFromOgre();
}

void Player::setScale(float x, float y, float z)
{
    boxSceneNode->setScale(x,y,z);
}


void Player::setRotation(Vector3 axis, Radian rads)
{
  //quat from axis angle
  Quaternion quat(rads, axis);
  boxSceneNode->setOrientation(quat);
}

void Player::setPosition(float x, float y, float z)
{
  boxSceneNode->setPosition(x,y,z);
}

void Player::boundingBoxFromOgre()
{
  //get bounding box here.
  boxSceneNode->_updateBounds();
  const AxisAlignedBox& b = boxSceneNode->_getWorldAABB();
  Vector3 temp(b.getSize());
  meshBoundingBox = temp;
}

void Player::createRigidBody(float bodyMass)
{
  colShape = new btBoxShape(btVector3(meshBoundingBox.x/2.0f, meshBoundingBox.y/2.0f, meshBoundingBox.z/2.0f));

  /// Create Dynamic Objects
  btTransform startTransform;
  startTransform.setIdentity();

  Quaternion quat2 = boxSceneNode->_getDerivedOrientation();
  startTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));

  Vector3 pos = boxSceneNode->_getDerivedPosition();
  startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

  btScalar mass(bodyMass);

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

  btScalar linearDamping = 0.2f;
  btScalar angularDamping = 0.8f;

  body->setDamping(linearDamping,angularDamping);

  //Set the user pointer to this object.
  body->setUserPointer((void*)this);
}

void Player::addToCollisionShapes(btAlignedObjectArray<btCollisionShape*> &collisionShapes)
{
  collisionShapes.push_back(colShape);
}

void Player::addToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
  this->dynamicsWorld = dynamicsWorld;
  dynamicsWorld->addRigidBody(body);
}

void Player::update()
{
  btTransform trans;

  if (body && body->getMotionState())
  {
    body->getMotionState()->getWorldTransform(trans);
    btQuaternion orientation = trans.getRotation();

    boxSceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    boxSceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
  }
}

void Player::forward()
{
    float forwardForce = 100.0f;
    btVector3 fwd(0.0f,0.0f,forwardForce);
    btVector3 push;

    btTransform trans;

    if (body && body->getMotionState())
    {
        body->getMotionState()->getWorldTransform(trans);
        btQuaternion orientation = trans.getRotation();

        push = quatRotate(orientation, fwd);

        body->activate();
        body->applyCentralForce(push);
    }
}

void Player::turnRight()
{
    float turningForce = 20.0f;
    btVector3 right(5.0f,0.0f,0.0f);
    btVector3 turn;

    btTransform trans;

    if (body)// && body->getMotionState())
    {
        body->getMotionState()->getWorldTransform(trans);
        btQuaternion orientation = trans.getRotation();
        btVector3 front(trans.getOrigin());

        //use original bounding mesh to get the front center
        front += btVector3(0.0f,0.0f,meshBoundingBox.z/2);

        turn = quatRotate(orientation, right);

        //took this out, can't turn if your not moving.
        //body->activate();

        //better - only turn if we're moving.
        //not ideal, if sliding sideways will keep turning.
        if(body->getLinearVelocity().length() > 0.0f)
            body->applyForce(turn,front);
    }
}

