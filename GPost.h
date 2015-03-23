#ifndef __GPOST_H__
#define __GPOST_H__

#define GPOST_RESOURCE_POSTHEIGHT	41
#define GPOST_RESOURCE_POSTWIDTH	56
#include "cocos2d.h"
#include <vector>
class GPost : public cocos2d::LayerColor
{
public:
	virtual bool init();
	bool initWithParameter(int level,bool trap,bool flytrap,bool hmove,bool drop);
	static GPost* create(int level,bool trap,bool flytrap,bool hmove,bool drop);
	
	//getmember
	int getLevel();
	bool isTrap();
	bool isFlyTrap();
	//bool isHmove();
	bool isDrop();
	void changeHMoveStatus();
	void setHMoveSprite(cocos2d::Sprite *sprite);
	//Animations
	void animationDropDown(float speed);
	void animationHMove(float speed,cocos2d::Vec2 topos,GPost *to);

private:
	int m_level;			//柱子高度[0,5]
	bool m_trap;			//是否有陷阱
	bool m_flytrap;			//陷阱是否悬空
	bool m_hmove;			//是否有横条 只在初始化时使用
	bool m_drop;			//是否会下坠

	//柱子精灵
	cocos2d::Vector<cocos2d::Sprite*> m_vector_posts;
	//陷阱精灵
	cocos2d::Sprite* m_sp_trap;
};



#endif