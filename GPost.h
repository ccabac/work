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
	int m_level;			//���Ӹ߶�[0,5]
	bool m_trap;			//�Ƿ�������
	bool m_flytrap;			//�����Ƿ�����
	bool m_hmove;			//�Ƿ��к��� ֻ�ڳ�ʼ��ʱʹ��
	bool m_drop;			//�Ƿ����׹

	//���Ӿ���
	cocos2d::Vector<cocos2d::Sprite*> m_vector_posts;
	//���徫��
	cocos2d::Sprite* m_sp_trap;
};



#endif