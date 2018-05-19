#ifndef _JETCONTROLLER_h_
#define _JETCONTROLLER_h_

#include "TopScroller.h"

class JetController
{
public:
	~JetController();
	static JetController* instance();

	void Start();
	void Update(const Ogre::FrameEvent& fe);
	
	bool takeDamage(Ogre::SceneNode*, int damage);

	void jetInfoWindowShow(bool show){ if (show) jetInfo->show(); else jetInfo->hide(); };
	CollisionObject* getJetObject() const { return jetObject; }
	
private:
	JetController(void);
	static JetController* instance_;
	JetController(const JetController&){}
	JetController& operator=(const JetController&){}

	void JetInfoWindow();
	void createJetThrust();
	void input(const Ogre::FrameEvent& fe);
	void shoot(const Ogre::FrameEvent& fe);
	void updateBullets(const Ogre::FrameEvent& fe);
	void CreateJet(Ogre::Degree degree, Ogre::Vector3 scale, std::string name);
	void death();
	
	Ogre::Vector3 boundaries = Ogre::Vector3(213, 0, 190);
	int bulletCount = 0;

	Ogre::Real moveSpeed = 250;
	Ogre::Real timeBetweenShots = .15f;
	Ogre::Real shotTimer = 0;
	Ogre::Real bulletSpeed = 350;
	Ogre::Real bulletDamage = 10;
	int jetHealth = 200;
	CollisionObject* jetObject;
	std::vector<CollisionObject*> bulletObjects;
	
	TopScroller* TS;
	CEGUI::Window* jetInfo = nullptr;
};


#endif