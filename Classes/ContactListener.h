#ifndef ContactListener_h
#define ContactListener_h

#include <stdio.h>
#include "cocos2d.h"
#include "external/Box2D/include/Box2D/Box2D.h"


struct MyContact
{
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    
    bool operator==(const MyContact &other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};


class ContactListener : public b2ContactListener
{
public:
    ContactListener();
    ~ContactListener();
    virtual void BeginContact(b2Contact *contact);
    virtual void EndContact(b2Contact *contact);
    std::vector<MyContact> contacts_;
};


#endif /* ContactListener_h */
