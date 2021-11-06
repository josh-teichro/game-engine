#pragma once

#include "gepch.h"

#include "GameEngine/Core/Core.h"

#define BIND_EVENT_FN(x) std::bind(x, this, std::placeholders::_1)

namespace GameEngine {

	/**
	* Event types.
	*/
	enum class EventType {
		UndefinedEvent,
		WindowCloseEvent,
		WindowResizeEvent,
		KeyDownEvent,
		KeyUpEvent,
		CharTypedEvent,
		MouseMoveEvent,
		MouseScrollEvent,
		MouseDownEvent,
		MouseUpEvent,
		NumEventTypes
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

/**
* Define class type for an event class
*/
#define EVENT_CLASS_TYPE(type) \
	static EventType GetStaticEventType() { return EventType::type; }\
	virtual EventType GetEventType() const override { return GetStaticEventType(); }

#define EVENT_CLASS_CATEGORY(category) \
	virtual int GetCategoryFlags() const override { return category; }

	/**
	* Event base class.
	*/
	class Event {
	public:
		virtual std::string ToString() const { return "Event"; }

		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;

		bool IsInCategory(EventCategory category) const
		{
			return GetCategoryFlags() & category;
		}
	};

	/**
	* Allows printing events to console or to the logging system
	*/
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

	/**
	* Dispatcher used to assign callbacks for specific event types.
	*/
	class EventDispatcher {
	public:
		EventDispatcher(const Event& event) : m_event(event) {}

		template <typename T, typename F>
		bool Dispatch(F callback)
		{
			if (m_event.GetEventType() == T::GetStaticEventType()) {
				return callback(static_cast<const T&>(m_event));
			}

			return false;
		}

	private:
		const Event& m_event;
	};

}
