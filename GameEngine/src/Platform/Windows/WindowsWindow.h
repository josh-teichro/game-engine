#pragma once

#include "GameEngine/Window.h"

#include <GLFW/glfw3.h>

namespace GameEngine {

	/**
	* Windows Window.
	*/
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() override { return m_data.width; };
		inline unsigned int GetHeight() override { return m_data.height; };

		inline void SetEventCallback(const EventCallbackFn& callback) override;
		void SetVSync(bool enabled) override;
		bool IsVSync() override;

	protected:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_window;

		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallbackFn;
		};

		struct WindowData m_data;
	};

}
