#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreApplicationContext.h"
#include "OgreCameraMan.h"


using namespace Ogre;
using namespace OgreBites;

class Game : public ApplicationContext, public InputListener
{
private:
    SceneManager* scnMgr;


public:
	Game();
	virtual ~Game();

	void setup();

	void setupCamera();

	void setupBoxMesh();

	void setupLights();

	bool keyPressed(const KeyboardEvent& evt);
	bool mouseMoved(const MouseMotionEvent& evt);

	bool frameStarted (const FrameEvent &evt);
	bool frameEnded(const FrameEvent &evt);

};
