#ifndef _ENEMY_h_
#define _ENEMY_h_

#include "TopScroller.h"

class Enemy
{
public:
	Enemy(int health, Ogre::Vector3 startPosition,Ogre::Real bulletSpeed = 50, Ogre::Real bulletTime = 2, AIMove* movingFunc = nullptr) 
		:health_(health), _startPosition(startPosition), bulletSpeed_(bulletSpeed), bulletTime_(bulletTime), aiMove(movingFunc){};
	virtual ~Enemy();
	virtual bool takeDamage(int damage);
	virtual void createEnemy(std::string name) = 0;
	virtual void update(const Ogre::FrameEvent& fe) = 0;

	void move			(const Ogre::FrameEvent& fe);
	void updateBullets	(const Ogre::FrameEvent& fe);
	void setEnemyObject	(CollisionObject* e)	{ enemyObject = e; }
	void setCanMove		(bool can)				{ canMove = can; }
	void setInvinsible	(bool inv)				{ invinsible = inv; };
	void setBulletTime	(Ogre::Real bulletTime)	{ bulletTime_ = bulletTime; }
	void setBulletSpeed	(Ogre::Real bulletSpeed){ bulletSpeed_ = bulletSpeed; }

	bool isDead()				const	{ return isDead_; }
	bool bulletContainerEmpty() const	{ if (bulletContainer.size() <= 0) return true; return false; }
	
	bool canStartUpdate();
	bool shoot(const Ogre::FrameEvent&, Ogre::Vector3 bulletDirection);

	Ogre::Vector3 getStartPosition() const		{ return _startPosition; }
	CollisionObject* getCollisionObject() const	{ return enemyObject; }
protected:
	bool isDead_ = false;
	bool invinsible = false;
	bool canMove = true;
	int health_;
	CollisionObject* enemyObject;
	Ogre::Vector3 _startPosition;
private:
	AIMove* aiMove;
	bool start = false;

	int bulletCount = 0;
	Ogre::Real bulletTimer = 100;
	Ogre::Real bulletTime_ = 2;
	Ogre::Real bulletSpeed_ = 50;
	Ogre::Real bulletDamage = 10;

	std::vector<std::pair<CollisionObject*, Ogre::Vector3>> bulletContainer;
};


class OgreHeadBoss :public Enemy
{
public:
	OgreHeadBoss(int health, std::string name, Ogre::Vector3 positionStart);
	bool takeDamage(int damage) override;
	void createEnemy(std::string name) override
	{
		setEnemyObject(createNode(name, getStartPosition(), 180, 
			Ogre::Vector3(1.7,1.7,1.7), "ogrehead.mesh", TopScroller::instance()->getMSceneManager()->getRootSceneNode(), 3));
	}
	void update(const Ogre::FrameEvent& fe) override;
	void createBossHealthUI();
	bool hit(Ogre::SceneNode* node, int damage)
	{
		if (isDead_)
			return false;
		if (checkCollisionBetween(node, enemyObject->node))
		{
			if (takeDamage(damage))
			{
				isDead_ = true;

			}
		}

		for (std::vector<Enemy*>::iterator it = spawnedJets.begin(); it != spawnedJets.end();)
		{
			if (checkCollisionBetween((*it)->getCollisionObject()->node, node))
			{
				(*it)->takeDamage(damage);

				return true;
				
			}
			it++;
		}
		return false;
	}
protected:
	void attackRound();
	void spawnJets();
private:
	CEGUI::Window * bossHealth;
	Ogre::Real startTime = 2;
	bool doingAttack = false;
	int attackNumber;
	
	std::vector< std::function<void(const Ogre::FrameEvent& fe)> > attacks;
	std::vector<Enemy*> spawnedJets;
};

class Fighter :public Enemy
{
public:
	Fighter(int health, std::string name, Ogre::Vector3 positionStart, Ogre::Real bulletSpeed = 50,Ogre::Real bulletTime = 2, AIMove* movingFunc = nullptr) :Enemy(health, positionStart,bulletSpeed,bulletTime, movingFunc){ createEnemy(name); }
	void createEnemy(std::string name) override
	{
		setEnemyObject(createNode(name, getStartPosition(), 180, 
			Ogre::Vector3(2, 2, 2), "RZR-002.mesh", TopScroller::instance()->getMSceneManager()->getRootSceneNode(), 3));
	}
	void update(const Ogre::FrameEvent& fe) override;
};



#endif