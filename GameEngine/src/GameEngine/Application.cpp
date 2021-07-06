#include "Application.h"
#include "Log.h"
#include "Events/KeyEvent.h"
#include "Events/Event.h"

namespace GameEngine {

	Application::Application() {
	}

	Application::~Application() {
	}

	void Application::Run() {
		Log::Init();
		GE_CORE_INFO("Starting Game Engine...");

		KeyPressEvent e(0, 0);
		EventDispatcher dispatcher(e);

		std::vector<std::function<bool(KeyEvent&)>> callbacks;

		callbacks.push_back([](KeyEvent& e) {GE_INFO("Key press: {}", e.ToString()); return false; });

		for (int i = 0; i < callbacks.size(); i++) {
			dispatcher.Dispatch<KeyEvent>(callbacks[i]);
		}

		while (true) {}
	}

	/*void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}*/

}
