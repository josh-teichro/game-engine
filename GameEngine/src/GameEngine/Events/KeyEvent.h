#pragma once

#include "gepch.h"
#include "Event.h";

namespace GameEngine {

	class GE_API KeyEvent : public Event {
	public:
		const int keyCode;

		std::string ToString() const override
		{
			return "KeyEvent";
		}

	protected:
		KeyEvent(int keyCode) :
			keyCode(keyCode) {}
	};

	class GE_API KeyDownEvent : public KeyEvent {
	public:
		const int repeatCount;

		KeyDownEvent(int keyCode, int repeatCount) :
			KeyEvent(keyCode),
			repeatCount(repeatCount) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyDownEvent: " << keyCode << " (" << repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyDownEvent)
	};

	class GE_API KeyUpEvent : public KeyEvent
	{
	public:
		KeyUpEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyUpEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyUpEvent)
	};

}