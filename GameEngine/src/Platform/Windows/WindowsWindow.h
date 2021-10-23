#pragma once

#include "GameEngine/Core/Window.h"
#include "GameEngine/Events/KeyCodes.h"
#include "GameEngine/Renderer/RenderContext.h"
#include "WindowsInput.h"

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

		virtual void OnUpdate() override;

		virtual inline unsigned int GetWidth() override { return m_data.width; };
		virtual inline unsigned int GetHeight() override { return m_data.height; };

		virtual inline void SetEventCallback(const EventCallbackFn& callback) override;
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() override;

		virtual void* GetNativeWindow() const override;

	protected:
		void Init(const WindowProps& props);
		void Shutdown();

	private:
		GLFWwindow* m_window;
		Scope<RenderContext> m_context;

		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallbackFn;
		};

		struct WindowData m_data;
	};

}
