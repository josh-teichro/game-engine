#pragma once

#include "GameEngine/Core.h"
#include <string>
#include <functional>

namespace GameEngine {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMove,
		AppTick, AppUpdate, AppRender,
		KeyPress, KeyRelease,
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	class GE_API Event {
	public:
		inline EventType GetEventType() const { return c_eventType; }
		inline const char* GetName() const { return c_name; }
		inline int GetCategoryFlags() const { return c_categoryFlags; }
		virtual std::string ToString() const { return c_name; }

		inline bool IsInCategory(EventCategory category) const { 
			return c_categoryFlags & category; 
		}

	protected:
		Event(EventType eventType, char* name, int categoryFlags) :
			c_eventType(eventType),
			c_name(name),
			c_categoryFlags(categoryFlags) {}

		const char* c_name;
		const EventType c_eventType;
		const int c_categoryFlags;
		bool m_Handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.m_Handled |= func(*(T*)&m_event);
				return true;
			}
			return false;
		}
	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}
