#pragma once

#include <list>
#include <memory>

#include "Vec2D.h"
#include "Sprite.h"

namespace CMD_3D_ENGINE
{

	class Object
	{
	private:
		static std::list<std::shared_ptr<Object>> objectList;
		std::shared_ptr<const Sprite> sprite = nullptr;
		bool remove = false;	// object will be removed from objectList if true

	protected:
		Vec2D position;
		
	public:
		static const std::list<std::shared_ptr<Object>>& getObjectList();
		static void cleanObjectList();	// remove objects marked with 'remove=true' from the list

		Object();
		Object(const Vec2D& position, std::wstring spritePath);
		Object(const Vec2D& position, std::shared_ptr<const Sprite> sprite);
		Object(const Object& object);	// shallow copy

		virtual ~Object();

		Object& operator=(const Object& right);	// shallow copy

		virtual void updatePhysics(float elapsedTime);

		// getters
		const Vec2D& getPosition() const;
		std::shared_ptr<const Sprite> getSprite() const;
		bool toBeRemoved() const;

		// setters
		void setPosition(const Vec2D& position);
		void setSprite(std::shared_ptr<const Sprite> sprite);
		void setRemove(bool remove);
	};

}


