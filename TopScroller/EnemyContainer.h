#ifndef ENEMYCONTAINER_H
#define ENEMYCONTAINER_H

#include "Enemy.h"

class EnemyContainer
{
public:
	static EnemyContainer* instance();
	virtual ~EnemyContainer(){ for (auto e : enemyContainer) delete e; };
	void start();
	void update(const Ogre::FrameEvent& fe);
	void addEnemy(Enemy* e){ enemyContainer.push_back((e)); }
	void removeEnemy(Enemy* e);
	bool enemyHit(Ogre::SceneNode* projectile, int damage);
private:
	//Singleton
	EnemyContainer(void);
	EnemyContainer(const EnemyContainer&){}
	EnemyContainer& operator=(const EnemyContainer&){}
	static EnemyContainer* instance_;
	std::vector<Enemy*> enemyContainer;
};

#endif