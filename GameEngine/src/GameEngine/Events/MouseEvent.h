#pragma once

#include "GameEngine/Core.h"
#include "Event.h"
#include <sstream>

namespace GameEngine {

	class GE_API MouseEvent : public Event {
	public:

	protected:
		MouseEvent(EventType eventType, char* name, float x, float y, int categoryFlags = 0) :
			Event(eventType, name, EventCategoryMouse | EventCategoryInput | categoryFlags),
			m_mouseX(x),
			m_mouseY(y) {}

		float m_mouseX, m_mouseY;
	};

	class GE_API MouseMoveEvent : public MouseEvent {
	public:
		MouseMoveEvent(float x, float y) :
			MouseEvent(EventType::MouseMove, "Mouse Move", x, y) {}
	};

	class GE_API MouseScrollEvent : public MouseEvent {
	public:
		MouseScrollEvent(float x, float y, float offsetX, float offsetY) :
			MouseEvent(EventType::MouseMove, "Mouse Move", x, y),
			m_offsetX(offsetX),
			m_offsetY(offsetY) {}

		inline float GetOffsetX() const { return m_offsetX; }
		inline float GetOffsetY() const { return m_offsetY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << GetOffsetX() << ", " << GetOffsetY();
			return ss.str();
		}

	private:
		float m_offsetX, m_offsetY;
	};

	class GE_API MouseButtonPressEvent : public MouseEvent {
	public:
		MouseButtonPressEvent(float x, float y) :
			MouseEvent(EventType::MouseMove, "Mouse Move", x, y) {}
	};

}