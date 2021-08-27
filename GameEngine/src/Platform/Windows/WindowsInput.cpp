#include "gepch.h"
#include "WindowsInput.h"

#include "GameEngine/Application.h"

#include <GLFW/glfw3.h>

namespace GameEngine {

	Input* Input::s_instance = nullptr;

	WindowsInput::WindowsInput()
	{
		// GLFW cursors
		// (By design, on X11 cursors are user configurable and some cursors may be missing. When a cursor doesn't exist,
		// GLFW will emit an error which will often be printed by the app, so we temporarily disable error reporting.
		// Missing cursors will return NULL and our SetMouseCursor() function will use the Arrow cursor instead.)
		GLFWerrorfun prev_error_callback = glfwSetErrorCallback(NULL);
		m_cursors[MouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_cursors[MouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
		m_cursors[MouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		m_cursors[MouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
		m_cursors[MouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
		m_cursors[MouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
		m_cursors[MouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
		m_cursors[MouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
		m_cursors[MouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
		glfwSetErrorCallback(prev_error_callback);
	}

	WindowsInput::~WindowsInput()
	{
		// Destroy GLFW cursors
		for (int cursor_n = 0; cursor_n < MouseCursor_Count; cursor_n++)
			glfwDestroyCursor(m_cursors[cursor_n]);
	}

	Input::MousePosition WindowsInput::GetMousePositionImpl()
	{
		double x, y;
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	bool WindowsInput::GetMouseDownImpl(MouseButton button) 
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, (int)button) == GLFW_PRESS;
	}

	bool WindowsInput::GetKeyDownImpl(KeyCode keycode) 
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, (int)keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	void WindowsInput::SetMouseCursorImpl(Input::MouseCursor cursor)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		if (cursor == MouseCursor_None) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else {
			glfwSetCursor(window, m_cursors[cursor] ? m_cursors[cursor] : m_cursors[MouseCursor_Arrow]);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	const char* WindowsInput::GetClipBoardTextImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetClipboardString(window);
	}

	void WindowsInput::SetClipBoardTextImpl(const char* text)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetClipboardString(window, text);
	}

}
