#include "Object.h"

using namespace CMD_3D_ENGINE;

std::list<std::shared_ptr<Object>> Object::objectList;

const std::list<std::shared_ptr<Object>>& Object::getObjectList()
{
    return objectList;
}

Object::Object() : position(0.0f, 0.0f), sprite(std::make_shared<Sprite>(0, 0))
{
    objectList.push_back(std::shared_ptr<Object>(this));
}

Object::Object(const Vec2D& position, std::wstring spritePath)
    : position(position), sprite(std::make_shared<Sprite>(spritePath))
{
    objectList.push_back(std::shared_ptr<Object>(this));
}

Object::Object(const Vec2D& position, std::shared_ptr<const Sprite> sprite) : position(position), sprite(sprite)
{
    objectList.push_back(std::shared_ptr<Object>(this));
}

Object::Object(const Object& object) : position(object.position), sprite(object.getSprite())
{
    objectList.push_back(std::shared_ptr<Object>(this));
}

Object& Object::operator=(const Object& right)
{
    if (this != &right) {
        this->position = right.position;
        this->sprite = right.getSprite();
    }
    return *this;
}

Object::~Object()
{
    for (auto it = objectList.begin(); it != objectList.end(); ++it) {
        if (it->get() == this) { // remove object from list
            objectList.erase(it);
            break;
        }
    }
}

const Vec2D& Object::getPosition() const
{
    return position;
}

std::shared_ptr<const Sprite> Object::getSprite() const
{
    return sprite;
}

void Object::setPosition(const Vec2D& position)
{
    this->position = position;
}

void Object::setSprite(std::shared_ptr<const Sprite> sprite)
{
    this->sprite = sprite;
}