#ifndef AIMOVE_H
#define AIMOVE_H

#include "TopScroller.h"

class AIMove
{
public:
	virtual void move(Ogre::SceneNode*, const Ogre::FrameEvent& fe) = 0;
	virtual ~AIMove(){};
};

class zigzagMove : public AIMove
{
public:
	zigzagMove(Ogre::Real distance, Ogre::Real time, Ogre::Vector3 directionVector, int zigTimes = 100, Ogre::Real waitTime = 0) :zigzagDistance(distance), zigzagTime(time), waitTime_(waitTime), directionVector_(directionVector), zigTimes_(zigTimes){}
	void move(Ogre::SceneNode* movingNode, const Ogre::FrameEvent& fe)
	{
		waitTimer += fe.timeSinceLastFrame;
		if (waitTimer <= waitTime_)
			return;

		if (zigzagTimer >= zigzagTime)
		{
			zigTimes_ -= 1;
			if (zigTimes_ <= 0)
				return;
			zigzagTimer = 0;
			direction *= -1;
		}
		zigzagTimer += fe.timeSinceLastFrame;
		movingNode->translate(directionVector_ * direction * zigzagDistance / zigzagTime * fe.timeSinceLastFrame, Ogre::Node::TransformSpace::TS_WORLD);
	}
	~zigzagMove(){}
private:
	int direction = 1;
	int zigTimes_;
	Ogre::Vector3 directionVector_;
	Ogre::Real waitTime_;
	Ogre::Real waitTimer = 0;
	Ogre::Real zigzagDistance;
	Ogre::Real zigzagTime;
	Ogre::Real zigzagTimer = 0;
};


#endif