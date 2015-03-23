#ifndef __GHMOVE_H__
#define __GHMOVE_H__

#include "cocos2d.h"
#include <iostream>
class GHMove : public cocos2d::Sprite
{
public:
	static GHMove* create(int posX,bool pointto);
	virtual bool init();
	bool initWithParamter(int posX,bool pointto);

	void setPointTo(bool pointto);
private:
	bool m_pointto;	//�ƶ����� false=�� true=��
	int m_posX;		//λ��
};

#endif