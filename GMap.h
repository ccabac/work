#ifndef __GMAP_H__
#define __GMAP_H__

#include "cocos2d.h"
#include "GPost.h"
#include <vector>

struct GHMove
{
	bool pointto;
	int posX;
};


class GMap : public cocos2d::LayerColor
{
public:
#define GMAP_DATALENGTH_MIN	10
#define GMAP_DATALENGTH_MAX	200

#define GMAP_MAPWIDTH	112
#define GMAP_MAPHEIGHT	100
public:
	virtual bool init();
	void setMapData(int len,int level[],bool trap[],bool flytrap[],bool hmove[],bool drop[]);
	//driver
	int driverHMove(float speed);						//驱动所有的滑块 返回值=滑块数量
	//getmember
	int getLength();
	int getLevel(int posX);
	bool isTrap(int posX);
	bool isFlyTrap(int posX);
	bool isHmove(int posX);
	bool isDrop(int posX);
	//Animations
	void animationDropDown(int posX,float speed);
	void animationMove(int num,float speed);
	CREATE_FUNC(GMap);
private:
	//地图长度
	int m_maplength;
	//柱子数据
	cocos2d::Vector<GPost*> m_vector_posts;
	
	//储存横条精灵
	cocos2d::Vector<cocos2d::Sprite *> m_vector_hmove;
	/*
	//储存所有柱子精灵
	std::vector<cocos2d::Vector<cocos2d::Sprite *>> m_mapspritepost;
	//*/

};




#endif