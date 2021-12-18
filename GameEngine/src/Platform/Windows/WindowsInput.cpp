#include "gepch.h"

#include "GameEngine/Core/Input.h"
#include "GameEngine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace GameEngine {

	struct InputData {
		GLFWcursor* cursors[(int)MouseCursor::Count];
		bool isCursorLocked;
	};

	static InputData s_data;

	void Input::Init()
	{
		GE_PROFILE_FUNCTION();

		s_data.isCursorLocked = false;

		// GLFW cursors
		// (By design, on X11 cursors are user configurable and some cursors may be missing. When a cursor doesn't exist,
		// GLFW will emit an error which will often be printed by the app, so we temporarily disable error reporting.
		// Missing cursors will return NULL and our SetMouseCursor() function will use the Arrow cursor instead.)
		GLFWerrorfun prev_error_callback = glfwSetErrorCallback(NULL);
		s_data.cursors[(int)MouseCursor::Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		s_data.cursors[(int)MouseCursor::Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
		s_data.cursors[(int)MouseCursor::TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		s_data.cursors[(int)MouseCursor::ResizeNS] = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
		s_data.cursors[(int)MouseCursor::ResizeEW] = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
		s_data.cursors[(int)MouseCursor::ResizeNESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
		s_data.cursors[(int)MouseCursor::ResizeNWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
		s_data.cursors[(int)MouseCursor::ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
		s_data.cursors[(int)MouseCursor::NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
		glfwSetErrorCallback(prev_error_callback);
	}

	void Input::Shutdown()
	{
		GE_PROFILE_FUNCTION();

		// Destroy GLFW cursors
		for (int cursor_n = 0; cursor_n < (int)MouseCursor::Count; cursor_n++)
			glfwDestroyCursor(s_data.cursors[cursor_n]);
	}

	glm::vec2 Input::GetMousePosition()
	{
		GE_PROFILE_FUNCTION();

		double x, y;
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	bool Input::GetMouseDown(MouseButton button) 
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, (int)button) == GLFW_PRESS;
	}

	bool Input::GetKeyDown(KeyCode keycode) 
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, (int)keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	void Input::SetMouseCursor(MouseCursor cursor)
	{
		GE_PROFILE_FUNCTION();

		if (s_data.isCursorLocked)
			return;

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		if (cursor == MouseCursor::None) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else {
			glfwSetCursor(window, s_data.cursors[(int)cursor] ? s_data.cursors[(int)cursor] : s_data.cursors[(int)MouseCursor::Arrow]);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	const char* Input::GetClipBoardText()
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetClipboardString(window);
	}

	void Input::SetClipBoardText(const char* text)
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetClipboardString(window, text);
	}

	void Input::LockMouseCursor()
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		s_data.isCursorLocked = true;
	}

	void Input::UnlockMouseCursor()
	{
		GE_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		s_data.isCursorLocked = false;
	}

}
