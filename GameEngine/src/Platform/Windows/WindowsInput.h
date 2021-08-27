#pragma once

#include "GameEngine/Input.h";

class GLFWcursor;

namespace GameEngine {

	class GE_API WindowsInput : public Input
	{
	public:
		WindowsInput();
		virtual ~WindowsInput();

	protected:
		virtual MousePosition GetMousePositionImpl() override;
		virtual bool GetMouseDownImpl(MouseButton button) override;
		virtual bool GetKeyDownImpl(KeyCode keycode) override;

		virtual void SetMouseCursorImpl(MouseCursor cursor) override;

		virtual const char* GetClipBoardTextImpl() override;
		virtual void SetClipBoardTextImpl(const char* text) override;

	private:
		GLFWcursor* m_cursors[MouseCursor_Count];

	};

}
