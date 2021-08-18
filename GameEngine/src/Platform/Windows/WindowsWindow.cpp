
#include "gepch.h"
#include "WindowsWindow.h"

#include "GameEngine/Events/MouseEvent.h"
#include "GameEngine/Events/KeyEvent.h"
#include "GameEngine/Events/ApplicationEvent.h"

#include <glad/glad.h>

namespace GameEngine {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		GE_CORE_ERROR("GLFW error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_data.title = props.title;
		m_data.width = props.width; 
		m_data.height = props.height;
		m_data.mapKeyCode = std::bind(&WindowsWindow::MapKeyCode, this, std::placeholders::_1);

		GE_CORE_INFO("Creating window: {0} ({1}x{2})", m_data.title, m_data.width, m_data.height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();

			GE_CORE_ASSERT(success, "Could not initialize GLFW!");
			s_GLFWInitialized = true;
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);

		// init Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GE_CORE_ASSERT(status, "Failed to initialize Glad!");

		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);

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

		// GLFW callbacks
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e; // TODO: replace with window close event
			data.eventCallbackFn(e);
		});

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.width = width;
				data.height = height;

				WindowResizeEvent e(width, height); // TODO: replace with window resize event
				data.eventCallbackFn(e);
			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyDownEvent e(data.mapKeyCode(key), 0);
					data.eventCallbackFn(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyUpEvent e(data.mapKeyCode(key));
					data.eventCallbackFn(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyDownEvent e(data.mapKeyCode(key), 1);
					data.eventCallbackFn(e);
					break;
				}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int character) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			CharTypedEvent e(character);

			data.eventCallbackFn(e);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseButton button2 = MouseButton_Unknown;

			switch (button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
				button2 = MouseButton_Left;
			case GLFW_MOUSE_BUTTON_RIGHT:
				button2 = MouseButton_Right;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				button2 = MouseButton_Middle;
			}

			double x, y;
			glfwGetCursorPos(window, &x, &y);

			switch (action) 
			{
				case GLFW_PRESS:
				{
					MouseDownEvent e(button2, (float)x, (float)y);
					data.eventCallbackFn(e);
				}
				case GLFW_RELEASE:
				{
					MouseUpEvent e(button2, (float)x, (float)y);
					data.eventCallbackFn(e);
				}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrollEvent e(0.0f, 0.0f, (float)xOffset, (float)yOffset);
			data.eventCallbackFn(e);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMoveEvent e((float)xPos, (float)yPos);
			data.eventCallbackFn(e);
		});
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	Window::CursorPos WindowsWindow::GetCursorPos()
	{
		double x, y;
		glfwGetCursorPos(m_window, &x, &y);
		return { (float)x, (float)y };
	}

	bool WindowsWindow::GetMouseButton(MouseButton button)
	{
		switch (button)
		{
		case MouseButton_Left:
			return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT);
		case MouseButton_Right:
			return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT);
		case MouseButton_Middle:
			return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE);
		}

		return false;
	}

	void WindowsWindow::SetMouseCursor(MouseCursor cursor)
	{
		if (cursor == MouseCursor_None) {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else {
			glfwSetCursor(m_window, m_cursors[cursor] ? m_cursors[cursor] : m_cursors[MouseCursor_Arrow]);
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	const char* WindowsWindow::GetClipboardText()
	{
		return glfwGetClipboardString(m_window);
	}

	void WindowsWindow::SetClipboardText(const char* text)
	{
		glfwSetClipboardString(m_window, text);
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		m_data.eventCallbackFn = callback;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled) 
			glfwSwapInterval(1);
		else 
			glfwSwapInterval(0);

		m_data.vSync = enabled;
	}

	bool WindowsWindow::IsVSync()
	{
		return m_data.vSync;
	}

	void WindowsWindow::Shutdown()
	{
		// Destroy GLFW cursors
		for (int cursor_n = 0; cursor_n < MouseCursor_Count; cursor_n++)
			glfwDestroyCursor(m_cursors[cursor_n]);

		glfwDestroyWindow(m_window);
	}

	KeyCode WindowsWindow::MapKeyCode(int keycode)
	{
		switch (keycode) {
			case GLFW_KEY_SPACE: return KeyCode::Space;
			case GLFW_KEY_APOSTROPHE: return KeyCode::Apostrophe;
			case GLFW_KEY_COMMA: return KeyCode::Comma;
			case GLFW_KEY_MINUS: return KeyCode::Minus;
			case GLFW_KEY_PERIOD: return KeyCode::Period;
			case GLFW_KEY_SLASH: return KeyCode::Slash;
			case GLFW_KEY_0: return KeyCode::Key0;
			case GLFW_KEY_1: return KeyCode::Key1;
			case GLFW_KEY_2: return KeyCode::Key2;
			case GLFW_KEY_3: return KeyCode::Key3;
			case GLFW_KEY_4: return KeyCode::Key4;
			case GLFW_KEY_5: return KeyCode::Key5;
			case GLFW_KEY_6: return KeyCode::Key6;
			case GLFW_KEY_7: return KeyCode::Key7;
			case GLFW_KEY_8: return KeyCode::Key8;
			case GLFW_KEY_9: return KeyCode::Key9;
			case GLFW_KEY_SEMICOLON: return KeyCode::Semicolon;
			case GLFW_KEY_EQUAL: return KeyCode::Equal;
			case GLFW_KEY_A: return KeyCode::A;
			case GLFW_KEY_B: return KeyCode::B;
			case GLFW_KEY_C: return KeyCode::C;
			case GLFW_KEY_D: return KeyCode::D;
			case GLFW_KEY_E: return KeyCode::E;
			case GLFW_KEY_F: return KeyCode::F;
			case GLFW_KEY_G: return KeyCode::G;
			case GLFW_KEY_H: return KeyCode::H;
			case GLFW_KEY_I: return KeyCode::I;
			case GLFW_KEY_J: return KeyCode::J;
			case GLFW_KEY_K: return KeyCode::K;
			case GLFW_KEY_L: return KeyCode::L;
			case GLFW_KEY_M: return KeyCode::M;
			case GLFW_KEY_N: return KeyCode::N;
			case GLFW_KEY_O: return KeyCode::O;
			case GLFW_KEY_P: return KeyCode::P;
			case GLFW_KEY_Q: return KeyCode::Q;
			case GLFW_KEY_R: return KeyCode::R;
			case GLFW_KEY_S: return KeyCode::S;
			case GLFW_KEY_T: return KeyCode::T;
			case GLFW_KEY_U: return KeyCode::U;
			case GLFW_KEY_V: return KeyCode::V;
			case GLFW_KEY_W: return KeyCode::W;
			case GLFW_KEY_X: return KeyCode::X;
			case GLFW_KEY_Y: return KeyCode::Y;
			case GLFW_KEY_Z: return KeyCode::Z;
			case GLFW_KEY_LEFT_BRACKET: return KeyCode::LeftBracket;
			case GLFW_KEY_RIGHT_BRACKET: return KeyCode::RightBracket;
			case GLFW_KEY_BACKSLASH: return KeyCode::Backslash;
			case GLFW_KEY_GRAVE_ACCENT: return KeyCode::GraveAccent;
			case GLFW_KEY_WORLD_1: return KeyCode::World1;
			case GLFW_KEY_WORLD_2: return KeyCode::World2;
			case GLFW_KEY_ESCAPE: return KeyCode::Escape;
			case GLFW_KEY_ENTER: return KeyCode::Enter;
			case GLFW_KEY_TAB: return KeyCode::Tab;
			case GLFW_KEY_BACKSPACE: return KeyCode::Backspace;
			case GLFW_KEY_INSERT: return KeyCode::Insert;
			case GLFW_KEY_DELETE: return KeyCode::Delete;
			case GLFW_KEY_RIGHT: return KeyCode::RightArrow;
			case GLFW_KEY_LEFT: return KeyCode::LeftArrow;
			case GLFW_KEY_DOWN: return KeyCode::DownArrow;
			case GLFW_KEY_UP: return KeyCode::UpArrow;
			case GLFW_KEY_PAGE_UP: return KeyCode::PageUp;
			case GLFW_KEY_PAGE_DOWN: return KeyCode::PageDown;
			case GLFW_KEY_HOME: return KeyCode::Home;
			case GLFW_KEY_END: return KeyCode::End;
			case GLFW_KEY_CAPS_LOCK: return KeyCode::CapsLock;
			case GLFW_KEY_SCROLL_LOCK: return KeyCode::ScollLock;
			case GLFW_KEY_NUM_LOCK: return KeyCode::NumLock;
			case GLFW_KEY_PRINT_SCREEN: return KeyCode::PrintScreen;
			case GLFW_KEY_PAUSE: return KeyCode::Pause;
			case GLFW_KEY_F1: return KeyCode::F1;
			case GLFW_KEY_F2: return KeyCode::F2;
			case GLFW_KEY_F3: return KeyCode::F3;
			case GLFW_KEY_F4: return KeyCode::F4;
			case GLFW_KEY_F5: return KeyCode::F5;
			case GLFW_KEY_F6: return KeyCode::F6;
			case GLFW_KEY_F7: return KeyCode::F7;
			case GLFW_KEY_F8: return KeyCode::F8;
			case GLFW_KEY_F9: return KeyCode::F9;
			case GLFW_KEY_F10: return KeyCode::F10;
			case GLFW_KEY_F11: return KeyCode::F11;
			case GLFW_KEY_F12: return KeyCode::F12;
			case GLFW_KEY_F13: return KeyCode::F13;
			case GLFW_KEY_F14: return KeyCode::F14;
			case GLFW_KEY_F15: return KeyCode::F15;
			case GLFW_KEY_F16: return KeyCode::F16;
			case GLFW_KEY_F17: return KeyCode::F17;
			case GLFW_KEY_F18: return KeyCode::F18;
			case GLFW_KEY_F19: return KeyCode::F19;
			case GLFW_KEY_F20: return KeyCode::F20;
			case GLFW_KEY_F21: return KeyCode::F21;
			case GLFW_KEY_F22: return KeyCode::F22;
			case GLFW_KEY_F23: return KeyCode::F23;
			case GLFW_KEY_F24: return KeyCode::F24;
			case GLFW_KEY_F25: return KeyCode::F25;
			case GLFW_KEY_KP_0: return KeyCode::KeyPad0;
			case GLFW_KEY_KP_1: return KeyCode::KeyPad1;
			case GLFW_KEY_KP_2: return KeyCode::KeyPad2;
			case GLFW_KEY_KP_3: return KeyCode::KeyPad3;
			case GLFW_KEY_KP_4: return KeyCode::KeyPad4;
			case GLFW_KEY_KP_5: return KeyCode::KeyPad5;
			case GLFW_KEY_KP_6: return KeyCode::KeyPad6;
			case GLFW_KEY_KP_7: return KeyCode::KeyPad7;
			case GLFW_KEY_KP_8: return KeyCode::KeyPad8;
			case GLFW_KEY_KP_9: return KeyCode::KeyPad9;
			case GLFW_KEY_KP_DECIMAL: return KeyCode::KeyPadDecimal;
			case GLFW_KEY_KP_DIVIDE: return KeyCode::KeyPadDivide;
			case GLFW_KEY_KP_MULTIPLY: return KeyCode::KeyPadMultiply;
			case GLFW_KEY_KP_SUBTRACT: return KeyCode::KeyPadSubtract;
			case GLFW_KEY_KP_ADD: return KeyCode::KeyPadAdd;
			case GLFW_KEY_KP_ENTER: return KeyCode::KeyPadEnter;
			case GLFW_KEY_KP_EQUAL: return KeyCode::KeyPadEqual;
			case GLFW_KEY_LEFT_SHIFT: return KeyCode::LeftShift;
			case GLFW_KEY_LEFT_CONTROL: return KeyCode::LeftCtrl;
			case GLFW_KEY_LEFT_ALT: return KeyCode::LeftAlt;
			case GLFW_KEY_LEFT_SUPER: return KeyCode::LeftSuper;
			case GLFW_KEY_RIGHT_SHIFT: return KeyCode::RightShift;
			case GLFW_KEY_RIGHT_CONTROL: return KeyCode::RightCtrl;
			case GLFW_KEY_RIGHT_ALT: return KeyCode::RightAlt;
			case GLFW_KEY_RIGHT_SUPER: return KeyCode::RightSuper;
			case GLFW_KEY_MENU: return KeyCode::Menu;
			default: return KeyCode::Unknown;
		}
	}

}
