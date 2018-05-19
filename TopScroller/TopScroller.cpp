/*
-----------------------------------------------------------------------------
Filename:    TopScroller.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
using namespace Ogre;
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/
#include "JetController.h"
#include "EnemyContainer.h"

#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>


bool restart(const CEGUI::EventArgs& e);
//---------------------------------------------------------------------------
TopScroller::TopScroller(void){}

TopScroller* TopScroller::instance_ = nullptr;
TopScroller* TopScroller::instance(){
	if (instance_ == nullptr)
		instance_ = new TopScroller;
	return instance_;
}

//---------------------------------------------------------------------------
TopScroller::~TopScroller(void)
{
}

bool checkCollisionBetween(Ogre::SceneNode* left, Ogre::SceneNode* right)
{
	if (left == nullptr || right == nullptr)
		return false;

	Ogre::AxisAlignedBox leftBox = static_cast<Ogre::Entity*>(left->getAttachedObject(0))->getWorldBoundingBox();
	Ogre::AxisAlignedBox rightBox = static_cast<Ogre::Entity*>(right->getAttachedObject(0))->getWorldBoundingBox();
	if (leftBox.intersects(rightBox))
		return true;
	return false;
}
void TopScroller::createScene(void)
{
	paused = true;
	menuOpen = false;
	srand(time(NULL));
	
	CEGUIbootstrap();
	setupCamera();

	pause();


	createUI();

	createLight();
	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");

	createGround();

	JetController::instance()->Start();
	EnemyContainer::instance()->start();

}
void TopScroller::createFrameListener()
{
	BaseApplication::createFrameListener();
	system("CLS");
}
bool TopScroller::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	bool ret = BaseApplication::frameRenderingQueued(fe);

	CEGUI::System::getSingleton().injectTimePulse(fe.timeSinceLastFrame);

	if (startMenu->isVisible())
		return ret;

	if (startSequenceBool && paused)
	{
		startSequence(fe);
		return ret;
	}

	if (!paused)
	{
		gameTimer += fe.timeSinceLastFrame;
		timerWindow->setText(std::to_string((int)gameTimer));

		if (cameraContainer->getPosition().z <= 8900) //89 seconds
			cameraContainer->translate(Ogre::Vector3(0, 0, 1) * 100 * fe.timeSinceLastFrame, Ogre::Node::TransformSpace::TS_LOCAL);
		else
		{
			static OgreHeadBoss* boss = nullptr;
			if (boss == nullptr)
			{
				
				EnemyContainer::instance()->addEnemy(boss = new OgreHeadBoss(4000, "boss", Ogre::Vector3(0, playPlanePos, 9300)));
			}
			else if (boss->isDead())
			{
				pause();
				endMenu->show();
				endMenu->setText("You Win!");
				CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
			}
		
		}


		JetController::instance()->Update(fe);
		EnemyContainer::instance()->update(fe);
		updateParticleNodes(fe);
		scoreWindow->setText(std::to_string(highScore));
	}



	if (!processUnbufferedInput(fe))
		return ret;

	return ret;
}
bool TopScroller::processUnbufferedInput(const Ogre::FrameEvent& fe)
{
	pauseKeyDown();
	if (isDebugMode && !menuOpen)
		debugMove(fe);
	return true;
}
void TopScroller::createLight()
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	Ogre::Vector3 lightdir(0.55f, -0.3f, 0.75f);
	lightdir.normalise();
	Ogre::Light* light = mSceneMgr->createLight("TestLight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(lightdir);
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));


}
void TopScroller::updateParticleNodes(const Ogre::FrameEvent& fe)
{
	for (std::vector<std::pair<Ogre::SceneNode*, float>>::iterator it = particleNodes.begin(); it != particleNodes.end();)
	{
		(*it).second += fe.timeSinceLastFrame;

		if ((*it).second >= 1)
		{
			mSceneMgr->destroySceneNode((*it).first);
			it = particleNodes.erase(it);
		}
		else
			++it;
	}
}
void TopScroller::pauseKeyDown()
{
	static bool pKeyDownLastFrame = false;
	if (!mKeyboard->isKeyDown(OIS::KeyCode::KC_P))
		pKeyDownLastFrame = false;

	if (mKeyboard->isKeyDown(OIS::KeyCode::KC_P) && pKeyDownLastFrame == false)
	{
		pKeyDownLastFrame = true;
		if (!menuOpen)
		{
			menuOpen = true;
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
			pauseMenu->show();
			pause();
		}
		else
		{
			menuOpen = false;
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
			pauseMenu->hide();
			if (!isDebugMode)
			{
				resume();
			}
		}
	}
}
CollisionObject* createNode(std::string name, Ogre::Vector3 position,Ogre::Real rotationY, Ogre::Vector3 scale, std::string mesh, Ogre::SceneNode* parent, int id)
{
	Ogre::Entity* testEntity = TopScroller::instance()->getMSceneManager()->createEntity(mesh);
	Ogre::SceneNode* testNode = parent->createChildSceneNode(name);
	testEntity->setCastShadows(false);
	testNode->attachObject(testEntity);
	testNode->setScale(scale);
	testNode->setPosition(position);
	testNode->setOrientation(1, 0, rotationY, 0);
	testNode->setInitialState();
	//testNode->rotate(Ogre::Quaternion(rotation, Ogre::Vector3::UNIT_Y));

	return new CollisionObject(testNode, testEntity, id);
}
void TopScroller::createUI()
{
	createStartMenu();
	createPauseMenu();
	createEndMenu();
	timerWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Label", "timerWindow");
	timerWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(.25, 0), CEGUI::UDim(.1, 0)));
	timerWindow->setSize(CEGUI::USize(CEGUI::UDim(.5f, 0), CEGUI::UDim(.05f, 0)));
	timerWindow->setText("0");
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(timerWindow);

	scoreWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Label", "scoreWindow");
	scoreWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(.25, 0), CEGUI::UDim(.05, 0)));
	scoreWindow->setSize(CEGUI::USize(CEGUI::UDim(.5f, 0), CEGUI::UDim(.05f, 0)));
	scoreWindow->setText("0");
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(scoreWindow);


}
void TopScroller::createEndMenu()
{
	endMenu = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/FrameWindow", "endMenu");
	endMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(.25, 0), CEGUI::UDim(.25, 0)));
	endMenu->setSize(CEGUI::USize(CEGUI::UDim(.5f, 0), CEGUI::UDim(.5f, 0)));
	endMenu->setText("Start Menu");
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(endMenu);

	CEGUI::Window* restartButton = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "RetartButtonEndMenu");
	restartButton->setPosition(CEGUI::UVector2(CEGUI::UDim(.25, 0), CEGUI::UDim(.125, 0)));
	restartButton->setSize(CEGUI::USize(CEGUI::UDim(.5f, 0), CEGUI::UDim(.25f, 0)));
	restartButton->setText("Restart");
	endMenu->addChild(restartButton);
	endMenu->hide();
	restartButton->subscribeEvent(CEGUI::PushButton::EventClicked, &TopScroller::restart, this);
}
void TopScroller::createStartMenu()
{
	startMenu = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/FrameWindow", "StartMenu");
	startMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(.25, 0), CEGUI::UDim(.25, 0)));
	startMenu->setSize(CEGUI::USize(CEGUI::UDim(.5f, 0), CEGUI::UDim(.5f, 0)));
	startMenu->setText("Start Menu");
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(startMenu);

	CEGUI::Window* infoWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Label", "infoWindow");
	infoWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(.5, 0)));
	infoWindow->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(.3, 0)));
	infoWindow->setText("Press P to pause\n Move with the arrow keys\n shoot with A\n DebugMode rotate with arrow Keys and move while holding shift");
	startMenu->addChild(infoWindow);

	CEGUI::Window* startButton = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "StartButton");
	startButton->setPosition(CEGUI::UVector2(CEGUI::UDim(.25, 0), CEGUI::UDim(.125, 0)));
	startButton->setSize(CEGUI::USize(CEGUI::UDim(.5f, 0), CEGUI::UDim(.25f, 0)));
	startButton->setText("Start");
	startMenu->addChild(startButton);
	startButton->subscribeEvent(CEGUI::PushButton::EventClicked, &TopScroller::beginGame, this);
}
bool TopScroller::beginGame(const CEGUI::EventArgs& arg)
{
	startMenu->hide();
	gameTimer = 0.f;
	startSequenceBool = true;
	JetController::instance()->getJetObject()->node->setPosition(-50 , 0, 0);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
	JetController::instance()->jetInfoWindowShow(true);
	return true;
}
void TopScroller::startSequence(const Ogre::FrameEvent& fe)
{
	static float startSequenceTimer = 0.0;
	startSequenceTimer += fe.timeSinceLastFrame;

	if (startSequenceTimer <= .7f)
	{
		
		Ogre::Real timeLapsed = startSequenceTimer / .7f;
		//static Ogre::Real timeCovered = 0;
		Ogre::Real LerpedPositionX = Ogre::Math::lerp(-350, 0, timeLapsed);
		JetController::instance()->getJetObject()->node->setPosition(LerpedPositionX,0,0);
		JetController::instance()->getJetObject()->node->rotate(Ogre::Vector3(0, 1, 0), Ogre::Degree(360 / .7f) * fe.timeSinceLastFrame);
	}
	else if (startSequenceTimer <= 1)
	{
		resume();
		startSequenceBool = false;
	}

}
void TopScroller::createPauseMenu()
{
	pauseMenu = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/FrameWindow", "PauseMenu");
	pauseMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(.25, 0), CEGUI::UDim(.25, 0)));
	pauseMenu->setSize(CEGUI::USize(CEGUI::UDim(.5f, 0), CEGUI::UDim(.5f, 0)));
	pauseMenu->setText("Pause Menu");
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(pauseMenu);

	CEGUI::Window* debugButton = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "DebugButton");
	debugButton->setPosition(CEGUI::UVector2(CEGUI::UDim(.25, 0), CEGUI::UDim(.125, 0)));
	debugButton->setSize(CEGUI::USize(CEGUI::UDim(.5f, 0), CEGUI::UDim(.25f, 0)));
	debugButton->setText("Debug Mode Off");
	pauseMenu->addChild(debugButton);
	debugButton->subscribeEvent(CEGUI::PushButton::EventClicked, &TopScroller::debugMode, this);


	pauseMenu->hide();
}
bool TopScroller::debugMode(const CEGUI::EventArgs& arg)
{
	if (isDebugMode)
	{
		std::cout << "Debug Mode Off\n";
		isDebugMode = false;
		pauseMenu->getChild("DebugButton")->setText("Debug Mode Off");
		mCamera->setPosition(cameraInfo.first);
		mCamera->setOrientation(cameraInfo.second);
	}
	else
	{
		std::cout << "Debug Mode On\n";
		cameraInfo.first = mCamera->getPosition();
		cameraInfo.second = mCamera->getOrientation();
		isDebugMode = true;
		pauseMenu->getChild("DebugButton")->setText("Debug Mode On");
		pauseMenu->hide();
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
		menuOpen = false;
	}

	return true;
}
void TopScroller::debugMove(const Ogre::FrameEvent& fe)
{
	Ogre::Real moveSpeed = 350;
	Ogre::Real rotateSpeed = 120;
	Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;

	if (mKeyboard->isKeyDown(OIS::KC_LSHIFT) && mKeyboard->isKeyDown(OIS::KC_LEFT))
		dirVec.x -= moveSpeed;
	else if (mKeyboard->isKeyDown(OIS::KC_LEFT))
	{
		mCamera->yaw(Ogre::Degree(rotateSpeed * fe.timeSinceLastFrame));
	}
	if (mKeyboard->isKeyDown(OIS::KC_LSHIFT) && mKeyboard->isKeyDown(OIS::KC_RIGHT))
		dirVec.x += moveSpeed;
	else if (mKeyboard->isKeyDown(OIS::KC_RIGHT))
	{
		mCamera->yaw(Ogre::Degree(-rotateSpeed * fe.timeSinceLastFrame));
	}
	if (mKeyboard->isKeyDown(OIS::KC_LSHIFT) && mKeyboard->isKeyDown(OIS::KC_UP))
		dirVec.z -= moveSpeed;
	else if (mKeyboard->isKeyDown(OIS::KC_UP))
	{
		mCamera->pitch(Ogre::Degree(rotateSpeed * fe.timeSinceLastFrame));
	}
	if (mKeyboard->isKeyDown(OIS::KC_LSHIFT) && mKeyboard->isKeyDown(OIS::KC_DOWN))
		dirVec.z += moveSpeed;
	else if (mKeyboard->isKeyDown(OIS::KC_DOWN))
	{
		mCamera->pitch(Ogre::Degree(-rotateSpeed * fe.timeSinceLastFrame));
	}

	mCamera->moveRelative(dirVec * fe.timeSinceLastFrame);
}
bool TopScroller::mouseMoved(const OIS::MouseEvent &arg)
{
	if (!paused)
		return true;
	if (CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel)) return true;

	mCameraMan->injectMouseMove(arg);
	return true;
}
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;
		break;

	case OIS::MB_Right:
		return CEGUI::RightButton;
		break;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;
		break;

	default:
		return CEGUI::LeftButton;
		break;
	}
}
bool TopScroller::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id))) return true;
	mCameraMan->injectMouseDown(arg, id);
	return true;
}
bool TopScroller::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id))) return true;
	mCameraMan->injectMouseUp(arg, id);
	return true;
}
void TopScroller::createGround()
{
	Ogre::Entity* groundEntity = mSceneMgr->createEntity("cube.mesh");
	Ogre::MaterialPtr mat = groundEntity->getSubEntity(0)->getMaterial();
	mat->getTechnique(0)->getPass(0)->setAmbient(.4, .1, 0);
	Ogre::SceneNode* ground = mSceneMgr->getRootSceneNode()->createChildSceneNode("boxNode");
	ground->attachObject(groundEntity);
	Ogre::AxisAlignedBox aab = groundEntity->getBoundingBox();
	Ogre::Vector3 sizeOfBox = Ogre::Vector3(aab.getSize().x, aab.getSize().y, aab.getSize().z) * (1.0f - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
	Ogre::Vector3 boxScale(7, 1, 200);
	ground->scale(boxScale);
	ground->setPosition(0, 400, sizeOfBox.z / 2 * boxScale.z - 300);
}
void TopScroller::setupCamera()
{
	////////////////////////////
	//setting camera position start in the air and looking straight down.
	mCamera->setPosition(0, 1000, 0);
	mCamera->setNearClipDistance(.1f);
	bool infiniteClip =
		mRoot->getRenderSystem()->getCapabilities()->hasCapability(
		Ogre::RSC_INFINITE_FAR_PLANE);
	if (infiniteClip)
		mCamera->setFarClipDistance(0);
	else
		mCamera->setFarClipDistance(50000);

	mCamera->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(-90)); //Look down
	mCamera->rotate(Ogre::Vector3(0, 1, 0), Ogre::Degree(180));
	cameraContainer = mSceneMgr->getRootSceneNode()->createChildSceneNode("cameraContainer");
	cameraContainer->attachObject(mCamera);
	cameraContainer->setPosition(0, 500, 0);
	mCamera->setPosition(0, mCamera->getPosition().y - 500, 0);
}
void TopScroller::destroyScene()
{

}
void TopScroller::CEGUIbootstrap()
{
	renderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	CEGUI::Window* myRoot = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "_MasterRoot");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

}

bool restartBool = false;
bool TopScroller::restart(const CEGUI::EventArgs& e)
{
	//restartBool = true;
	mShutDown = true;
	return true;
}
void showWin32Console()
{
	static const WORD MAX_CONSOLE_LINES = 500;
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;
	// allocate a console for this app
	AllocConsole();
	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
		coninfo.dwSize);
	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);
	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);
	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TopScroller* app = TopScroller::instance();
		showWin32Console();
        try {
            app->go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
		FreeConsole();
        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
