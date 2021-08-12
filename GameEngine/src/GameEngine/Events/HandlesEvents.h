#pragma once

#include "Event.h"
#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace GameEngine {

#define BIND_EVENT_FN(x) std::bind(&HandlesEvents::x, this, std::placeholders::_1)

	/**
	* Interface which implements empty callbacks for every Event type and binds
	* them to the corresponding event. Subclasses of this class need only override
	* the function corresponding to the event that they want to subscribe to in 
	* order to setup the callback for that event.
	*/
	class GE_API HandlesEvents
	{
	public:
		HandlesEvents() {}

		bool OnEvent(const Event& e)
		{
			return EventDispatcher(e).Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose))
				|| EventDispatcher(e).Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize))
				|| EventDispatcher(e).Dispatch<KeyDownEvent>(BIND_EVENT_FN(OnKeyDown))
				|| EventDispatcher(e).Dispatch<KeyUpEvent>(BIND_EVENT_FN(OnKeyUp))
				|| EventDispatcher(e).Dispatch<MouseMoveEvent>(BIND_EVENT_FN(OnMouseMove))
				|| EventDispatcher(e).Dispatch<MouseScrollEvent>(BIND_EVENT_FN(OnMouseScroll))
				|| EventDispatcher(e).Dispatch<MouseDownEvent>(BIND_EVENT_FN(OnMouseDown))
				|| EventDispatcher(e).Dispatch<MouseUpEvent>(BIND_EVENT_FN(OnMouseUp));
		}

		virtual bool OnWindowClose(const WindowCloseEvent& e) { return false; }
		virtual bool OnWindowResize(const WindowResizeEvent& e) { return false; }
		virtual bool OnKeyDown(const KeyDownEvent& e) { return false; }
		virtual bool OnKeyUp(const KeyUpEvent& e) { return false; }
		virtual bool OnMouseMove(const MouseMoveEvent& e) { return false; }
		virtual bool OnMouseScroll(const MouseScrollEvent& e) { return false; }
		virtual bool OnMouseDown(const MouseDownEvent& e) { return false; }
		virtual bool OnMouseUp(const MouseUpEvent& e) { return false; }
	};

}
