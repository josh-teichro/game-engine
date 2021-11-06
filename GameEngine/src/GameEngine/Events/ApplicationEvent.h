#pragma once

#include "GameEngine/Events/Event.h"

namespace GameEngine {

	class ApplicationEvent : public Event
	{
	public:
		virtual std::string ToString() const override
		{
			return "ApplicationEvent";
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	protected: 
		ApplicationEvent() = default;
	};

	class WindowCloseEvent : public ApplicationEvent
	{
	public:
		WindowCloseEvent() = default;

		std::string ToString() const override
		{
			return "WindowCloseEvent";
		}

		EVENT_CLASS_TYPE(WindowCloseEvent)
	};

	class WindowResizeEvent : public ApplicationEvent
	{
	public:
		const int width, height;

		WindowResizeEvent(int width, int height) :
			width(width),
			height(height) {};

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << width << ", " << height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResizeEvent)
	};

}
