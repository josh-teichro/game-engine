#pragma once

#include "gepch.h"
#include "Event.h"
#include "KeyCodes.h"

namespace GameEngine {

	class GE_API KeyEvent : public Event {
	public:
		const KeyCode keyCode;

		std::string ToString() const override
		{
			return "KeyEvent";
		}

	protected:
		KeyEvent(KeyCode keyCode) :
			keyCode(keyCode) {}
	};

	class GE_API KeyDownEvent : public KeyEvent {
	public:
		const int repeatCount;

		KeyDownEvent(KeyCode keyCode, int repeatCount) :
			KeyEvent(keyCode),
			repeatCount(repeatCount) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyDownEvent: " << (int)keyCode << " (" << repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyDownEvent)
	};

	class GE_API KeyUpEvent : public KeyEvent
	{
	public:
		KeyUpEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyUpEvent: " << (int)keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyUpEvent)
	};

	class GE_API CharTypedEvent : public Event
	{
	public:
		unsigned int character;

		CharTypedEvent(int character) :
			character(character) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CharTypedEvent: " << (int)character;
			return ss.str();
		}

		EVENT_CLASS_TYPE(CharTypedEvent)
	};

}