#include "Application.h"
#include "Log.h"

namespace GameEngine {

	Application::Application() {
	}

	Application::~Application() {
	}

	void Application::Run() {
		Log::Init();
		GE_CORE_INFO("Starting Game Engine...");

		while (true) {}
	}

}
