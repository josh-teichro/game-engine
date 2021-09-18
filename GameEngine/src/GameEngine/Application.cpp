#include "gepch.h"

#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexBuffer.h"
#include "Log.h"

// TODO: move to Sandbox
#include "imgui.h"

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
		m_window->SetEventCallback(std::bind(&Application::EventCallback, this, std::placeholders::_1));

		m_imGuiLayer = new ImGuiLayer();
		PushOverlay(m_imGuiLayer);

		// create scene
		// 1
		m_vertexArray = VertexArray::Create();

		float vertices[3 * 3 + 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.2f, 1.0f,
			 0.45f, -0.5f, 0.0f, 0.2f, 0.8f, 0.1f, 1.0f,
			 -0.5f,  0.45f, 0.0f, 0.0f, 0.4f, 0.7f, 1.0f
		};

		VertexBufferLayout layout = {
			{ ShaderDataType::Vec3, "a_position" },
			{ ShaderDataType::Vec4, "a_color" }
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices), layout);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(indexBuffer);


		m_shader = Shader::Create("../GameEngine/res/shaders/red.shader");

		// 2
		m_vertexArray2 = VertexArray::Create();

		float vertices2[3 * 3] = {
			0.5f, 0.5f, 0.0f,
			-0.45f,  0.5f, 0.0f,
			0.5f, -0.45f, 0.0f
		};

		VertexBufferLayout layout2 = {
			{ ShaderDataType::Vec3, "a_position" }
		};

		std::shared_ptr<VertexBuffer> vertexBuffer2 = VertexBuffer::Create(vertices2, sizeof(vertices2), layout2);

		uint32_t indices2[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer2 = IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t));

		m_vertexArray2->AddVertexBuffer(vertexBuffer2);
		m_vertexArray2->SetIndexBuffer(indexBuffer2);

		m_shader2 = Shader::Create("../GameEngine/res/shaders/blue.shader");
	}

	/**
	* Run the Application until window is closed.
	*/
	void Application::Run() {
		GE_CORE_INFO("Starting Game Engine...");

		while (m_isRunning) {
			// Update
			for (Layer* layer : m_layerStack)
				layer->OnUpdate();

			// ImGui update
			m_imGuiLayer->BeginFrame();
			for (Layer* layer : m_layerStack) 
				layer->OnImGuiUpdate();

			static bool show = true;
			ImGui::ShowDemoWindow(&show);
			m_imGuiLayer->EndFrame();

			// draw scene
			Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Renderer::Clear();

			Renderer::BeginScene();
			Renderer::Submit(m_vertexArray, m_shader);
			Renderer::Submit(m_vertexArray2, m_shader2);
			Renderer::EndScene();

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
