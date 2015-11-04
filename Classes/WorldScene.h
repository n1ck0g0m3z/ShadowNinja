#ifndef __Sumo__WorldScene__
#define __Sumo__WorldScene__

#include <stdio.h>
#include "cocos2d.h"
//#include "extensions/cocos-ext.h"


class World : public cocos2d::LayerGradient
{
private:
    float speed;    
    int life;
    cocos2d::Vec2 movement;
    bool touchActive;
    bool jump;
	bool col;
    int score;
    cocos2d::Touch accionTouch;
public:
   
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);

	void addScore();
    
    void addStar();
    
    void gameLogic(float a);
    
    void starMoveFinished(Node* sender);

	void initGame(Ref* pSender);
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    void update(float delta);
    
	void animNinja();

    void eliminate(float delta);

	void start();
    

    
    CREATE_FUNC(World);
};

#endif /* defined(__Sumo__WorldScene__) */

