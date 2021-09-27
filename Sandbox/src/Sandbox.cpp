#include "Sandbox.h"
#include "imgui.h"

/**
* Sandbox.
*/
Sandbox::Sandbox()
{
	PushLayer(new ExampleLayer());
}

Sandbox::~Sandbox()
{
}

GameEngine::Application* GameEngine::CreateApplication() {
	return new Sandbox();
}

/**
* Example Layer
*/
inline ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	CreateScene();
	ResetScene();

	m_moveCamera = false;
	m_prevMousePos = { GameEngine::Input::GetMousePosition().x, GameEngine::Input::GetMousePosition().y };
	//GameEngine::Input::LockMouseCursor();
}

inline void ExampleLayer::OnUpdate()
{
	float deltaTime = GameEngine::Time::GetDeltaTime();
	//static bool show = true;
	//ImGui::ShowDemoWindow(&show);

	// move camera
	if (m_moveCamera) {
		if (GameEngine::Input::GetKeyDown(GameEngine::KeyCode::W)) {
			m_camera->GetTransform().position += deltaTime * m_camera->GetTransform().Forward() * m_cameraSpeed;
		}
		else if (GameEngine::Input::GetKeyDown(GameEngine::KeyCode::S)) {
			m_camera->GetTransform().position -= deltaTime * m_camera->GetTransform().Forward() * m_cameraSpeed;
		}

		if (GameEngine::Input::GetKeyDown(GameEngine::KeyCode::A)) {
			m_camera->GetTransform().position -= deltaTime * m_camera->GetTransform().Right() * m_cameraSpeed;
		}
		else if (GameEngine::Input::GetKeyDown(GameEngine::KeyCode::D)) {
			m_camera->GetTransform().position += deltaTime * m_camera->GetTransform().Right() * m_cameraSpeed;
		}

		if (GameEngine::Input::GetKeyDown(GameEngine::KeyCode::LeftShift)) {
			m_camera->GetTransform().position -= deltaTime * m_camera->GetTransform().Up() * m_cameraSpeed;
		}
		else if (GameEngine::Input::GetKeyDown(GameEngine::KeyCode::Space)) {
			m_camera->GetTransform().position += deltaTime * m_camera->GetTransform().Up() * m_cameraSpeed;
		}

		auto newMousePos = GameEngine::Input::GetMousePosition();
		float dx = newMousePos.x - m_prevMousePos.x;
		float dy = newMousePos.y - m_prevMousePos.y;
		m_camRotation.y -= (m_invertCameraX ? -1.0f : 1.0f) * deltaTime * dx * m_cameraRotationSpeed;
		m_camRotation.x -= (m_invertCameraY ? -1.0f : 1.0f) * deltaTime * dy * m_cameraRotationSpeed;
		m_prevMousePos = { newMousePos.x, newMousePos.y };
	}

	// ---
	if (m_lookAtObject) {
		m_camera->LookAt({ 0.0f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f });
		m_camRotation = m_camera->GetTransform().GetEulerAngles();
	}
	else {
		m_camera->GetTransform().SetEulerAngles(m_camRotation);
	}

	// draw scene
	GameEngine::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	GameEngine::Renderer::Clear();

	GameEngine::Renderer::BeginScene(m_camera);
	if (m_camera->GetTransform().position.y > 0.0f) {
		GameEngine::Renderer::Submit(m_vertexArray3, m_objectTransform3, m_shader3);
		GameEngine::Renderer::Submit(m_vertexArray, m_objectTransform, m_shader);
	}
	else {
		GameEngine::Renderer::Submit(m_vertexArray, m_objectTransform, m_shader);
		GameEngine::Renderer::Submit(m_vertexArray3, m_objectTransform3, m_shader3);
	}
	GameEngine::Renderer::EndScene();
}

void ExampleLayer::OnImGuiUpdate()
{
	ImGui::SliderFloat3("Object Position", &m_objectTransform.position[0], -10.0f, 10.0f);
	ImGui::SliderFloat3("Camera Position", &m_camera->GetTransform().position[0], -10.0f, 10.0f);
	ImGui::SliderFloat3("Camera Rotation", &m_camRotation[0], -180.0f, 180.0f);
	ImGui::Checkbox("Lock Onto Object", &m_lookAtObject);
	ImGui::Checkbox("Invert Camera X", &m_invertCameraX);
	ImGui::Checkbox("Invert Camera Y", &m_invertCameraY);

	ImGui::BeginDisabled(m_moveCamera);
	if (ImGui::Button("Move Camera"))
	{
		GameEngine::Input::LockMouseCursor();
		m_moveCamera = true;
		m_prevMousePos = GameEngine::Input::GetMousePosition();
	}
	ImGui::EndDisabled();

	if (ImGui::Button(m_isOrthographic ? "Perspective View" : "Orthographic View")) 
	{
		m_isOrthographic = !m_isOrthographic;
		GameEngine::Transform tempTransform = m_camera->GetTransform();
		GameEngine::Application& app = GameEngine::Application::Get();
		float aspect = (float)app.GetWindow().GetWidth() / app.GetWindow().GetHeight();

		if (m_isOrthographic) {
			m_camera = GameEngine::MakeRef<GameEngine::OrthographicCamera>(-2.0f, 2.0f, -2.0f / aspect, 2.0f / aspect);
		}
		else {
			m_camera = GameEngine::MakeRef<GameEngine::PerspectiveCamera>(45.0f, aspect);
		}

		m_camera->GetTransform() = tempTransform;
	}

	if (ImGui::Button("Reset"))
	{
		ResetScene();
	}
}

bool ExampleLayer::OnMouseDown(const GameEngine::MouseDownEvent& e)
{
	//GE_INFO("ExampleLayer::OnMouseDown {}", e);
	return false;
}

bool ExampleLayer::OnKeyUp(const GameEngine::KeyUpEvent& e)
{
	if (e.keyCode == GameEngine::KeyCode::Escape) 
	{
		GameEngine::Input::UnlockMouseCursor();
		m_moveCamera = false;
	}
	else if (e.keyCode == GameEngine::KeyCode::M)
	{
		GameEngine::Input::LockMouseCursor();
		m_moveCamera = true;
	}

	return false;
}

bool ExampleLayer::OnWindowResize(const GameEngine::WindowResizeEvent& e)
{
	GameEngine::Application& app = GameEngine::Application::Get();
	float aspect = (float)app.GetWindow().GetWidth() / app.GetWindow().GetHeight();

	if (m_isOrthographic) {
		std::dynamic_pointer_cast<GameEngine::OrthographicCamera>(m_camera)->SetBounds(-2.0f, 2.0f, -2.0f / aspect, 2.0f / aspect);
	}
	else {
		std::dynamic_pointer_cast<GameEngine::PerspectiveCamera>(m_camera)->SetAspect(aspect);
	}

	return false;
}

void ExampleLayer::CreateScene()
{
	// set camera
	GameEngine::Application& app = GameEngine::Application::Get();
	float aspect = (float)app.GetWindow().GetWidth() / app.GetWindow().GetHeight();

	if (m_isOrthographic) {
		m_camera = GameEngine::MakeRef<GameEngine::OrthographicCamera>(-2.0f, 2.0f, -2.0f / aspect, 2.0f / aspect);
	}
	else {
		m_camera = GameEngine::MakeRef<GameEngine::PerspectiveCamera>(45.0f, aspect);
	}

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

	m_shader = GameEngine::Shader::Create("./res/shaders/texture.shader");
	m_texture = GameEngine::Texture2D::Create("./res/textures/logo.png");
	m_texture->Bind(0);
	m_shader->SetUniform1i("u_texture", 0);

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

	m_shader3 = GameEngine::Shader::Create("./res/shaders/basic.shader");
}

void ExampleLayer::ResetScene()
{
	// reset camera
	m_camera->GetTransform().position = glm::vec3(0.0f, 0.5f, 2.0f);
	m_camera->LookAt({ 0.0f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	m_camRotation = m_camera->GetTransform().GetEulerAngles();

	// reset objects
	m_objectTransform.Reset();
	m_objectTransform3.Reset();
}
