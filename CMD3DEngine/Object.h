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
		Vec2D position;
		std::shared_ptr<const Sprite> sprite = nullptr;

	public:
		static const std::list<std::shared_ptr<Object>>& getObjectList();

		Object();
		Object(const Vec2D& position, std::wstring spritePath);
		Object(const Vec2D& position, std::shared_ptr<const Sprite> sprite);	// shallow copy
		Object(const Object& object);

		virtual ~Object();

		Object& operator=(const Object& right);	// shallow copy

		// getters
		const Vec2D& getPosition() const;
		std::shared_ptr<const Sprite> getSprite() const;

		// setters
		void setPosition(const Vec2D& position);
		void setSprite(std::shared_ptr<const Sprite> sprite);
	};

}


