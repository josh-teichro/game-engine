
#include "gepch.h"
#include "WindowsWindow.h"

namespace GameEngine {

	static bool s_GLFWInitialized = false;

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

		GE_CORE_INFO("Creating window: {0} ({1}x{2})", m_data.title, m_data.width, m_data.height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();

			GE_CORE_ASSERT(success, "Could not initialize GLFW!");
			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::SetEventCallback(EventCallbackFn& callback)
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
		glfwDestroyWindow(m_window);
	}

}
