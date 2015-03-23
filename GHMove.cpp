#include "GHMove.h"
USING_NS_CC;
bool GHMove::init()
{
	if(!Sprite::init())
		return false;
	setPointTo(false);		//Ä¬ÈÏ×ó±ß
	m_posX = 0;
	return true;
}
bool GHMove::initWithParamter(int posX,bool pointto)
{
	if(!this->init())
		return false;
	setPointTo(pointto);
	m_posX = 0;
	return true;
}
void GHMove::setPointTo(bool pointto)
{
	this->m_pointto = pointto;
}
GHMove* GHMove::create(int posX,bool pointto)
{
	GHMove *obj = new GHMove();
	if(obj && obj->initWithFile("hmove.png"))
	{
		obj->initWithParamter(posX,pointto);
		obj->autorelease();
		return obj;
	}
	CC_SAFE_DELETE(obj);
    return nullptr;
}
