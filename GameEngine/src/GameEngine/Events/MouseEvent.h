#pragma once

#include "gepch.h"
#include "GameEngine/Core/Core.h"
#include "Event.h"
#include "MouseCodes.h"

namespace GameEngine {

	class MouseEvent : public Event {
	public:
		const float x, y;

	protected:
		MouseEvent(float x, float y) :
			x(x),
			y(y) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseEvent: " << x << ", " << y;
			return ss.str();
		}
	};

	class MouseMoveEvent : public MouseEvent {
	public:
		MouseMoveEvent(float x, float y) :
			MouseEvent(x, y) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << x << ", " << y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoveEvent)
	};

	class MouseScrollEvent : public MouseEvent {
	public:
		const float offsetX, offsetY;

		MouseScrollEvent(float x, float y, float offsetX, float offsetY) :
			MouseEvent(x, y),
			offsetX(offsetX),
			offsetY(offsetY) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << offsetX << ", " << offsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrollEvent)
	};

	class MouseDownEvent : public MouseEvent {
	public:
		const MouseButton button;

		MouseDownEvent(MouseButton button, float x, float y) :
			MouseEvent(x, y), button(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseDownEvent: " << x << ", " << y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseDownEvent)
	};

	class MouseUpEvent : public MouseEvent {
	public:
		const MouseButton button;

		MouseUpEvent(MouseButton button, float x, float y) :
			MouseEvent(x, y), button(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseUpEvent: " << x << ", " << y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseUpEvent)
	};

}