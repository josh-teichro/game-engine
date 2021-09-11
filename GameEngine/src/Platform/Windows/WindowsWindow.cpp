
#include "gepch.h"
#include "WindowsWindow.h"
#include "WindowsInput.h"

#include "GameEngine/Events/MouseEvent.h"
#include "GameEngine/Events/KeyEvent.h"
#include "GameEngine/Events/ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

namespace GameEngine {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		GE_CORE_ERROR("GLFW error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props) {
		Window* window =  new WindowsWindow(props);
		Input::s_instance = new WindowsInput();
		return window;
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

		GE_CORE_INFO("Creating window: {0} ({1}x{2})", m_data.title, m_data.width, m_data.height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();

			GE_CORE_ASSERT(success, "Could not initialize GLFW!");
			s_GLFWInitialized = true;
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), nullptr, nullptr);
		m_context = new OpenGLContext(m_window);
		m_context->Init();

		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);

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
					KeyDownEvent e((KeyCode)key, 0);
					data.eventCallbackFn(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyUpEvent e((KeyCode)key);
					data.eventCallbackFn(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyDownEvent e((KeyCode)key, 1);
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

			double x, y;
			glfwGetCursorPos(window, &x, &y);

			switch (action) 
			{
				case GLFW_PRESS:
				{
					MouseDownEvent e((MouseButton)button, (float)x, (float)y);
					data.eventCallbackFn(e);
				}
				case GLFW_RELEASE:
				{
					MouseUpEvent e((MouseButton)button, (float)x, (float)y);
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
		m_context->SwapBuffers();
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

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_window;
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_window);
	}

}
