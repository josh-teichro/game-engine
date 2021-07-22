#pragma once

#include "gepch.h"
#include "GameEngine/Core.h"

namespace GameEngine {

	struct WindowProps {
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Game Engine", 
					unsigned int width = 640, 
					unsigned int height = 360) :
			title(title), width(width), height(height) {}
	};

	class GE_API Window {
	public:
		using Event = int; // TODO: change to actual event class
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;

		virtual void SetEventCallback(EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() = 0;

		// to be defined in platform specific files
		static Window* Create(const WindowProps& props = WindowProps());
	};

}
