#pragma once

#include "GameEngine/Core/Input.h"

struct GLFWcursor;

namespace GameEngine {

	class WindowsInput : public Input
	{
	public:
		WindowsInput();
		virtual ~WindowsInput();

	protected:
		virtual void InitImpl() override;
		virtual void ShutdownImpl() override;

		virtual glm::vec2 GetMousePositionImpl() override;
		virtual bool GetMouseDownImpl(MouseButton button) override;
		virtual bool GetKeyDownImpl(KeyCode keycode) override;
		virtual const char* GetClipBoardTextImpl() override;

		virtual void SetMouseCursorImpl(MouseCursor cursor) override;
		virtual void SetClipBoardTextImpl(const char* text) override;

		virtual void LockMouseCursorImpl() override;
		virtual void UnlockMouseCursorImpl() override;

	private:
		GLFWcursor* m_cursors[(int)MouseCursor::Count];
		bool m_isCursorLocked;

	};

}
