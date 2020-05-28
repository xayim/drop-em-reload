#include "ContactListener.h"


ContactListener::ContactListener() : contacts_()
{
}

ContactListener::~ContactListener()
{
    
}

void ContactListener::BeginContact(b2Contact *contact)
{
    MyContact myContact = {contact->GetFixtureA(), contact->GetFixtureB()};
    contacts_.push_back(myContact);
}

void ContactListener::EndContact(b2Contact *contact)
{
    MyContact myContact = {contact->GetFixtureA(), contact->GetFixtureB()};
    std::vector<MyContact>::iterator itrPosition;
    
    itrPosition = find(contacts_.begin(), contacts_.end(), myContact);
    contacts_.erase(itrPosition);
}
