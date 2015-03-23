#include "MainScene.h"
USING_NS_CC;
//#define CC_SPRITE_DEBUG_DRAW 1

Scene* MainScene::createScene()
{
	auto scene = Scene::create();

	auto layer = MainScene::create();

	scene->addChild(layer);

	return scene;
}
bool MainScene::init()
{
	if ( !LayerColor::initWithColor(Color4B::WHITE) )
	{
		return false;
	}
	srand(time(0));
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//��������
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded,this);
	listener->setSwallowTouches(true);
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	//�رհ�ť
	auto closeItem = MenuItemImage::create( "CloseNormal.png","CloseSelected.png",CC_CALLBACK_1(MainScene::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,origin.y + closeItem->getContentSize().height/2));
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//��Ϸ������ʼ
	float InitSpeed = 1.0f;
	this->CreateMapScene(InitSpeed);

	return true;
}
void MainScene::CreateMapScene(float newspeed)
{
	Size vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//���ݹ���
	m_gmap = GMap::create();
#pragma region testdata
	int len = 20;
	int *mapdata = new int[len];
	bool *traps = new bool[len];
	bool *flytraps = new bool[len];
	bool *hmoves = new bool[len];
	bool *drops = new bool[len];
	for(int i = 0; i < len;i++)
	{
		mapdata[i] = 0;
		traps[i] = false;
		flytraps[i] = false;
		hmoves[i] = false;
		drops[i] = false;
	}
#pragma region set mapdata
	int i = 5;
	mapdata[i++] = 1;
	mapdata[i++] = 2;
	mapdata[i++] = 3;
	mapdata[i++] = 2;
	mapdata[i++] = 2;
	mapdata[i++] = 3;
	mapdata[i++] = 2;
	mapdata[i++] = 1;
	mapdata[i++] = 0;
	mapdata[i++] = 0;
	mapdata[i++] = 0;

	traps[3] = true;
	traps[8] = true;
	flytraps[8]=true;
	hmoves[8] = true;
	//flytraps[4] = true;
	//drops[8] = true;
#pragma endregion
#pragma endregion
	m_gmap->setMapData(len,mapdata,traps,flytraps,hmoves,drops);
	this->addChild(m_gmap);

	//����
	m_body = Sprite::create("body.png");
	this->addChild(m_body);
	m_body->setZOrder(2);
	m_body->setPosition(this->bodyGetInitPosition());

	//��λ״̬
	this->m_posX = 0;
	this->m_posY = 0;
	this->m_jump = false;
	this->m_status = false;
	this->m_autojump = false;
	this->m_flytrap = false;
	this->m_speed = newspeed;
}
void MainScene::Start()
{
	//ִ��һ��ʱ��Ƭ
	this->updateCustom(0);
	//������ʱ��
	this->schedule(schedule_selector(MainScene::updateCustom),m_speed + 0.015f,kRepeatForever,0);
}
void MainScene::StopStep()
{
	//��ʱ��ֹͣ
	this->unschedule(schedule_selector(MainScene::updateCustom));
}
Vec2 MainScene::bodyGetInitPosition()
{
	return Vec2(GMAP_MAPWIDTH,GMAP_MAPHEIGHT + MAINSCENE_BODY_MOREHEIGHT);
}
void MainScene::Stop()
{
	//ֹͣ��ʱ��
	this->StopStep();
	//��ͼ����
	if(m_gmap)
	{
		m_gmap->removeAllChildrenWithCleanup(true);
		this->removeChild(m_gmap);
		m_gmap = nullptr;
	}
	//��������
	if(m_body)
	{
		m_body->stopAllActions();
		m_body->removeAllChildrenWithCleanup(true);
		this->removeChild(m_body);
		m_body = nullptr;
	}


	//��������
	this->CreateMapScene(m_speed);
}
void MainScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


void MainScene::updateCustom(float dt)
{
	bool tjump = m_jump;
	bool cflytrap = false;
	//m_jump = false;
	log("update %f",dt);
	if(m_gmap == nullptr)
		return ;
	Size vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//����Ƿ�����
	DIECODE diecode = this->CheckDie();
	if(diecode != DIECODE::NODIE)
	{
		if(diecode != DIECODE::DROPDOWN)
		{
			this->Stop();
		}
		this->Die(diecode);
		return;
	}
	//����DropDown
	if(m_posX <= m_gmap->getLength() - 5)
	{
		if(m_gmap->isDrop(m_posX + 4))
		{
			m_gmap->animationDropDown(m_posX + 4,m_speed);
		}
	}
	//����hmove
	m_gmap->driverHMove(m_speed);
	//return;
	//��������
	/*
	1��ȷ��������
	2��ȷ����Ծ
	NONE+jump = Զ��			//ֻ����������Ǹ���(��ظ߶�=farlevel && farlevel<=nowlevel)  || Զ����ʱ��Ҫ����Ƿ��п�������
	down+jump = Զ��			//���඼����ͨ������ظ߶�=nextlevel��						   ||
	up	+jump = ��ͨ��
	NONE+none = ��ͨ��
	up	+none = ��ͨ��
	down+none = ��ͨ��
	*/
	//1 ȷ��������
	int nowLevel = m_gmap->getLevel(m_posX);
	int nextLevel = m_gmap->getLevel(m_posX+1);
	int farLevel = m_gmap->getLevel(m_posX+2);
	int subLevel = nextLevel - nowLevel;
	int subFarLevel = farLevel - nowLevel;
	#pragma region ȫ�Զ�
	if(m_autojump)
	{
		//ǰ���е������塢��Drop��Զ��
		if(m_gmap->isDrop(m_posX+1) || (m_gmap->isTrap(m_posX+1) && !m_gmap->isFlyTrap(m_posX+1)))
		{
			//��ֹײǽ
			if(subLevel <= 0 )
			{
				if(farLevel<=nowLevel)
					m_posY = farLevel;
				m_posX++;
				m_gmap->animationMove(2,m_speed);
			}

		}else{
			m_posY = nextLevel;
			m_gmap->animationMove(1,m_speed);
		//��ͨ��
		}
		this->bodyJump(m_speed);
		m_posX++;
		return ;
	}
	#pragma endregion
	auto funcCheckFlyTrap = [&]()-> bool{

		if(m_gmap->isTrap(m_posX) && m_gmap->isFlyTrap(m_posX) && tjump)
			return true;
		return false;
	};


	if(tjump){
		//NONE��down+jump  Զ��||Զ����
		if(subLevel <= 0 && farLevel<=nowLevel)
		{
			m_posX++;
			m_posY = farLevel;
			if(!funcCheckFlyTrap())
				m_gmap->animationMove(2,m_speed);
			else{
				cflytrap = true;
				m_gmap->animationMove(1,m_speed);
				m_posX--;
			}
		}else if(subLevel<=0 && farLevel>nowLevel)	//Զ�� && ײ�
		{
			m_posX++;
			if(!funcCheckFlyTrap())
				m_gmap->animationMove(2,m_speed);
			else{
				cflytrap = true;
				m_gmap->animationMove(1,m_speed);
				m_posX--;
			}
		}else if(subLevel ==1)	//up+jump
		{
			m_posY++;
			m_gmap->animationMove(1,m_speed);
			bool d = funcCheckFlyTrap();
		}
	}else{
		//NONE+none = ��ͨ��
		if(subLevel == 0)
		{

		}else if(subLevel == 1){  //up  +none = ��ͨ�� (ײ�)

		}else if(subLevel<0){     //down+none = ��ͨ�� (����)
			m_posY = nextLevel;
		}
		m_gmap->animationMove(1,m_speed);
	}
	if(cflytrap)
	{
		this->bodyJumpMore(m_speed,1);
		m_posY++;
	}else {
		this->bodyJump(m_speed);
	}
	m_jump = false;
	m_posX++;
}
void MainScene::bodyJump(float speed)
{
	this->bodyJumpMore(m_speed,0);
}
void MainScene::bodyJumpMore(float speed,int morePosY = 0)
{
	//������Ծ
	JumpTo *acjt = JumpTo::create(speed,this->bodyGetInitPosition() + Vec2(0,(m_posY+morePosY)*GPOST_RESOURCE_POSTHEIGHT),GPOST_RESOURCE_POSTHEIGHT,1);
	m_body->runAction(acjt);
}
void MainScene::animationBodyDrop(float speed)
{
	if(m_body != NULL)
	{
		m_body->stopAllActions();
		DelayTime *acdt = DelayTime::create(0.5f);
		JumpTo *acjt = JumpTo::create(1.0f,m_body->getPosition() +  Vec2(rand()%200 - 100,-500),40,1);
		Sequence *se = Sequence::create(acdt,acjt,CallFunc::create([&](){
			log("end");
			this->Stop();
		}),NULL);
		m_body->runAction(se);
	}
}
MainScene::DIECODE MainScene::CheckDie()
{
	if(m_posX>=m_gmap->getLength()-3)
		return DIECODE::MAPEND;
	int nowLevel = m_gmap->getLevel(m_posX);
	int nextLevel = m_gmap->getLevel(m_posX+1);
	int farLevel = m_gmap->getLevel(m_posX+2);
	int subLevel = nextLevel - nowLevel;
	int subFarLevel = farLevel - nowLevel;

	//ײǽ
	if(nowLevel >m_posY)
		return DIECODE::UPDIE;
	//��ǰposX���Ƿ�������
	if(m_gmap->isTrap(m_posX))
	{
		if(m_gmap->isFlyTrap(m_posX))
		{
			//�Ƿ�������������
			if(m_posY - nowLevel > 0)
				return DIECODE::FLYTRAP;
			else
				return DIECODE::NODIE;
		}
		return DIECODE::TRAP;
	}
	//���Drop
	if(m_gmap->isDrop(m_posX))
	{
		return DIECODE::DROPDOWN;
	}
	
	return DIECODE::NODIE;
}
void MainScene::Die(DIECODE diecode)
{
	log("die");
	//ֹͣ��ʱ��
	this->StopStep();
	switch (diecode)
	{
	case DIECODE::NONE:
		log("unknow die");
		break;
	case DIECODE::DROPDOWN:
		log("fall ");
		//m_gmap->animationDropDown(m_posX,m_speed);
		this->animationBodyDrop(m_speed);
		break;
	case DIECODE::UPDIE:
		log("mission faild:hurt");
		break;
	case DIECODE::MAPEND:
		log("mission completed");
		break;
	case DIECODE::TRAP:
		log("mission faild:trap");
		break;
	case DIECODE::FLYTRAP:
		log("mission faild:flytrap");
		break;
	default:
		break;
	}
}
bool MainScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	log("Touched Begin");
	if(!m_status)
	{
		//��δ��ʼ
		m_status = true;
		this->Start();
	}else{
		//�Ѿ���ʼ
		this->m_jump = true;
	}
	return false;
}
void MainScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	log("Touched Moved");
}
void MainScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	log("Touched Ended");

}