#include "gepch.h"

#include "Application.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace GameEngine {

	/**
	* Create and initialize an Application.
	*/
	Application::Application() {
		Log::Init();
		m_window = std::unique_ptr<Window>(Window::Create());
	}

	/**
	* Destroy this Applcation.
	*/
	Application::~Application() {
	}

	/**
	* Run the Application until window is closed.
	*/
	void Application::Run() {
		GE_CORE_INFO("Starting Game Engine...");

		while (m_isRunning) {
			// Set color to magenta for now
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_layerStack)
				layer->OnUpdate();

			m_window->OnUpdate();
		}
	}

	/**
	* Called on event.
	*/
	void Application::OnEvent()
	{
		GE_CORE_INFO("Application::OnEvent");

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->OnEvent();

			// if handled, break
		}
	}

	/**
	* Push a layer onto the stack to be rendered.
	*/
	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
	}

	/**
	* Push an overlay onto the stack to be rendered. (Overlays
	* are rendered after normal Layers).
	*/
	void Application::PushOverlay(Layer* overlay)
	{
		m_layerStack.PushOverlay(overlay);
	}

}
