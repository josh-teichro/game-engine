#pragma once

#include "gepch.h"
#include "Events/MouseEvent.h"
#include "Events/KeyCodes.h"

namespace GameEngine {

	class Input
	{
	public:
		virtual ~Input() {};

		struct MousePosition {
			float x;
			float y;
		};
		inline static MousePosition GetMousePosition() { return s_instance->GetMousePositionImpl(); };
		inline static bool GetMouseDown(MouseButton button) { return s_instance->GetMouseDownImpl(button); };
		inline static bool GetKeyDown(KeyCode keycode) { return s_instance->GetKeyDownImpl(keycode); };

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
		inline static void SetMouseCursor(MouseCursor cursor) { return s_instance->SetMouseCursorImpl(cursor); };
		
		inline static const char* GetClipBoardText() { return s_instance->GetClipBoardTextImpl(); };
		inline static void SetClipBoardText(const char* text) { return s_instance->SetClipBoardTextImpl(text); };

	protected:
		virtual MousePosition GetMousePositionImpl() = 0;
		virtual bool GetMouseDownImpl(MouseButton button) = 0;
		virtual bool GetKeyDownImpl(KeyCode keycode) = 0;

		virtual void SetMouseCursorImpl(MouseCursor cursor) = 0;

		virtual const char* GetClipBoardTextImpl() = 0;
		virtual void SetClipBoardTextImpl(const char* text) = 0;

	private:
		static Input* s_instance;

		friend class Window;
	};

}

