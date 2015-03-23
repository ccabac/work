#include "GPost.h"

USING_NS_CC;


GPost* GPost::create(int level,bool trap,bool flytrap,bool hmove,bool drop)
{
	GPost *obj = nullptr;
	if(0<=level && level<=5)
	{
		obj = new GPost();
		if(obj && obj->initWithParameter(level,trap,flytrap,hmove,drop))
		{
			obj->autorelease();
		}else{
			delete obj;
			obj = nullptr;
		}
	}
	return obj;

}
bool GPost::init()
{
	if(!LayerColor::initWithColor(Color4B::WHITE))
		return false;
	return true;
}
bool GPost::initWithParameter(int level,bool trap,bool flytrap,bool hmove,bool drop)
{
	using namespace std;
	if(!(0<=level && level<=5))
		return false;

	if(!this->init())
		return false;
	/*
#pragma region œ›⁄Â°¢∫·Ãı≤‚ ‘
	if(rand()%100<=25)
	{
		trap = true;
		
		#pragma region œ›⁄Â–¸ø’≤‚ ‘
		if(rand()%100<=25)
			flytrap = true;
		m_flytrap = flytrap;
#pragma endregion 
		
	}
	if(rand()%100<=25)
		hmove = true;
#pragma endregion
		//*/
	this->m_level = level;
	this->m_trap = trap;
	this->m_flytrap = flytrap;
	this->m_hmove = hmove;
	this->m_drop = drop;
	//this->m_hmove_pointto = false;	//ƒ¨»œÕ˘◊Û±ﬂ“∆∂Ø
	auto funcAddSpritePost = [&](Sprite* sp)
	{
		m_vector_posts.pushBack(sp);
	};
#pragma region …˙≥…÷˘◊”æ´¡È
	if(level == 0){
		string spname = "none.png";
		Sprite *sp = Sprite::create("none.png");
		sp->setPosition(0,0);
		this->addChild(sp);
		funcAddSpritePost(sp);
	}else if(level == 1){
		Sprite *sp = Sprite::create("one.png");
		sp->setPosition(0,0);
		this->addChild(sp);
		funcAddSpritePost(sp);
	}else if(level >=2){
		Sprite *spdown = Sprite::create("more_down.png");
		spdown->setPosition(0,0);
		this->addChild(spdown);
		funcAddSpritePost(spdown);
		int numcell = level - 2;
		for(int i = 0; i < numcell;i++)
		{
			Sprite *spcell = Sprite::create("more_center.png");
			spcell->setPosition(0,(i+1)*GPOST_RESOURCE_POSTHEIGHT);
			this->addChild(spcell);
			funcAddSpritePost(spcell);
		}
		Sprite *sptop = Sprite::create("more_top.png");
		sptop->setPosition(0,(numcell+1)*GPOST_RESOURCE_POSTHEIGHT);
		this->addChild(sptop);
		funcAddSpritePost(sptop);
	}
#pragma endregion

#pragma region …˙≥…œ›⁄Â
	if(trap)
	{
		m_sp_trap = Sprite::create("trap.png");	
		this->addChild(m_sp_trap);
		m_sp_trap->setPosition(0,level * GPOST_RESOURCE_POSTHEIGHT);
		if(flytrap)
			m_sp_trap->setPosition(0,(level+2) * GPOST_RESOURCE_POSTHEIGHT);
		//m_sp_trap->setZOrder(1);


	}

#pragma endregion

	return true;

}
#pragma region getmember
int GPost::getLevel()
{
	return m_level;
}
bool GPost::isTrap()
{
	return m_trap;
}
bool GPost::isFlyTrap()
{
	return m_flytrap;
}
bool GPost::isDrop()
{
	return m_drop;
}
#pragma endregion
void GPost::animationHMove(float speed,Vec2 topos,GPost *to)
{
	if(!to)
		return;
	MoveTo *acmt = MoveTo::create(speed,topos);
	auto funcCallBack = [&](){
		//«–ªª◊¥Ã¨
	};
	CallFunc *accf =CallFunc::create(funcCallBack);
	Sequence *seq = Sequence::create(acmt,accf,NULL);
	this->runAction(seq);
}
void GPost::setHMoveSprite(cocos2d::Sprite *sprite)
{

}
void GPost::animationDropDown(float speed)
{
	if(!m_drop)
		return ;
	if(m_vector_posts.size()<=0)
		return;
	//this->m_level = 0;
	Sprite *sp = m_vector_posts.at(0);
	for(int i = 0; i < m_vector_posts.size(); i++)
	{
		RotateTo *acrt = RotateTo::create(speed*6,rand()%180 - 90);
		MoveTo *acmoveto = MoveTo::create(speed*6,sp->getPosition()+ Vec2(rand()%300-150,-500));
		FadeOut *acfo = FadeOut::create(speed*2);
		Spawn *acspawn = Spawn::create(acrt,acmoveto,acfo,NULL); 
		m_vector_posts.at(i)->runAction(acspawn);
	}
}



