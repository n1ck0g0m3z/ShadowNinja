
#include "WorldScene.h"

USING_NS_CC;

Scene* World::createScene()
{
    auto scene = Scene::createWithPhysics();
    
    auto layer = World::create();
    scene->getPhysicsWorld()->setSpeed(3.0f);
    scene->addChild(layer);
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    return scene;
}

bool World::init()
{
    if ( !LayerGradient::initWithColor(Color4B(170,145,25,255),Color4B(17,220,160,255),Point(5,5))) //fondo gradiente
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto backGround = Sprite::create("fondoanimado-1.png");
    backGround->setPosition(Point(0,0));
    backGround->setAnchorPoint(Point(0,0));
    this->addChild(backGround,0,5);
    
    Animation *fondo = Animation::create();
    
    fondo->addSpriteFrameWithFile("fondoanimado-2.png");
    fondo->addSpriteFrameWithFile("fondoanimado-3.png");
    fondo->addSpriteFrameWithFile("fondoanimado-4.png");
    fondo->addSpriteFrameWithFile("fondoanimado-5.png");
    fondo->addSpriteFrameWithFile("fondoanimado-6.png");
    fondo->addSpriteFrameWithFile("fondoanimado-7.png");
    fondo->addSpriteFrameWithFile("fondoanimado-8.png");
    
    fondo->setRestoreOriginalFrame(true);   // 最初の画像に戻すかどうか
    fondo->setDelayPerUnit(0.5f / 4.0f);
    
    // パラパラアニメを動かす
    Animate *animate = Animate::create(fondo);
    RepeatForever *animated = RepeatForever::create(animate);

    backGround->runAction(animated);
    
    auto backGround1 = Sprite::create("fondoanimado-111.png");
    backGround1->setPosition(Point(visibleSize.width ,0));
    backGround1->setAnchorPoint(Point(0,0));
    this->addChild(backGround1,0,6);
    
    Animation *fondo1 = Animation::create();
    
    fondo1->addSpriteFrameWithFile("fondoanimado-222.png");
    fondo1->addSpriteFrameWithFile("fondoanimado-333.png");
    fondo1->addSpriteFrameWithFile("fondoanimado-444.png");
    fondo1->addSpriteFrameWithFile("fondoanimado-555.png");
    fondo1->addSpriteFrameWithFile("fondoanimado-666.png");
    fondo1->addSpriteFrameWithFile("fondoanimado-777.png");
    fondo1->addSpriteFrameWithFile("fondoanimado-888.png");
    
    fondo1->setRestoreOriginalFrame(true);   // 最初の画像に戻すかどうか
    fondo1->setDelayPerUnit(0.5f / 4.0f);
    
    // パラパラアニメを動かす
    Animate *animate1 = Animate::create(fondo1);
    RepeatForever *animated1 = RepeatForever::create(animate1);
    backGround1->runAction(animated1);
    
	start();

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(World::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/4 ,
                                origin.y + closeItem->getContentSize().height/4));
    closeItem->setScale(0.5f);
    
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    return true;
}

void World::animNinja()//animar y crear en ninja
{
	auto player = (Sprite*)this->getChildByTag(1);
	Animation *ninja = Animation::create();
    
    ninja->addSpriteFrameWithFile("ninja2.png");
    ninja->addSpriteFrameWithFile("ninja3.png");
    ninja->addSpriteFrameWithFile("ninja4.png");
    ninja->addSpriteFrameWithFile("ninja5.png");
    ninja->addSpriteFrameWithFile("ninja6.png");
    
    ninja->setRestoreOriginalFrame(true);   // 最初の画像に戻すかどうか
    ninja->setDelayPerUnit(0.5f / 5.0f);
    
    // パラパラアニメを動かす
    Animate *ninjaAn = Animate::create(ninja);
    RepeatForever *ninjAni = RepeatForever::create(ninjaAn);
    player->runAction(ninjAni);
}

void World::initGame(Ref* pSender)//inicio del juego
{
	auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(World::onTouchBegan, this);
    listener->onTouchCancelled = CC_CALLBACK_2(World::onTouchCancelled, this);
    listener->onTouchEnded = CC_CALLBACK_2(World::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->removeChildByTag(56);
	this->removeChildByTag(57);
    //inicializar los puntajes
	this->score=0;
    this->life=15;
    this->speed=2.5f;
	this->addScore();
    this->touchActive = false;
    this->jump = false;
    
	auto player = Sprite::create("ninja10.png");
    player->setTag(1);
    player->setAnchorPoint(Point(0,0));
    player->setPosition(Vec2(70,50));
    auto physicsBody = PhysicsBody::createBox(Size(player->getContentSize().width,player->getContentSize().height),PhysicsMaterial(0.1f,1.0f,0.0f));
    player->setPhysicsBody(physicsBody);
    player->getPhysicsBody()->setGravityEnable(false);
    movement = Vec2(70,50);
    this->addChild(player,1);

    animNinja();

    this->schedule(schedule_selector(World::gameLogic), 1);
	this->scheduleUpdate();

}

bool World::onTouchBegan(Touch *touch, Event *event)//tocar la pantalla
{
    touchActive = true;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Vec2 location = touch->getLocation();
    
    accionTouch = *touch;
    
    auto player = (Sprite*) this->getChildByTag(1);
    
    if (location.y>visibleSize.height/3 && player->getPosition().y<=50 && jump==false) {
        player->getPhysicsBody()->setGravityEnable(true);
        player->getPhysicsBody()->setVelocity(Vec2(0,150));
        jump = true;
    }
    return true;
}

void World::onTouchEnded(Touch *touch, Event *event){//finalizar el toque
    touchActive=false;
}

void World::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    onTouchEnded(touch, unused_event);
}

void World::update(float delta)//variaciones en el juego 120 creo yo :p veces por segundo
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto spr1 = (Sprite*)this->getChildByTag(5);//fondo bloque 1
    auto spr2 = (Sprite*)this->getChildByTag(6);//fondo bloque 2
    Point pos1 = spr1->getPosition();
    Point pos2 = spr2->getPosition();
    
    pos1.x -= this->speed;
    pos2.x -= this->speed;
    
    if (pos1.x < -(visibleSize.width))
    {
        pos1.x = visibleSize.width;
    }
    
    if (pos2.x < -(visibleSize.width)){
        pos2.x = visibleSize.width;
    }
    
    Vec2 location = accionTouch.getLocation();
    
    if(touchActive && jump == false && location.y<=visibleSize.height/3){
        
        auto player = (Sprite*) this->getChildByTag(1);
        
        if(location.x>visibleSize.width/2){
            movement.x += 3;
            log("mayor %f",movement.x);
        }else if (location.x<visibleSize.width/2){
            movement.x -= 2;
            log("menor %f",movement.x);
        }
        
        movement = movement.getClampPoint(Vec2(0,15),Vec2(visibleSize.width - player->getContentSize().width,
                                                          visibleSize.height - player->getContentSize().height));
        
        player->setPosition(movement);
    }
    
    spr1->setPosition(pos1);
    spr2->setPosition(pos2);
    
    auto player = (Sprite*)this->getChildByTag(1);
    
    if(player->getPositionY()<=50){
		player->setPositionY(50);
		movement.y = player->getPositionY();
		movement.x = player->getPositionX();
        player->getPhysicsBody()->setGravityEnable(false);
        player->getPhysicsBody()->setVelocity(Vec2(0,0));
		jump = false;
    }
    
    auto playerRect = Rect(player->getPositionX() , player->getPositionY() , player->getContentSize().width*2.0f, player->getContentSize().height/2.7f);
	auto eliminateRect = Rect(player->getPositionX(), player->getPositionY(), player->getContentSize().width, player->getContentSize().height/2.7f);

    auto food = (Sprite*)this->getChildByTag(2);
    if(food==nullptr)
		return;
    
    auto shurikenRect = Rect(food->getPositionX() - (food->getContentSize().width/2), food->getPositionY() - (food->getContentSize().height/2), food->getContentSize().width/2, food->getContentSize().height/2);
    
    if(playerRect.intersectsRect(shurikenRect))
    {
        this->score += 1;
    
        this->scheduleOnce(schedule_selector(World::eliminate), 0.01f);
        
		//if (eliminateRect.intersectsRect(shurikenRect))
		//{
			this->removeChild(food, true);
		//}
    }

	if(this->life == 0){

		auto scene = World::createScene();
		Director::getInstance()->replaceScene(scene);
		
	}

}

void World::eliminate(float delta)
{
    auto player = (Sprite*)this->getChildByTag(1);
    
    /*Vector<SpriteFrame*> frames;
    
    for(int i=0;i<4;i++){
        auto rect = Rect(88*i, 0, 88, 84);
        auto frame = SpriteFrame::create("ninja.png", rect);
        frames.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(frames,0.08f);
    animation->setLoops(1);
    animation->setRestoreOriginalFrame(true);*/
    
    auto ninja = Animation::create();
    
    ninja->addSpriteFrameWithFile("ninja9.png");
    //ninja->addSpriteFrameWithFile("ninja10.png");
    ninja->addSpriteFrameWithFile("ninja11.png");
    ninja->addSpriteFrameWithFile("ninja13.png");
    ninja->addSpriteFrameWithFile("ninja15.png");
    //ninja->addSpriteFrameWithFile("ninja16.png");

	ninja->setDelayPerUnit(0.09f);
    ninja->setLoops(1);
    ninja->setRestoreOriginalFrame(true);
    
    auto label = (Label*)this->getChildByTag(21);
    
    auto points = String::createWithFormat("%d", (int)this->score);
    
    label->setString(points->getCString());
    
    auto action = Animate::create(ninja);
    
    player->runAction(action);
    //player->runAction(Animate::create(animation));
}

void World::addStar()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto star = Sprite::create("shuriken.png");
    
    auto rotate = RotateBy::create(0.5, 360);
    
    RepeatForever* rero = RepeatForever::create(rotate);
    
    star->runAction(rero);
    
    int height = 70 + (rand() % ((int)visibleSize.height - 120));
    star->setTag(2);
    star->setPosition(Point(visibleSize.width + star->getContentSize().width/2 ,height));
    this->addChild(star,1);
    
    float duration = 2.0f;
    
    auto actionMove = MoveTo::create(duration, Point(star->getContentSize().width/2 ,star->getPositionY()));
    
    auto actionMoveDone = CallFuncN::create(CC_CALLBACK_1(World::starMoveFinished, this));
    
    auto seq = Sequence::create(actionMove,actionMoveDone,nullptr);
    
    star->runAction(seq);
}

void World::addScore()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto scoreLabel = Label::createWithTTF("Life Points " , "fonts/KingsButcher-Regular.ttf", 15);
    scoreLabel->setTextColor(Color4B::BLACK);
    this->addChild(scoreLabel, 1,10);
    
    auto points = String::createWithFormat("%d", (int)this->life);
	auto pointsLabel = Label::createWithTTF(points->getCString(), "fonts/KingsButcher-Regular.ttf", 15);
    pointsLabel->setTextColor(Color4B::BLACK);
    
    scoreLabel->setAnchorPoint(Point(0,1));
    scoreLabel->setPosition(Vec2(10,visibleSize.height));

    pointsLabel->setAnchorPoint(Point(0,1));
    pointsLabel->setPosition(Vec2(scoreLabel->getPositionX() + scoreLabel->getContentSize().width,
                                  visibleSize.height));
    
    this->addChild(pointsLabel, 1,11);
    
    auto scoreLabelScore = Label::createWithTTF("Score" , "fonts/KingsButcher-Regular.ttf", 15);
    scoreLabelScore->setTextColor(Color4B::BLUE);
    this->addChild(scoreLabelScore, 1,20);

	auto pointsLabelScore = Label::createWithTTF("0", "fonts/KingsButcher-Regular.ttf", 15);
    pointsLabelScore->setTextColor(Color4B::BLUE);
    
    scoreLabelScore->setAnchorPoint(Point(1,1));
	pointsLabelScore->setPosition(Vec2(visibleSize.width - pointsLabelScore->getContentSize().width,
                                 visibleSize.height));
    
    pointsLabelScore->setAnchorPoint(Point(1,1));
    scoreLabelScore->setPosition(Vec2(visibleSize.width - scoreLabelScore->getContentSize().width+5,
                                  visibleSize.height));
    
    this->addChild(pointsLabelScore, 1,21);
}

void World::gameLogic(float a)
{
	this->addStar();

}

void World::starMoveFinished(Node* sender)
{
    auto sprite = (Sprite *) sender;
    bool isCleanUp = true;
    
    this->life-=1;
    
    auto label = (Label*)this->getChildByTag(11);
     
    auto points = String::createWithFormat("%d", (int)this->life);
     
    label->setString(points->getCString());
    
    this->removeChild(sprite,isCleanUp);
}

void World::start(){

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto startLabel = Label::createWithTTF("ShAdoW NinjA" , "fonts/KingsButcher-Regular.ttf", 29);
    startLabel->setTextColor(Color4B::BLACK);
	startLabel->setPosition(Point(visibleSize.width/2,visibleSize.height/2));

    this->addChild(startLabel, 1,56);
	
	auto pLabel = Label::createWithTTF("Touch here","fonts/KingsButcher-Regular.ttf" , 23);
	pLabel->setTextColor(Color4B(0,0,0,128));
	auto pBtnItem = MenuItemLabel::create(pLabel, CC_CALLBACK_1(World::initGame, this));
 
	//メニューの作成　pMenuの中にpBtnItemを入れる
	Menu* pMenu = Menu::create(pBtnItem, NULL);
 
	//pMenuを画面中央に配置
	pMenu->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 - pLabel->getContentSize().height));

	this->addChild(pMenu, 1 , 57);

}

void World::menuCloseCallback(Ref* pSender)
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
