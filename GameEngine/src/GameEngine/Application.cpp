#include "gepch.h"

#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexBuffer.h"
#include "Log.h"

namespace GameEngine {

	Application* Application::s_instance = nullptr;

	/**
	* Create and initialize an Application.
	*/
	Application::Application() {
		GE_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		Log::Init();
		m_window = Scope<Window>(Window::Create());
		m_window->SetEventCallback(std::bind(&Application::EventCallback, this, std::placeholders::_1));

		Renderer::Init();

		m_time = MakeScope<Time>();

		m_imGuiLayer = new ImGuiLayer();
		PushOverlay(m_imGuiLayer);
	}

	/**
	* Run the Application until window is closed.
	*/
	void Application::Run() {
		GE_CORE_INFO("Starting Game Engine...");

		while (m_isRunning) {
			// Update
			m_time->OnUpdate();

			for (Layer* layer : m_layerStack)
				layer->OnUpdate();

			// ImGui update
			m_imGuiLayer->BeginFrame();
			for (Layer* layer : m_layerStack) 
				layer->OnImGuiUpdate();
			m_imGuiLayer->EndFrame();

			m_window->OnUpdate();
		}
	}

	/**
	* Called on event.
	*/
	void Application::EventCallback(const Event& e)
	{
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
