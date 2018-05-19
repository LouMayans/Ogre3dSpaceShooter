/*
-----------------------------------------------------------------------------
Filename:    TopScroller.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TOPSCROLLER_h_
#define __TOPSCROLLER_h_

#include "BaseApplication.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <vector>
#include <string>
#include <iostream>
#include "CollisionObject.h"
#include "AIMove.h"

//---------------------------------------------------------------------------
bool checkCollisionBetween(Ogre::SceneNode* left, Ogre::SceneNode* right);
CollisionObject* createNode(std::string name, Ogre::Vector3 position, Ogre::Real, Ogre::Vector3 scale, std::string mesh, Ogre::SceneNode* parent, int id);


class TopScroller : public BaseApplication
{
public:
	static TopScroller* instance();
    virtual ~TopScroller(void);

	Ogre::SceneManager* getMSceneManager()		const { return mSceneMgr; }
	Ogre::SceneNode*	getCameraController()	const { return cameraContainer; }
	CEGUI::Window*		getEndWindow()			const { return endMenu; }
	OIS::Keyboard*		getKeyboard()			const { return mKeyboard; }
	Ogre::Real			getPlayPlane()			const { return playPlanePos; }
	bool				isPaused()				const { return paused; }

	void pause(){ paused = true; };
	void resume(){ paused = false; }
	
	void addHighscore(int points) { highScore += points; }
	void addParticleNode(std::pair<Ogre::SceneNode*, float> partNode) { particleNodes.push_back(partNode); };
private:
	//Singleton
	TopScroller(void);
	TopScroller(const TopScroller&){}
	TopScroller& operator=(const TopScroller&){}
	static TopScroller * instance_;

    virtual void createScene(void);
	virtual void createFrameListener();
	virtual void destroyScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void CEGUIbootstrap();
	void createGround();
	void createLight();
	void setupCamera();
	void createUI();
	void createStartMenu();
	void createPauseMenu();
	void createEndMenu();
	void pauseKeyDown();
	void debugMove(const Ogre::FrameEvent& fe);
	void startSequence(const Ogre::FrameEvent& fe);
	void updateParticleNodes(const Ogre::FrameEvent& fe);
	bool beginGame(const CEGUI::EventArgs&);
	bool debugMode(const CEGUI::EventArgs&);
	bool processUnbufferedInput(const Ogre::FrameEvent& fe);

	int highScore = 0;
	float gameTimer = 0.0f;
	Ogre::Real playPlanePos = 500;

	Ogre::SceneNode* cameraContainer;
	bool paused;
	bool menuOpen;
	bool mouseDownLastFrame;
	bool isDebugMode = false;
	bool startSequenceBool = false;
	
	bool restart(const CEGUI::EventArgs& e);

	CEGUI::OgreRenderer* renderer;
	CEGUI::Window* endMenu = nullptr;
	CEGUI::Window* pauseMenu = nullptr;
	CEGUI::Window* startMenu = nullptr;
	CEGUI::Window* timerWindow = nullptr;
	CEGUI::Window* scoreWindow = nullptr;

	std::pair<Ogre::Vector3, Ogre::Quaternion> cameraInfo;
	std::vector<std::pair<Ogre::SceneNode*, float>> particleNodes;
	

};

//---------------------------------------------------------------------------

#endif // #ifndef __TopScroller_h_

//---------------------------------------------------------------------------
