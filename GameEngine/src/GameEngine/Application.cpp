#include "gepch.h"

#include "Application.h"
#include "Log.h"
#include "ImGui/ImGuiLayer.h"

// TODO: remove
#include <glad/glad.h>

namespace GameEngine {

	Application* Application::s_instance = nullptr;

	/**
	* Create and initialize an Application.
	*/
	Application::Application() {
		GE_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		Log::Init();
		m_window = std::unique_ptr<Window>(Window::Create());
		//auto fn = std::bind(&Application::OnEvent, this, std::placeholders::_1);
		m_window->SetEventCallback(std::bind(&Application::EventCallback, this, std::placeholders::_1));
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

		PushOverlay(new ImGuiLayer());

		while (m_isRunning) {
			// Set color to magenta for now
			// TODO: remove
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
	void Application::EventCallback(const Event& e)
	{
		GE_CORE_INFO("Application::OnEvent - {}", e);

		if (EventDispatcher(e).Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1)))
			return;

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			if ((*--it)->OnEvent(e))
				break;
		}
	}

	bool Application::OnWindowClose(const WindowCloseEvent& e)
	{
		m_isRunning = false;
		return true;
	}

	/**
	* Push a layer onto the stack to be rendered.
	*/
	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	/**
	* Push an overlay onto the stack to be rendered. (Overlays
	* are rendered after normal Layers).
	*/
	void Application::PushOverlay(Layer* overlay)
	{
		m_layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

}
