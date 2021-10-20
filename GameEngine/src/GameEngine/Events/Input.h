#pragma once

#include "gepch.h"

#include "KeyCodes.h"
#include "MouseCodes.h"
#include "MouseEvent.h"

#include "glm/vec2.hpp"

namespace GameEngine {

	class Input
	{
	protected:
		Input() = default;

	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		virtual ~Input() {};

		inline static glm::vec2 GetMousePosition() { return s_instance->GetMousePositionImpl(); }
		inline static bool GetMouseDown(MouseButton button) { return s_instance->GetMouseDownImpl(button); }
		inline static bool GetKeyDown(KeyCode keycode) { return s_instance->GetKeyDownImpl(keycode); }
		inline static const char* GetClipBoardText() { return s_instance->GetClipBoardTextImpl(); }

		inline static void SetMouseCursor(MouseCursor cursor) { return s_instance->SetMouseCursorImpl(cursor); }
		inline static void SetClipBoardText(const char* text) { return s_instance->SetClipBoardTextImpl(text); }

		inline static void LockMouseCursor() { return s_instance->LockMouseCursorImpl(); }
		inline static void UnlockMouseCursor() { return s_instance->UnlockMouseCursorImpl(); }

	protected:
		virtual glm::vec2 GetMousePositionImpl() = 0;
		virtual bool GetMouseDownImpl(MouseButton button) = 0;
		virtual bool GetKeyDownImpl(KeyCode keycode) = 0;
		virtual const char* GetClipBoardTextImpl() = 0;

		virtual void SetMouseCursorImpl(MouseCursor cursor) = 0;
		virtual void SetClipBoardTextImpl(const char* text) = 0;

		virtual void LockMouseCursorImpl() = 0;
		virtual void UnlockMouseCursorImpl() = 0;

	private:
		static Scope<Input> s_instance;

		friend class Window;
	};

}

