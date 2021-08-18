#pragma once

#include "GameEngine/Window.h"
#include "GameEngine/Events/KeyCodes.h"

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
		CursorPos GetCursorPos() override;
		bool GetMouseButton(MouseButton button) override;
		void SetMouseCursor(MouseCursor cursor) override;
		const char* GetClipboardText() override;
		void SetClipboardText(const char* text) override;

		inline void SetEventCallback(const EventCallbackFn& callback) override;
		void SetVSync(bool enabled) override;
		bool IsVSync() override;

	protected:
		void Init(const WindowProps& props);
		void Shutdown();

		KeyCode MapKeyCode(int keycode);

	private:
		GLFWwindow* m_window;

		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallbackFn;
			std::function<KeyCode(int keycode)> mapKeyCode;
		};

		struct WindowData m_data;

		GLFWcursor* m_cursors[MouseCursor_Count];
	};

}
