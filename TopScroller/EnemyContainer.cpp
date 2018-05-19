#include "EnemyContainer.h"


EnemyContainer::EnemyContainer(void){}
EnemyContainer* EnemyContainer::instance_ = nullptr;
EnemyContainer* EnemyContainer::instance(){
	if (instance_ == nullptr)
		instance_ = new EnemyContainer;
	return instance_;
}
void EnemyContainer::start()
{
	int fighterCount = 0;
	for (int i = 1; i <= 2; ++i)
	{
		Ogre::Real playPlane = TopScroller::instance()->getPlayPlane();
		enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-50, playPlane, 550 + 4200 * (i - 1)), 150, 1.1));
		enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(0, playPlane, 500 + 4200 * (i - 1)), 150, 1.1));
		enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(50, playPlane, 550 + 4200 * (i - 1)), 150, 1.1));


		enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(225, playPlane, 950 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(150, 2, Ogre::Vector3(-1, 0, 0), 1)));
		enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(175, playPlane, 900 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(150, 2, Ogre::Vector3(-1, 0, 0), 1)));
		enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(125, playPlane, 950 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(150, 2, Ogre::Vector3(-1, 0, 0), 1)));

		enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-225, playPlane, 1250 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(150, 2, Ogre::Vector3(1, 0, 0), 1)));
		enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-175, playPlane, 1200 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(150, 2, Ogre::Vector3(1, 0, 0), 1)));
		enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-125, playPlane, 1250 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(150, 2, Ogre::Vector3(1, 0, 0), 1)));


		for (int j = 0; j < 2; ++j)
		{
			int side = 1;
			if (j == 1) side = -1;
			enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(225 * side, playPlane, (1450 + 550 * j) + 4200 * (i - 1)), 150, 1.1, new zigzagMove(85, 2, Ogre::Vector3(-1 * side, 0, 0), 1)));

			enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(200 * side, playPlane, (1550 + 550 * j) + 4200 * (i - 1)), 150, 1.1, new zigzagMove(110, 2, Ogre::Vector3(-1 * side, 0, 0), 1)));
			enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(100 * side, playPlane, (1550 + 550 * j) + 4200 * (i - 1)), 150, 1.1, new zigzagMove(110, 2, Ogre::Vector3(-1 * side, 0, 0), 1)));

			enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(100 * side, playPlane, (1750 + 550 * j) + 4200 * (i - 1)), 150, 1.1, new zigzagMove(110, 2, Ogre::Vector3(-1 * side, 0, 0), 1)));
			enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(0, playPlane, (1750 + 550 * j) + 4200 * (i - 1)), 150, 1.1, new zigzagMove(110, 2, Ogre::Vector3(-1 * side, 0, 0), 1)));
		}
		//last ended at 2300

		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(225, playPlane, 2600 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(125, 1, Ogre::Vector3(-1, 0, 0), 1, 1.25)));
		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-225, playPlane, 2600 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(125, 1, Ogre::Vector3(1, 0, 0), 1, 1.25)));

		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(100, playPlane, 2800 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(75, 1, Ogre::Vector3(1, 0, 1), 1, 1.25)));
		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-100, playPlane, 2800 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(75, 1, Ogre::Vector3(-1, 0, 1), 1, 1.25)));

		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(225, playPlane, 3150 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(125, 1, Ogre::Vector3(-1, 0, 0), 1, 1.25)));
		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-225, playPlane, 3150 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(125, 1, Ogre::Vector3(1, 0, 0), 1, 1.25)));

		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(200, playPlane, 3475 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(90, 1.6, Ogre::Vector3(-1, 0, 0), 3, .15)));
		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(100, playPlane, 3475 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(90, 1.6, Ogre::Vector3(-1, 0, 0), 3, .15)));

		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-200, playPlane, 3775 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(90, 1.6, Ogre::Vector3(1, 0, 0), 3, .15)));
		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-100, playPlane, 3775 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(90, 1.6, Ogre::Vector3(1, 0, 0), 3, .15)));

		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(100, playPlane, 4050 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(75, 1, Ogre::Vector3(1, 0, 1), 1, 1.65)));
		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-100, playPlane, 4050 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(75, 1, Ogre::Vector3(-1, 0, 1), 1, 1.65)));

		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(80, playPlane, 4125 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(75, 1, Ogre::Vector3(.5, 0, 1), 1, 1.65)));
		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-80, playPlane, 4125 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(75, 1, Ogre::Vector3(-.5, 0, 1), 1, 1.65)));

		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(60, playPlane, 4200 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(75, 1, Ogre::Vector3(0, 0, 1), 1, 1.65)));
		enemyContainer.push_back(new Fighter(20, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-60, playPlane, 4200 + 4200 * (i - 1)), 150, 1.1, new zigzagMove(75, 1, Ogre::Vector3(0, 0, 1), 1, 1.65)));

	}
	/*for (int i = 1; i < 7; ++i)
	{
	enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-50, TopScroller::instance()->getPlayPlane(), i * 4 * 500 + 0), 500 * 4 + 4200 * (i-1) - 200, new zigzagMove(50, 2, Ogre::Vector3(-1, 0, 0))));
	enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-30 - 50, TopScroller::instance()->getPlayPlane(), i * 4 * 500 + 50), 500 * 4 + 4200 * (i-1) - 200, new zigzagMove(50, 2, Ogre::Vector3(-1, 0, 0))));
	enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(30 - 50, TopScroller::instance()->getPlayPlane(), i * 4 * 500 + 50), 500 * 4 + 4200 * (i-1) - 200, new zigzagMove(50, 2, Ogre::Vector3(-1, 0, 0))));

	enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(50, TopScroller::instance()->getPlayPlane(), i * 4 * 500 + 0), 500 * 4 + 4200 * (i-1) - 200, new zigzagMove(50, 2, Ogre::Vector3(1, 0, 0))));
	enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-30 + 50, TopScroller::instance()->getPlayPlane(), i * 4 * 500 + 50), 500 * 4 + 4200 * (i-1) - 200, new zigzagMove(50, 2, Ogre::Vector3(1, 0, 0))));
	enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(30 + 50, TopScroller::instance()->getPlayPlane(), i * 4 * 500 + 50), 500 * 4 + 4200 * (i-1) - 200, new zigzagMove(50, 2, Ogre::Vector3(1, 0, 0))));
	}

	for (int i = 1; i < 11; ++i)
	{
	Enemy* one = new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-170, TopScroller::instance()->getPlayPlane(), i * 550 + 120), 550 + 4200 * (i-1) - 200 + 120);
	Enemy* two = new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(170, TopScroller::instance()->getPlayPlane(), i * 550 + 120), 550 + 4200 * (i-1) - 200 + 120);
	one->setBulletTime(.6);
	two->setBulletTime(.6);
	enemyContainer.push_back(one);
	enemyContainer.push_back(two);
	}
	for (int i = 0; i < 7; ++i)
	{
	enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(-200, TopScroller::instance()->getPlayPlane(), 800 + 4 + 4200 * (i-1) * 800), 800 + 4 + 4200 * (i-1) * 800 - 200, new zigzagMove(400, 3, Ogre::Vector3(1, 0, 0), 2)));
	enemyContainer.push_back(new Fighter(30, "fighter" + std::to_string(++fighterCount), Ogre::Vector3(200, TopScroller::instance()->getPlayPlane(), 870 + 4 + 4200 * (i-1) * 800), 870 + 4 + 4200 * (i-1) * 800 - 200, new zigzagMove(400, 3, Ogre::Vector3(-1, 0, 0), 2)));
	}*/

}
void EnemyContainer::update(const Ogre::FrameEvent& fe)
{
	for (std::vector<Enemy*>::iterator it = enemyContainer.begin(); it != enemyContainer.end();)
	{
		if ((*it)->bulletContainerEmpty() && (*it)->isDead())
			it = enemyContainer.erase(it);
		else
		{
			(*it)->update(fe);
			it++;
		}

	}
}
bool EnemyContainer::enemyHit(Ogre::SceneNode* projectile, int damage)
{
	for (std::vector<Enemy*>::iterator it = enemyContainer.begin(); it != enemyContainer.end();)
	{
		if ((*it)->isDead())
		{
			++it;
			continue;
		}

		if (checkCollisionBetween(projectile, (*it)->getCollisionObject()->node))
		{
			if ((*it)->takeDamage(damage))
			{
				if ((*it)->bulletContainerEmpty())
					it = enemyContainer.erase(it);
			}
			return true;
		}
		else
			++it;
	}
	return false;
}
void EnemyContainer::removeEnemy(Enemy* e)
{
	for (std::vector<Enemy*>::iterator it = enemyContainer.begin(); it != enemyContainer.end(); ++it)
	{
		if ((*it) == e)
		{
			if (e->getCollisionObject()->node != nullptr)
				TopScroller::instance()->getMSceneManager()->destroySceneNode(e->getCollisionObject()->node->getName());
			it = enemyContainer.erase(it);
			return;
		}
	}
}