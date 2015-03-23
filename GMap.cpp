#include "GMap.h"
USING_NS_CC;

bool GMap::init()
{

	if ( !LayerColor::initWithColor(ccColor4B::WHITE))
	{
		return false;
	}
	//_mapdata = nullptr;
	//_maplen = -1;

	Size vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	return true;
}
void GMap::setMapData(int len,int levels[],bool traps[],bool flytraps[],bool hmoves[],bool drops[])
{
	if(len<= GMAP_DATALENGTH_MIN)
	{
		log("��error��[GMap::setMapData] mapdata's length is to short ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		return;
	}
	if(len >GMAP_DATALENGTH_MAX)
	{
		log("��warnning��[GMap::setMapData] mapdata's length is to long ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		len = GMAP_DATALENGTH_MAX;
	}
	m_maplength = len;
	for(int i = 0; i < 5;i++)
	{
		levels[i] = 0;
		levels[len - i - 1] = 0;
	}
	for(int i = 0; i < len ;i++)
	{
		int level = levels[i];
		bool trap = traps[i];
		bool flytrap = flytraps[i];
		bool hmove = hmoves[i];
		bool drop = drops[i];
		//����GPost
		GPost *gpost = GPost::create(level,trap,flytrap,hmove,drop);
		this->addChild(gpost);
		Vec2 posPost(i * GPOST_RESOURCE_POSTWIDTH + GMAP_MAPWIDTH,0 + GMAP_MAPHEIGHT);
		gpost->setPosition(posPost);
		m_vector_posts.pushBack(gpost);
		//����HMove
		if(hmove)
		{
			//GHMove *ghmove = GHMove::create(i,false);
			Sprite * sphmove = Sprite::create("hmove.png");
			this->addChild(sphmove);
			sphmove->setZOrder(1);
			GHMove *stc = new GHMove;
			stc->posX = i;
			stc->pointto = false;			//���� false=�� true=��
			sphmove->setTag((int)stc);		
			sphmove->setPosition(Vec2(0,(level+1)*GPOST_RESOURCE_POSTHEIGHT)+posPost);
			m_vector_hmove.pushBack(sphmove);
		}
	}
}
int GMap::getLevel(int posX)
{
	if(posX< 0)
	{
		log("��error��[GMap::setMapData] mapdata's length is to short ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		return -1;
	}
	if(posX >GMAP_DATALENGTH_MAX)
	{
		log("��warnning��[GMap::setMapData] mapdata's length is to long ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		return -1;
	}
	GPost *spost = m_vector_posts.at(posX);
	if(spost)
	{
		return spost->getLevel();
	}
	return -1;
}
bool GMap::isTrap(int posX)
{
	if(posX< 0)
	{
		log("��error��[GMap::setMapData] mapdata's length is to short ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		return false;
	}
	if(posX >GMAP_DATALENGTH_MAX)
	{
		log("��warnning��[GMap::setMapData] mapdata's length is to long ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		return false;
	}
	GPost *spost = m_vector_posts.at(posX);
	if(spost)
	{
		return spost->isTrap();
	}
	return false;
}
bool GMap::isFlyTrap(int posX)
{
	if(posX< 0)
	{
		log("��error��[GMap::setMapData] mapdata's length is to short ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		return false;
	}
	if(posX >GMAP_DATALENGTH_MAX)
	{
		log("��warnning��[GMap::setMapData] mapdata's length is to long ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		return false;
	}
	GPost *spost = m_vector_posts.at(posX);
	if(spost)
	{
		return spost->isFlyTrap();
	}
	return false;
}
bool GMap::isHmove(int posX)
{
	if(posX< 0)
	{
		log("��error��[GMap::setMapData] mapdata's length is to short ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		return false;
	}
	if(posX >GMAP_DATALENGTH_MAX)
	{
		log("��warnning��[GMap::setMapData] mapdata's length is to long ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		return false;
	}
	//����HMove
	for(int i = 0; i < m_vector_hmove.size();i++)
	{
		Sprite* sp = m_vector_hmove.at(i);
		GHMove *stc = (GHMove*)sp->getTag();
		if(stc->posX == posX)
			return true;
	}
	return false;
}
bool GMap::isDrop(int posX)
{
	if(posX< 0)
	{
		log("��error��[GMap::setMapData] mapdata's length is to short ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		return false;
	}
	if(posX >GMAP_DATALENGTH_MAX)
	{
		log("��warnning��[GMap::setMapData] mapdata's length is to long ((%d,%d])!",GMAP_DATALENGTH_MIN,GMAP_DATALENGTH_MAX);
		return false;
	}
	GPost *spost = m_vector_posts.at(posX);
	if(spost)
	{
		return spost->isDrop();
	}
	return false;
}
int GMap::getLength()
{
	return m_maplength;
}
#pragma region Animations
void GMap::animationDropDown(int posX,float speed)
{
	if(!(0 <posX && posX <=m_maplength) || speed<=0)
		return ;
	GPost *gpost = m_vector_posts.at(posX);
	if(gpost)
	{
		gpost->animationDropDown(speed);
	}
}
void GMap::animationMove(int num,float speed)
{
	MoveBy *acmb = MoveBy::create(speed,Vec2(-GPOST_RESOURCE_POSTWIDTH*num,0));
	this->runAction(acmb);
}
int GMap::driverHMove(float speed)
{

	for(int i = 0; i < m_vector_hmove.size();i++)
	{//CC_SPRITE_DEBUG_DRAW
		Sprite *hmove = m_vector_hmove.at(i);
		GHMove *stc = (GHMove*)hmove->getTag();
		GPost *gpost = m_vector_posts.at(stc->posX);
		//Animation
		Vec2 src = hmove->getPosition();
		Vec2 pos(stc->posX * GPOST_RESOURCE_POSTWIDTH + GMAP_MAPWIDTH,src.y);
		//�߽��ж�
		auto funcChangeHMoveStatus = [&](GHMove *stc)
		{
			//�߽��ж� + �������
			int posX = stc->posX;
			bool pointto = stc->pointto;
			GPost *last = NULL;
			GPost *now = m_vector_posts.at(posX);
			GPost *next = NULL;
			if(posX > 0)
				last = m_vector_posts.at(posX-1);
			if(posX < m_maplength - 1)
				next = m_vector_posts.at(posX+1);
			bool state = true;
			//1���Ƿ񵽴���߽� || ������ϰ���  �� ת��Ϊ�ұ�
			if(!last || last->getLevel()> now->getLevel())
			{
				posX++;
				pointto = true;
				state = false;
			}
			//2���Ƿ񵽴��ұ߽� || �ұ����ϰ��� �� ת��Ϊ���
			if(!next || next->getLevel() > now->getLevel())
			{
				posX--;
				pointto = false;
				state = false;
			}
			//3���ճ��ƶ�
			if(state)
			{
				if(pointto)
				{
					posX++;
				}
				else 
				{
					posX--;
				}
			}
			//4��д��
			stc->pointto = pointto;
			stc->posX = posX;
		};
		//���жϱ߽����ƶ�
		funcChangeHMoveStatus(stc);
		//�����޸�
		int t = 0;
		//GPost *obj = m_vector_posts.at(stc->posX);
		
		if(stc->pointto)
		{
			t = 1;
			
		}
		else
		{
			t = -1;
		}
		//�ص����� ����GPost����
		//auto funcPost = [&]()
		//{

		//};
		MoveTo *acmt = MoveTo::create(speed,pos+Vec2(t*GPOST_RESOURCE_POSTWIDTH,0));
		//CallFunc *accf = CallFunc::create(funcPost);
		hmove->runAction(acmt);
		log("�ƶ�����");
	}

	return m_vector_hmove.size();
}
#pragma endregion 