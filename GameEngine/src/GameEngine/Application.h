#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

namespace GameEngine {

	/**
	* The main Application.
	*/
	class GE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		std::unique_ptr<Window> m_window;
		bool m_isRunning = true;

		LayerStack m_layerStack;

	};

	/**
	* To be defined in Client.
	*
	* Must return an Application object.
	*/
	Application* CreateApplication();

}

