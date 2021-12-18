#include "ExampleLayer.h"

#include <imgui.h>

/**
* Example Layer
*/
ExampleLayer::ExampleLayer() :
	Layer("Example"),
	m_cameraController2D(1.0f),
	m_cameraController3D(45.0f)
{
	GE_PROFILE_FUNCTION();

	CreateScene();
	ResetScene();
}

void ExampleLayer::OnUpdate()
{
	GE_PROFILE_FUNCTION();

	float deltaTime = GameEngine::Time::GetDeltaTime();
	//static bool show = true;
	//ImGui::ShowDemoWindow(&show);

	if (m_is2D)
		m_cameraController2D.OnUpdate();
	else
		m_cameraController3D.OnUpdate();

	if (!m_is2D && m_lookAtObject) {
		m_cameraController3D.LookAt({ 0.0f, 0.5f, 0.0f });
	}

	// draw scene
	GameEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	GameEngine::RenderCommand::Clear();

	if (m_is2D)
		GameEngine::Renderer::BeginScene(m_cameraController2D.GetCamera());
	else
		GameEngine::Renderer::BeginScene(m_cameraController3D.GetCamera());


	auto basicShader = m_shaderLibrary.Get("basic");
	auto textureShader = m_shaderLibrary.Get("texture");

	if (!m_is2D && (m_cameraController3D.GetPosition().y > 0.0f)) {
		GameEngine::Renderer::Submit(m_vertexArray3, m_objectTransform3, basicShader);
		GameEngine::Renderer::Submit(m_vertexArray, m_objectTransform, textureShader);
	}
	else {
		GameEngine::Renderer::Submit(m_vertexArray, m_objectTransform, textureShader);
		GameEngine::Renderer::Submit(m_vertexArray3, m_objectTransform3, basicShader);
	}
	GameEngine::Renderer::EndScene();
}

void ExampleLayer::OnImGuiUpdate()
{
	GE_PROFILE_FUNCTION();

	if (ImGui::Button(m_is2D ? "Switch to 3D" : "Switch to 2D"))
		m_is2D = !m_is2D;

	ImGui::SliderFloat3("Object Position", &m_objectTransform.position[0], -10.0f, 10.0f);

	if (m_is2D)
	{
		ImGui::BeginDisabled(m_walk);
		if (ImGui::Button("Walk"))
		{
			m_walk = true;
			m_cameraController2D.SetMode(GameEngine::CameraController2D::Mode::Walk);
		}
		ImGui::EndDisabled();
	}
	else
	{
		ImGui::Checkbox("Lock Onto Object", &m_lookAtObject);
		ImGui::Checkbox("Invert Camera X", &m_invertCameraX);
		ImGui::Checkbox("Invert Camera Y", &m_invertCameraY);

		m_cameraController3D.SetInvertCameraX(m_invertCameraX);
		m_cameraController3D.SetInvertCameraY(m_invertCameraY);

		ImGui::BeginDisabled(m_fly);
		if (ImGui::Button("Fly"))
		{
			m_fly = true;
			m_cameraController3D.SetMode(GameEngine::CameraController3D::Mode::Fly);
		}
		ImGui::EndDisabled();

		if (ImGui::Button(m_isOrthographic ? "Perspective View" : "Orthographic View"))
		{
			m_isOrthographic = !m_isOrthographic;
			m_cameraController3D.SetIsOrthographic(m_isOrthographic);
		}
	}

	if (ImGui::Button("Reset"))
	{
		ResetScene();
	}
}

bool ExampleLayer::OnEvent(const GameEngine::Event& e)
{
	GE_PROFILE_FUNCTION();

	bool result = false;

	if (m_is2D)
		result = m_cameraController2D.OnEvent(e);
	else
		result = m_cameraController3D.OnEvent(e);

	result = result || HandlesEvents::OnEvent(e);
	return result;
}

bool ExampleLayer::OnMouseDown(const GameEngine::MouseDownEvent& e)
{
	//GE_INFO("ExampleLayer::OnMouseDown {}", e);
	return false;
}

bool ExampleLayer::OnKeyUp(const GameEngine::KeyUpEvent& e)
{
	if (m_is2D)
	{
		if (e.keyCode == GameEngine::KeyCode::Escape)
		{
			m_walk = false;
			m_cameraController2D.SetMode(GameEngine::CameraController2D::Mode::Normal);
		}
		else if (e.keyCode == GameEngine::KeyCode::W)
		{
			m_walk = true;
			m_cameraController2D.SetMode(GameEngine::CameraController2D::Mode::Walk);
		}
	}
	else
	{
		if (e.keyCode == GameEngine::KeyCode::Escape)
		{
			m_fly = false;
			m_cameraController3D.SetMode(GameEngine::CameraController3D::Mode::Normal);
		}
		else if (e.keyCode == GameEngine::KeyCode::F)
		{
			m_fly = true;
			m_cameraController3D.SetMode(GameEngine::CameraController3D::Mode::Fly);
		}
	} 

	return false;
}

bool ExampleLayer::OnWindowResize(const GameEngine::WindowResizeEvent& e)
{
	return false;
}

void ExampleLayer::CreateScene()
{
	GE_PROFILE_FUNCTION();

	// object 1
	m_vertexArray = GameEngine::VertexArray::Create();

	float vertices[(3 + 2) * 4] = {
		-0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 1.0, 0.0f, 0.0f, 1.0f,
		0.5, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5, 1.0, 0.0f, 1.0f, 1.0f
	};

	GameEngine::VertexBufferLayout layout = {
		{ GameEngine::ShaderDataType::Vec3, "a_position" },
		{ GameEngine::ShaderDataType::Vec2, "a_texCoord" }
	};

	GameEngine::Ref<GameEngine::VertexBuffer> vertexBuffer = GameEngine::VertexBuffer::Create(vertices, sizeof(vertices), layout);

	uint32_t indices[6] = { 0, 1, 2, 3, 1, 2 };
	GameEngine::Ref<GameEngine::IndexBuffer> indexBuffer = GameEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

	m_vertexArray->AddVertexBuffer(vertexBuffer);
	m_vertexArray->SetIndexBuffer(indexBuffer);

	auto textureShader = m_shaderLibrary.Load("./res/shaders/texture.shader");
	m_texture = GameEngine::Texture2D::Create("./res/textures/logo.png");
	m_texture->Bind(0);
	textureShader->SetUniform1i("u_texture", 0);

	// object 3
	m_vertexArray3 = GameEngine::VertexArray::Create();

	float vertices3[(3 + 4) * 4] = {
		-0.5f, 0.0f, -0.5f, 0.4f, 0.4f, 0.4f, 1.0f,
		-0.5f,  0.0f, 0.5f, 0.4f, 0.4f, 0.4f, 1.0f,
		0.5f, 0.0f, -0.5f, 0.4f, 0.4f, 0.4f, 1.0f,
		0.5f, 0.0f, 0.5f, 0.4f, 0.4f, 0.4f, 1.0f
	};

	GameEngine::VertexBufferLayout layout3 = {
		{ GameEngine::ShaderDataType::Vec3, "a_position" },
		{ GameEngine::ShaderDataType::Vec4, "a_color" }
	};

	GameEngine::Ref<GameEngine::VertexBuffer> vertexBuffer3 = GameEngine::VertexBuffer::Create(vertices3, sizeof(vertices3), layout3);

	uint32_t indices3[6] = { 0, 1, 2, 3, 1, 2 };
	GameEngine::Ref<GameEngine::IndexBuffer> indexBuffer3 = GameEngine::IndexBuffer::Create(indices3, sizeof(indices3) / sizeof(uint32_t));

	m_vertexArray3->AddVertexBuffer(vertexBuffer3);
	m_vertexArray3->SetIndexBuffer(indexBuffer3);

	m_shaderLibrary.Load("./res/shaders/basic.shader");
}

void ExampleLayer::ResetScene()
{
	GE_PROFILE_FUNCTION();

	// reset camera
	m_cameraController2D.SetZoom(1.0f);
	m_cameraController2D.SetPosition({ 0.0f, 0.5f });
	m_cameraController3D.SetFOV(45.0f);
	m_cameraController3D.SetPosition({ 0.0f, 0.5f, 2.0f });
	m_cameraController3D.LookAt({ 0.0f, 0.5f, 0.0f });

	// reset objects
	m_objectTransform.Reset();
	m_objectTransform3.Reset();
}
