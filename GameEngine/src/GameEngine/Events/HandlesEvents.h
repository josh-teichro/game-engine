#pragma once

#include "Event.h"
#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace GameEngine {

	/**
	* Interface which implements empty callbacks for every Event type and binds
	* them to the corresponding event. Subclasses of this class need only override
	* the function corresponding to the event that they want to subscribe to in 
	* order to setup the callback for that event.
	*/
	class HandlesEvents
	{
	public:
		bool OnEvent(const Event& e)
		{
			switch (e.GetEventType())
			{
				case EventType::WindowCloseEvent: return OnWindowClose(*(WindowCloseEvent*)&e);
				case EventType::WindowResizeEvent: return OnWindowResize(*(WindowResizeEvent*)&e);
				case EventType::KeyDownEvent: return OnKeyDown(*(KeyDownEvent*)&e);
				case EventType::KeyUpEvent: return OnKeyUp(*(KeyUpEvent*)&e);
				case EventType::CharTypedEvent: return OnCharTyped(*(CharTypedEvent*)&e);
				case EventType::MouseMoveEvent: return OnMouseMove(*(MouseMoveEvent*)&e);
				case EventType::MouseScrollEvent: return OnMouseScroll(*(MouseScrollEvent*)&e);
				case EventType::MouseDownEvent: return OnMouseDown(*(MouseDownEvent*)&e);
				case EventType::MouseUpEvent: return OnMouseUp(*(MouseUpEvent*)&e);
				default: return false;
			}
		}

		virtual bool OnWindowClose(const WindowCloseEvent& e) { return false; }
		virtual bool OnWindowResize(const WindowResizeEvent& e) { return false; }
		virtual bool OnKeyDown(const KeyDownEvent& e) { return false; }
		virtual bool OnKeyUp(const KeyUpEvent& e) { return false; }
		virtual bool OnCharTyped(const CharTypedEvent& e) { return false; }
		virtual bool OnMouseMove(const MouseMoveEvent& e) { return false; }
		virtual bool OnMouseScroll(const MouseScrollEvent& e) { return false; }
		virtual bool OnMouseDown(const MouseDownEvent& e) { return false; }
		virtual bool OnMouseUp(const MouseUpEvent& e) { return false; }
	};

}
