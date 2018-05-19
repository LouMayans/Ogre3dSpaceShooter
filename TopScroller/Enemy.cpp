#include "JetController.h"
#include "EnemyContainer.h"


bool Enemy::canStartUpdate()
{
	if (TopScroller::instance()->getCameraController()->getPosition().z <= _startPosition.z - 200)
		return true;
	return false;

/*	Ogre::Real distance = TopScroller::instance()->getCameraController()->getPosition().z - enemyObject->node->getPosition().z;
	if (Ogre::Math::Abs(distance) <= 200)
		return true;
	return false;
	*/
	}
bool Enemy::shoot(const Ogre::FrameEvent& fe, Ogre::Vector3 bulletDirection = Ogre::Vector3(0,0,-1))
{
	bulletTimer += fe.timeSinceLastFrame;
	if (bulletTimer <= bulletTime_)
		return false;
	bulletTimer = 0;
	bulletCount++;
	Ogre::Vector3 nodePos = enemyObject->node->_getDerivedPosition();

	bulletContainer.push_back(std::make_pair(createNode(enemyObject->node->getName() + '_' + std::to_string(bulletCount), nodePos - Ogre::Vector3(0,0,20),
		90, Ogre::Vector3(1., 1., 1.) / 50.0, "sphere.mesh", TopScroller::instance()->getMSceneManager()->getRootSceneNode(), 1), bulletDirection));
	return true;
}
void Enemy::updateBullets(const Ogre::FrameEvent& fe)
{

	for (std::vector<std::pair<CollisionObject*, Ogre::Vector3>>::iterator it = bulletContainer.begin(); it != bulletContainer.end();)
	{
		Ogre::Vector3 bulletz = (*it).first->node->getPosition();
		Ogre::Vector3 bulletStart = (*it).first->node->getInitialPosition();

		if (bulletStart.distance(bulletz) > 300)
		{
			TopScroller::instance()->getMSceneManager()->destroySceneNode((*it).first->node->getName());
			it = bulletContainer.erase(it);
			continue;
		}
		else
		{

			if (JetController::instance()->takeDamage((*it).first->node, bulletDamage))
			{
				TopScroller::instance()->getMSceneManager()->destroySceneNode((*it).first->node->getName());
				it = bulletContainer.erase(it);
				continue;
			}
		}

		(*it).first->node->translate((*it).second * bulletSpeed_ * fe.timeSinceLastFrame, Ogre::Node::TransformSpace::TS_WORLD);
		++it;
	}
}
bool Enemy::takeDamage(int damage)
{
	if (invinsible)
		return false;
	health_ -= damage;
	if (health_ <= 0)
	{
		isDead_ = true;
		static int particleNumber = 0;

		// attach the particle system to a scene node
		Ogre::SceneNode* deathPosition = TopScroller::instance()->getMSceneManager()->getRootSceneNode()->createChildSceneNode(enemyObject->node->getName() + "_particle");
		deathPosition->setPosition(enemyObject->node->_getDerivedPosition());
		deathPosition->setScale(.5, .5, .5);
		Ogre::ParticleSystem* deathParticleSystem = TopScroller::instance()->getMSceneManager()->createParticleSystem("particle" + std::to_string(++particleNumber), "Examples/GreenyNimbus");
		deathPosition->attachObject(deathParticleSystem);
		TopScroller::instance()->addParticleNode(std::make_pair(deathPosition, 0));
		TopScroller::instance()->getMSceneManager()->destroySceneNode(enemyObject->node->getName());
		TopScroller::instance()->addHighscore(15);
		enemyObject->node = nullptr;
		return true;
	}
	return false;
}
void Enemy::move(const Ogre::FrameEvent& fe)
{
	if (aiMove != nullptr) aiMove->move(enemyObject->node, fe);
	if (enemyObject->node != nullptr && canMove)
		enemyObject->node->translate(Ogre::Vector3(0, 0, 15 * fe.timeSinceLastFrame), Ogre::Node::TS_WORLD);
}
Enemy::~Enemy()
{ 
	delete enemyObject;
	delete aiMove; 

}

OgreHeadBoss::OgreHeadBoss(int health, std::string name, Ogre::Vector3 positionStart) : Enemy(health, positionStart)
{
	createEnemy(name);
	createBossHealthUI();
	spawnJets();
	attackRound();
	setBulletTime(.8);
}
bool OgreHeadBoss::takeDamage(int damage)
{
	bool b = Enemy::takeDamage(damage);
	if (b)TopScroller::instance()->addHighscore(200);
	bossHealth->setText("OGRE BOSS HEALTH = " + std::to_string(health_));
	return b;
}
void OgreHeadBoss::createBossHealthUI()
{
	bossHealth = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Label", "bossHealthWindow");
	bossHealth->setPosition(CEGUI::UVector2(CEGUI::UDim(.25, 0), CEGUI::UDim(.15, 0)));
	bossHealth->setSize(CEGUI::USize(CEGUI::UDim(.5f, 0), CEGUI::UDim(.05f, 0)));
	bossHealth->setText("OGRE BOSS HEALTH = " + std::to_string(health_));
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(bossHealth);
	
}
void OgreHeadBoss::update(const Ogre::FrameEvent& fe) 
{
	if (isDead_)
		return;
	static Ogre::Real startTimer = 0;
	if (startTimer < startTime)
	{
		setInvinsible(true);
		startTimer += fe.timeSinceLastFrame;
		enemyObject->node->translate(Ogre::Vector3(0, 0, -1) * (300 / startTime) * fe.timeSinceLastFrame, Ogre::Node::TS_WORLD);
	}
	else
	{
		setInvinsible(false);
		//random attack check then call that attack with timers to know what stage of attack it is.
		//then attack
		if (doingAttack)
		{
			if (attackNumber == 1)
				attacks[0](fe);
			else
				attacks[1](fe);
		}
		else
		{
			attackNumber = rand() % 4;
			std::cout << "attackNumber = " << std::to_string(attackNumber) << std::endl;
			doingAttack = true;
		}
	}
	updateBullets(fe);
}
void OgreHeadBoss::attackRound()
{
	attacks.push_back([&](const Ogre::FrameEvent& fe)
	{
		static float timer = 0;
		int multiplier = 10;
		timer += fe.timeSinceLastFrame;
		setBulletTime(.3);
		setBulletSpeed(100);
		static int shotSide = 1;
		if (timer <= 2)
		{
			enemyObject->node->translate(Ogre::Vector3(-4, 0, -4) * multiplier* fe.timeSinceLastFrame, Ogre::Node::TS_WORLD);
			shoot(fe,Ogre::Vector3(1,0,-1));
		}
		else if (timer <= 4)
		{
			enemyObject->node->translate(Ogre::Vector3(1.25, 0, -4.25) * multiplier *fe.timeSinceLastFrame, Ogre::Node::TS_WORLD);
			shoot(fe, Ogre::Vector3(1, 0, -1));
		}
		else if (timer <= 6)
		{
			enemyObject->node->translate(Ogre::Vector3(5, 0, 0) * multiplier *fe.timeSinceLastFrame, Ogre::Node::TS_WORLD);
			if (shoot(fe, Ogre::Vector3(1 * shotSide, 0, -1)))
				shotSide *= -1;
		}
		else if (timer <= 8)
		{
			enemyObject->node->translate(Ogre::Vector3(1.25, 0, 4.25) *multiplier * fe.timeSinceLastFrame, Ogre::Node::TS_WORLD);
			shoot(fe, Ogre::Vector3(-1, 0, -1));
		}
		else if (timer <= 10)
		{
			enemyObject->node->translate(Ogre::Vector3(-4, 0, 4) *multiplier * fe.timeSinceLastFrame, Ogre::Node::TS_WORLD);
			shoot(fe, Ogre::Vector3(1, 0, -1));
		}
		else if (timer > 10)
		{
			enemyObject->node->setPosition(0, enemyObject->node->getPosition().y, enemyObject->node->getPosition().z);
			timer = 0;
			doingAttack = false;
		}
	});
}
void OgreHeadBoss::spawnJets()
{
	attacks.push_back([&](const Ogre::FrameEvent& fe)
	{
	setInvinsible(true);
	static bool finshSpawn = false;
	static int jetNumber = 0;
	Ogre::Real jetSpawnTime = .5;
	static Ogre::Real jetSpawnTimer = 1;

	if (!finshSpawn)
	{
		jetSpawnTimer += fe.timeSinceLastFrame;
		if (jetSpawnTimer >= jetSpawnTime)
		{
			jetSpawnTimer = 0;
			switch (jetNumber)
			{
			case 0:

				spawnedJets.push_back(new Fighter(40, "bossJet" + std::to_string(jetNumber),
					enemyObject->node->getPosition() + Ogre::Vector3(-215, 0, 0) , 150
					));
				EnemyContainer::instance()->addEnemy(spawnedJets[spawnedJets.size() - 1]);
				break;
			case 1:
				spawnedJets.push_back(new Fighter(40, "bossJet" + std::to_string(jetNumber),
					enemyObject->node->getPosition() + Ogre::Vector3(-154, 0, 0), 150
					));
				EnemyContainer::instance()->addEnemy(spawnedJets[spawnedJets.size() - 1]);
				break;
			case 2:
				spawnedJets.push_back(new Fighter(40, "bossJet" + std::to_string(jetNumber),
					enemyObject->node->getPosition() + Ogre::Vector3(-93, 0, 0), 150
					));
				EnemyContainer::instance()->addEnemy(spawnedJets[spawnedJets.size() - 1]);
				break;
			case 3:
				spawnedJets.push_back(new Fighter(40, "bossJet" + std::to_string(jetNumber),
					enemyObject->node->getPosition() + Ogre::Vector3(215, 0, 0), 150
					));
				EnemyContainer::instance()->addEnemy(spawnedJets[spawnedJets.size() - 1]);
				break;
			case 4:
				spawnedJets.push_back(new Fighter(40, "bossJet" + std::to_string(jetNumber),
					enemyObject->node->getPosition() + Ogre::Vector3(154, 0, 0), 150
					));
				EnemyContainer::instance()->addEnemy(spawnedJets[spawnedJets.size() - 1]);
				break;
			case 5:
				spawnedJets.push_back(new Fighter(40, "bossJet" + std::to_string(jetNumber),
					enemyObject->node->getPosition() + Ogre::Vector3(93, 0, 0), 150
					));
				EnemyContainer::instance()->addEnemy(spawnedJets[spawnedJets.size() - 1]);
				break;
			case 6:
				finshSpawn = true;
			default:
				break;
			}
			if (!finshSpawn)
			{
				spawnedJets[jetNumber]->setCanMove(false);
				++jetNumber;
			}
		}
	}
	else
	{
		if (spawnedJets.size() <= 0)
		{
			setInvinsible(false);
			doingAttack = false;
			finshSpawn = false;
			jetSpawnTimer = 1;
			jetNumber = 0;
		}
		else
		{
			for (std::vector<Enemy*>::iterator it = spawnedJets.begin(); it != spawnedJets.end();)
			{
				if ((*it)->isDead() && (*it)->bulletContainerEmpty())
					it = spawnedJets.erase(it);
				else
				{
					(*it)->update(fe);
					it++;
				}
			}
		}
	}
	});
}

void Fighter::update(const Ogre::FrameEvent& fe)
{
	if (!isDead())
	{
		if (canStartUpdate()) return;
		if (enemyObject->node->getPosition().z >= TopScroller::instance()->getCameraController()->getPosition().z - 200)
		{
			move(fe);
			shoot(fe);
		}
	}
	updateBullets(fe);
}