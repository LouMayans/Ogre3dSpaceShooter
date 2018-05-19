#ifndef _COLLISIONOBJECT_
#define _COLLISIONOBJECT_

#include "TopScroller.h"

struct CollisionObject{
	int id;		// 0 = jet , 1 = bullets, 3 = enemies 4 = boss
	Ogre::SceneNode* node;
	Ogre::Entity* entity;
	CollisionObject(){}
	CollisionObject(Ogre::SceneNode* n, Ogre::Entity* e, int i) :node(n), entity(e), id(i){}
};
#endif