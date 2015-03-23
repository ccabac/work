#ifndef __MainScene_SCENE_H__
#define __MainScene_SCENE_H__

#include "cocos2d.h"
#include "GMap.h"
class MainScene : public cocos2d::LayerColor
{
public:
#define MAINSCENE_BODY_MOREHEIGHT	20
	enum DIECODE
	{
		NODIE = 0,
		NONE,
		TRAP,
		FLYTRAP,
		DROPDOWN,
		UPDIE,
		MAPEND
	};
	enum JUMPTYPE{
		JNONE = 0,
		JFAR,
		JUP,
		JDOWN,
		JTO
	};
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override; 
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override; 
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override; 

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void CreateMapScene(float newspeed);
    void Start();
	void StopStep();
	void Stop();
	DIECODE CheckDie();
	void Die(DIECODE diecode);
	void updateCustom(float dt);
	//Animations
	void bodyJump(float speed);
	void bodyJumpMore(float speed,int morePosY);
	//Animations
	void animationBodyDrop(float speed);
	cocos2d::Vec2 bodyGetInitPosition();
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);

private:
	GMap * m_gmap;
	cocos2d::Sprite * m_body;
	int m_posX,m_posY;
	bool m_jump;
	float m_speed;
	bool m_status;
	bool m_autojump;
	bool m_flytrap;
};

#endif // __MainScene_SCENE_H__
