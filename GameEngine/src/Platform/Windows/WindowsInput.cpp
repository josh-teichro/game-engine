#include "gepch.h"

#include "Platform/Windows/WindowsInput.h"
#include "GameEngine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace GameEngine {

	Scope<Input> Input::s_instance = MakeScope<WindowsInput>();

	WindowsInput::WindowsInput() :
		m_isCursorLocked(false)
	{
	}

	WindowsInput::~WindowsInput()
	{
	}

	void WindowsInput::InitImpl()
	{
		GE_PROFILE_FUNCTION();

		// GLFW cursors
		// (By design, on X11 cursors are user configurable and some cursors may be missing. When a cursor doesn't exist,
		// GLFW will emit an error which will often be printed by the app, so we temporarily disable error reporting.
		// Missing cursors will return NULL and our SetMouseCursor() function will use the Arrow cursor instead.)
		GLFWerrorfun prev_error_callback = glfwSetErrorCallback(NULL);
		m_cursors[(int)MouseCursor::Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_cursors[(int)MouseCursor::Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
		m_cursors[(int)MouseCursor::TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		m_cursors[(int)MouseCursor::ResizeNS] = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
		m_cursors[(int)MouseCursor::ResizeEW] = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
		m_cursors[(int)MouseCursor::ResizeNESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
		m_cursors[(int)MouseCursor::ResizeNWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
		m_cursors[(int)MouseCursor::ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
		m_cursors[(int)MouseCursor::NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
		glfwSetErrorCallback(prev_error_callback);
	}

	void WindowsInput::ShutdownImpl()
	{
		GE_PROFILE_FUNCTION();

		// Destroy GLFW cursors
		for (int cursor_n = 0; cursor_n < (int)MouseCursor::Count; cursor_n++)
			glfwDestroyCursor(m_cursors[cursor_n]);
	}

	glm::vec2 WindowsInput::GetMousePositionImpl()
	{
		GE_PROFILE_FUNCTION();

		double x, y;
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	bool WindowsInput::GetMouseDownImpl(MouseButton button) 
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, (int)button) == GLFW_PRESS;
	}

	bool WindowsInput::GetKeyDownImpl(KeyCode keycode) 
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, (int)keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	void WindowsInput::SetMouseCursorImpl(MouseCursor cursor)
	{
		GE_PROFILE_FUNCTION();

		if (m_isCursorLocked)
			return;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		if (cursor == MouseCursor::None) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else {
			glfwSetCursor(window, m_cursors[(int)cursor] ? m_cursors[(int)cursor] : m_cursors[(int)MouseCursor::Arrow]);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	const char* WindowsInput::GetClipBoardTextImpl()
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetClipboardString(window);
	}

	void WindowsInput::SetClipBoardTextImpl(const char* text)
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetClipboardString(window, text);
	}

	void WindowsInput::LockMouseCursorImpl()
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_isCursorLocked = true;
	}

	void WindowsInput::UnlockMouseCursorImpl()
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_isCursorLocked = false;
	}

}
