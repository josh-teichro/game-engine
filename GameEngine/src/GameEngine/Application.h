#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace GameEngine {

	class GE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	};

	// To be defined in Client
	Application* CreateApplication();

}

