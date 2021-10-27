
#include "gepch.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Windows/WindowsInput.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include "GameEngine/Events/MouseEvent.h"
#include "GameEngine/Events/KeyEvent.h"
#include "GameEngine/Events/ApplicationEvent.h"
#include "GameEngine/Renderer/Renderer.h"

#include <glad/glad.h>

namespace GameEngine {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		GE_CORE_ERROR("GLFW error ({0}): {1}", error, description);
	}

	Scope<Window> Window::Create(const WindowProps& props) {
		return MakeScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
		WindowsInput::Init();
	}

	WindowsWindow::~WindowsWindow()
	{
		WindowsInput::Shutdown();
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		GE_PROFILE_FUNCTION();

		m_data.title = props.title;
		m_data.width = props.width; 
		m_data.height = props.height;

		if (s_GLFWWindowCount == 0) {
			GE_PROFILE_SCOPE("glfwInit");

			GE_CORE_INFO("Initializing GLFW");
			int success = glfwInit();

			GE_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			GE_PROFILE_SCOPE("glfwCreateWindow");

#ifdef GE_DEBUG
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

			m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), nullptr, nullptr);
			s_GLFWWindowCount++;
		}

		m_context = MakeScope<OpenGLContext>(m_window);
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
		GE_PROFILE_FUNCTION();

		glfwPollEvents();
		m_context->SwapBuffers();
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		m_data.eventCallbackFn = callback;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		GE_PROFILE_FUNCTION();

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
		GE_PROFILE_FUNCTION();

		glfwDestroyWindow(m_window);
		s_GLFWWindowCount--;

		if (s_GLFWWindowCount == 0)
		{
			GE_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}

}
