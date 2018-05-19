#include "JetController.h"
#include "EnemyContainer.h"

JetController::JetController(void)
{
}
JetController* JetController::instance_ = nullptr;
JetController* JetController::instance()
{
	if (instance_ == nullptr)
		instance_ = new JetController;
	return instance_;
}
void JetController::Start()
{
	JetInfoWindow();
	TS = TopScroller::instance();
	CreateJet(Ogre::Degree(0), Ogre::Vector3(.27f, .27f, .27f), "razor.mesh");
}
void JetController::Update(const Ogre::FrameEvent& fe)
{
	updateBullets(fe);
	input(fe);

}
bool JetController::takeDamage(Ogre::SceneNode* left, int damage)
{

	if (checkCollisionBetween(left, jetObject->node))
	{
		jetHealth -= damage;

		if (jetHealth <= 0)
		{
			death();

		}
		return true;
	}
	return false;

}
void JetController::death()
{
	TS->pause();
	TS->getEndWindow()->show();
	TS->getEndWindow()->setText("You Lose");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
}
void JetController::input(const Ogre::FrameEvent& fe)
{
	shotTimer += fe.timeSinceLastFrame;

	if (shotTimer >= timeBetweenShots)
	{
		shotTimer = 0;
		if (TS->getKeyboard()->isKeyDown(OIS::KC_A))
		{
			shoot(fe);
		}
	}

	Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;
	if (TS->getKeyboard()->isKeyDown(OIS::KC_UP))
		dirVec.z += moveSpeed;
	if (TS->getKeyboard()->isKeyDown(OIS::KC_DOWN))
		dirVec.z -= moveSpeed;
	if (TS->getKeyboard()->isKeyDown(OIS::KC_LEFT))
		dirVec.x += moveSpeed;
	if (TS->getKeyboard()->isKeyDown(OIS::KC_RIGHT))
		dirVec.x -= moveSpeed;


	if (Ogre::Math::IAbs((jetObject->node->getPosition().x + dirVec.x *fe.timeSinceLastFrame)) < boundaries.x)
		jetObject->node->translate(Ogre::Vector3(dirVec.x, 0, 0)* fe.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	if (Ogre::Math::IAbs((jetObject->node->getPosition().z + dirVec.z *fe.timeSinceLastFrame)) < boundaries.z)
		jetObject->node->translate(Ogre::Vector3(0, 0, dirVec.z)* fe.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

	jetInfo->setText("Health = " + std::to_string(jetHealth));
}
void JetController::CreateJet(Ogre::Degree degree, Ogre::Vector3 scale, std::string name)
{

	Ogre::Entity* jetEntity = TS->getMSceneManager()->createEntity(name);
	Ogre::SceneNode* jetNode = TS->getCameraController()->createChildSceneNode("Jet");
	jetEntity->setCastShadows(false);
	jetNode->attachObject(jetEntity);
	jetNode->setScale(scale);
	jetNode->setPosition(0, 0, 0);
	jetNode->rotate(Ogre::Quaternion(degree, Ogre::Vector3::UNIT_Y));

	jetObject = new CollisionObject(jetNode, jetEntity, 0);

	createJetThrust();
}
void JetController::createJetThrust()
{
	Ogre::ParticleSystem* jetThrustParticle = TS->getMSceneManager()->createParticleSystem("JetEngine1", "Examples/JetEngine1");
	jetThrustParticle->setDefaultHeight(15);
	jetThrustParticle->setDefaultWidth(15);
	jetThrustParticle->getEmitter(0)->setTimeToLive(.05);


	Ogre::SceneNode* jetThrust = jetObject->node->createChildSceneNode("jetThrust");
	jetThrust->attachObject(jetThrustParticle);
	jetThrust->setPosition(0, 0, -50);
	//jetThrust->setScale(.6, .6, .6);
	jetThrust->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(90));
}
void JetController::shoot(const Ogre::FrameEvent& fe)
{
	bulletCount += 1;
	Ogre::Entity* bulletEntity = TS->getMSceneManager()->createEntity("Barrel.mesh");
	Ogre::SceneNode* bullet = TS->getCameraController()->createChildSceneNode("bullet" + std::to_string(bulletCount));
	bullet->attachObject(bulletEntity);
	bullet->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(90));
	bullet->setPosition(jetObject->node->getPosition() + Ogre::Vector3(10, 0, 0));
	bullet->scale(.25, 1.5, .25);

	bulletCount += 1;
	Ogre::Entity* bulletEntity2 = TS->getMSceneManager()->createEntity("Barrel.mesh");
	Ogre::SceneNode* bullet2 = TS->getCameraController()->createChildSceneNode("bullet" + std::to_string(bulletCount));
	bullet2->attachObject(bulletEntity2);
	bullet2->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(90));
	bullet2->setPosition(jetObject->node->getPosition() + Ogre::Vector3(-10, 0, 0));
	bullet2->scale(.25, 1.5, .25);

	bulletObjects.push_back(new CollisionObject(bullet, bulletEntity, 1));
	bulletObjects.push_back(new CollisionObject(bullet2, bulletEntity2, 1));

}
void JetController::updateBullets(const Ogre::FrameEvent& fe)
{
	for (std::vector<CollisionObject*>::iterator it = bulletObjects.begin(); it != bulletObjects.end();)
	{
		if ((*it)->node->getPosition().z >= boundaries.z)
		{
			TS->getMSceneManager()->destroySceneNode((*it)->node->getName());
			it = bulletObjects.erase(it);
		}
		else
		{
			if (EnemyContainer::instance()->enemyHit((*it)->node, bulletDamage))
			{
				TS->getMSceneManager()->destroySceneNode((*it)->node->getName());
				it = bulletObjects.erase(it);
				continue;
			}

			//https://svn.cs.kent.edu/gpg/trac/wiki/acarlet1
			(*it)->node->translate(Ogre::Vector3(0, 1, 0) * bulletSpeed * fe.timeSinceLastFrame, Ogre::Node::TransformSpace::TS_LOCAL);
			++it;
			continue;
		}
	}
}
void JetController::JetInfoWindow()
{
	jetInfo = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "jetInfo");
	jetInfo->setPosition(CEGUI::UVector2(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.9, 0)));
	jetInfo->setSize(CEGUI::USize(CEGUI::UDim(0, 200), CEGUI::UDim(0, 75)));
	jetInfo->setText("Health");
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(jetInfo);
	jetInfo->hide();

}