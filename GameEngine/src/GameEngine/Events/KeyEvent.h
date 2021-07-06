#pragma once

#include "Event.h";
#include <sstream>

namespace GameEngine {

	class GE_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_keyCode; }

	protected:
		KeyEvent(EventType eventType, char* name, int keyCode) :
			Event(eventType, name, EventCategoryKeyboard | EventCategoryInput),
			m_keyCode(keyCode) {}

		int m_keyCode;
	};

	class GE_API KeyPressEvent : public KeyEvent {
	public:
		KeyPressEvent(int keyCode, int repeatCount) : 
			KeyEvent(EventType::KeyPress, "Key Press", keyCode),
			m_repeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_repeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}

	private:
		int m_repeatCount;
	};

	class GE_API KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keycode)
			: KeyEvent(EventType::KeyRelease, "Key Release", keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << m_keyCode;
			return ss.str();
		}
	};

}
