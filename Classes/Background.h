#ifndef Background_h
#define Background_h

#include <stdio.h>
#include "cocos2d.h"
#include "GameCamera.h"


class Background : public cocos2d::Node
{
public:
    static Background * create(unsigned int level_id);

private:
    ~Background();
    
    CREATE_FUNC(Background);
    
    virtual bool init();
    
    cocos2d::Sprite * background_0_;
    cocos2d::Sprite * background_1_;
};

#endif /* Background_h */
