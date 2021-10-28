#include "gepch.h"

#include "GameEngine/Core/Application.h"
#include "GameEngine/Renderer/Renderer.h"
#include "GameEngine/Renderer/Renderer2D.h"
#include "GameEngine/Renderer/VertexBuffer.h"
#include "GameEngine/Core/Log.h"
#include "GameEngine/Core/Input.h"

namespace GameEngine {

	Application* Application::s_instance = nullptr;

	/**
	* Create and initialize an Application.
	*/
	Application::Application() 
	{
		GE_PROFILE_FUNCTION();

		GE_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = Window::Create();
		m_window->SetEventCallback(std::bind(&Application::EventCallback, this, std::placeholders::_1));

		Input::Init();

		// TODO: automate some of this so we dont have to manually init every Renderer class
		RenderCommand::Init();
		Renderer::Init();
		Renderer2D::Init();

		m_time = MakeScope<Time>();

		m_imGuiLayer = new ImGuiLayer();
		PushOverlay(m_imGuiLayer);
	}

	Application::~Application()
	{
		GE_PROFILE_FUNCTION();

		Input::Shutdown();

		Renderer::Shutdown();
		Renderer2D::Shutdown();
	}

	/**
	* Run the Application until window is closed.
	*/
	void Application::Run() 
	{
		GE_PROFILE_FUNCTION();

		GE_CORE_INFO("Starting Game Engine...");

		while (m_isRunning) {
			GE_PROFILE_SCOPE("RunLoop");

			// Update
			m_time->OnUpdate();

			{
				GE_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_layerStack)
					layer->OnUpdate();
			}

			// ImGui update
			m_imGuiLayer->BeginFrame();
			{
				GE_PROFILE_SCOPE("LayerStack OnImguiUpdate");

				for (Layer* layer : m_layerStack) 
					layer->OnImGuiUpdate();
				m_imGuiLayer->EndFrame();
			}

			m_window->OnUpdate();
		}
	}

	/**
	* Called on event.
	*/
	void Application::EventCallback(const Event& e)
	{
		GE_PROFILE_FUNCTION();

		bool handled = false;
		EventDispatcher dispatcher(e);
		handled &= dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(&Application::OnWindowClose));
		handled &= dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(&Application::OnWindowResize));

		if (handled)
			return;

		for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it++)
		{
			if ((*it)->OnEvent(e))
				break;
		}
	}

	bool Application::OnWindowClose(const WindowCloseEvent& e)
	{
		m_isRunning = false;
		return true;
	}

	bool Application::OnWindowResize(const WindowResizeEvent& e)
	{
		if (e.width == 0 || e.height == 0)
		{ 
			m_isMinimized = true;
			return false;
		}

		m_isMinimized = false;
		RenderCommand::SetViewport(0, 0, e.width, e.height);
		return false;
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
