#pragma once

#include "Core.h"
#include "Window.h"

namespace GameEngine {

	class GE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_window;
	};

	// To be defined in Client
	Application* CreateApplication();
	bool m_isRunning = true;

}

