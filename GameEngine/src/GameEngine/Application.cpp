#include "gepch.h"

#include "Application.h"
#include "Log.h"

namespace GameEngine {

	Application::Application() {
		Log::Init();
		m_window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application() {
	}

	void Application::Run() {
		GE_CORE_INFO("Starting Game Engine...");

		while (m_isRunning) {
			m_window->OnUpdate();
		}
	}

}
