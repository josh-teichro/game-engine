#pragma once

#include "GameEngine/Core/Core.h"
#include "GameEngine/Core/Window.h"
#include "GameEngine/Core/Time.h"
#include "GameEngine/Core/LayerStack.h"
#include "GameEngine/Events/Event.h"
#include "GameEngine/Events/EventManager.h"
#include "GameEngine/ImGui/ImGuiLayer.h"

namespace GameEngine {

	/**
	* The main Application.
	*/
	class Application// : public HandlesEvents
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void Close();

		void EventCallback(const Event& e);

		bool OnWindowClose(const WindowCloseEvent& e);
		bool OnWindowResize(const WindowResizeEvent& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_window; }

		inline static Application& Get() { return *s_instance; }

	private:
		static Application* s_instance;

		Scope<Window> m_window;
		Scope<Time> m_time;
		bool m_isRunning = true;
		bool m_isMinimized = false;

		LayerStack m_layerStack;

		ImGuiLayer* m_imGuiLayer;

	};

	/**
	* To be defined in Client.
	*
	* Must return an Application object.
	*/
	Application* CreateApplication();

}

