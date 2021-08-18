#pragma once

#include "gepch.h"
#include "GameEngine/Core.h"
#include "GameEngine/Events/Event.h"
#include "GameEngine/Events/MouseEvent.h"

namespace GameEngine {

	/**
	* Props needed to create a Window.
	*/
	struct WindowProps {
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Game Engine", 
					unsigned int width = 640, 
					unsigned int height = 360) :
			title(title), width(width), height(height) {}
	};

	/**
	* Window abstraction. To be defined per platform.
	*/
	class GE_API Window {
	public:
		using EventCallbackFn = std::function<void(const Event&)>;

		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;

		struct CursorPos {
			float x;
			float y;
		};
		virtual CursorPos GetCursorPos() = 0;
		virtual bool GetMouseButton(MouseButton button) = 0;

		enum MouseCursor {
			MouseCursor_None = -1,
			MouseCursor_Arrow = 0,
			MouseCursor_Hand,
			MouseCursor_TextInput,
			MouseCursor_ResizeNS,		  // When hovering over a horizontal border
			MouseCursor_ResizeEW,		  // When hovering over a vertical border or a column
			MouseCursor_ResizeNESW,		  // When hovering over the bottom-left corner of a window
			MouseCursor_ResizeNWSE,		  // When hovering over the bottom-right corner of a window
			MouseCursor_ResizeAll,
			MouseCursor_NotAllowed,
			MouseCursor_Count,
			MouseCursor_ResizeVertical = MouseCursor_ResizeNS,
			MouseCursor_ResizeHorizontal = MouseCursor_ResizeEW
		};
		virtual void SetMouseCursor(MouseCursor cursor) = 0;

		virtual const char* GetClipboardText() = 0;
		virtual void SetClipboardText(const char* text) = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() = 0;

		// to be defined in platform specific files
		static Window* Create(const WindowProps& props = WindowProps());
	};

}
