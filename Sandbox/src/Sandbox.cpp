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

	m_prevMousePos = { GameEngine::Input::GetMousePosition().x, GameEngine::Input::GetMousePosition().y };
}

inline void ExampleLayer::OnUpdate()
{
	float deltaTime = GameEngine::Time::GetDeltaTime();
	//static bool show = true;
	//ImGui::ShowDemoWindow(&show);

	glm::vec4 pos = { 0.5f, 0.5f, 0.0f, 0.0f };
	glm::vec3 Mpos = glm::mat4(1) * pos;
	glm::vec3 VMpos = m_camera->V() * glm::mat4(1) * pos;
	glm::vec3 PVMpos = m_camera->VP() * glm::mat4(1) * pos;

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
		GameEngine::Renderer::Submit(m_vertexArray3, glm::translate(glm::mat4(1.0f), objectPositon3), m_shader3);
		GameEngine::Renderer::Submit(m_vertexArray, glm::translate(glm::mat4(1.0f), objectPositon), m_shader);
		GameEngine::Renderer::Submit(m_vertexArray2, glm::translate(glm::mat4(1.0f), objectPositon2), m_shader2);
	}
	else {
		GameEngine::Renderer::Submit(m_vertexArray, glm::translate(glm::mat4(1.0f), objectPositon), m_shader);
		GameEngine::Renderer::Submit(m_vertexArray2, glm::translate(glm::mat4(1.0f), objectPositon2), m_shader2);
		GameEngine::Renderer::Submit(m_vertexArray3, glm::translate(glm::mat4(1.0f), objectPositon3), m_shader3);
	}
	GameEngine::Renderer::EndScene();
}

void ExampleLayer::OnImGuiUpdate()
{
	ImGui::SliderFloat3("Object 1 Position", &objectPositon.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Object 2 Position", &objectPositon2.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Camera Position", &m_camera->GetTransform().position.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Camera Rotation", &m_camRotation.x, -180.0f, 180.0f);
	ImGui::Checkbox("Lock Onto Object", &m_lookAtObject);
	ImGui::Checkbox("Invert Camera X", &m_invertCameraX);
	ImGui::Checkbox("Invert Camera Y", &m_invertCameraY);
	ImGui::Checkbox("Move Camera", &m_moveCamera);

	if (ImGui::Button(m_isOrthographic ? "Perspective View" : "Orthographic View")) {
		m_isOrthographic = !m_isOrthographic;
		GameEngine::Transform tempTransform = m_camera->GetTransform();
		GameEngine::Application& app = GameEngine::Application::Get();
		float aspect = (float)app.GetWindow().GetWidth() / app.GetWindow().GetHeight();

		if (m_isOrthographic) {
			m_camera = std::make_shared<GameEngine::OrthographicCamera>(-2.0f, 2.0f, 2.0f / aspect, -2.0f / aspect, 0.1f, 100.0f);
		}
		else {
			m_camera = std::make_shared<GameEngine::PerspectiveCamera>(45.0f, aspect);
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
	if (e.keyCode == GameEngine::KeyCode::Escape) {
		m_moveCamera = false;
	}
	else if (e.keyCode == GameEngine::KeyCode::M) {
		m_moveCamera = true;
	}

	return false;
}

void ExampleLayer::CreateScene()
{
	// set camera
	GameEngine::Application& app = GameEngine::Application::Get();
	float aspect = (float)app.GetWindow().GetWidth() / app.GetWindow().GetHeight();

	if (m_isOrthographic) {
		m_camera = std::make_shared<GameEngine::OrthographicCamera>(-2.0f, 2.0f, 2.0f / aspect, -2.0f / aspect, 0.1f, 100.0f);
	}
	else {
		m_camera = std::make_shared<GameEngine::PerspectiveCamera>(45.0f, aspect);
	}

	// object 1
	m_vertexArray = GameEngine::VertexArray::Create();

	float vertices[3 * 3 + 3 * 4] = {
		-0.5f, 0.0f, 0.0f, 0.8f, 0.0f, 0.2f, 1.0f,
		 0.45f, 0.0f, 0.0f, 0.2f, 0.8f, 0.1f, 1.0f,
		 -0.5f,  0.95f, 0.0f, 0.0f, 0.4f, 0.7f, 1.0f
	};

	GameEngine::VertexBufferLayout layout = {
		{ GameEngine::ShaderDataType::Vec3, "a_position" },
		{ GameEngine::ShaderDataType::Vec4, "a_color" }
	};

	std::shared_ptr<GameEngine::VertexBuffer> vertexBuffer = GameEngine::VertexBuffer::Create(vertices, sizeof(vertices), layout);

	uint32_t indices[3] = { 0, 1, 2 };
	std::shared_ptr<GameEngine::IndexBuffer> indexBuffer = GameEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

	m_vertexArray->AddVertexBuffer(vertexBuffer);
	m_vertexArray->SetIndexBuffer(indexBuffer);


	m_shader = GameEngine::Shader::Create("../GameEngine/res/shaders/red.shader");

	// object 2
	m_vertexArray2 = GameEngine::VertexArray::Create();

	float vertices2[3 * 3] = {
		0.5f, 1.0f, 0.0f,
		-0.45f,  1.0f, 0.0f,
		0.5f, 0.05f, 0.0f
	};

	GameEngine::VertexBufferLayout layout2 = {
		{ GameEngine::ShaderDataType::Vec3, "a_position" }
	};

	std::shared_ptr<GameEngine::VertexBuffer> vertexBuffer2 = GameEngine::VertexBuffer::Create(vertices2, sizeof(vertices2), layout2);

	uint32_t indices2[3] = { 0, 1, 2 };
	std::shared_ptr<GameEngine::IndexBuffer> indexBuffer2 = GameEngine::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t));

	m_vertexArray2->AddVertexBuffer(vertexBuffer2);
	m_vertexArray2->SetIndexBuffer(indexBuffer2);

	m_shader2 = GameEngine::Shader::Create("../GameEngine/res/shaders/blue.shader");

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

	std::shared_ptr<GameEngine::VertexBuffer> vertexBuffer3 = GameEngine::VertexBuffer::Create(vertices3, sizeof(vertices3), layout3);

	uint32_t indices3[6] = { 0, 1, 2, 3, 1, 2 };
	std::shared_ptr<GameEngine::IndexBuffer> indexBuffer3 = GameEngine::IndexBuffer::Create(indices3, sizeof(indices3) / sizeof(uint32_t));

	m_vertexArray3->AddVertexBuffer(vertexBuffer3);
	m_vertexArray3->SetIndexBuffer(indexBuffer3);

	m_shader3 = GameEngine::Shader::Create("../GameEngine/res/shaders/basic.shader");
}

void ExampleLayer::ResetScene()
{
	// reset camera
	m_camera->GetTransform().position = glm::vec3(0.0f, 0.5f, 2.0f);
	m_camera->LookAt({ 0.0f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	m_camRotation = m_camera->GetTransform().GetEulerAngles();

	// reset objects
	objectPositon = glm::vec3(0.0f);
	objectPositon2 = glm::vec3(0.0f);
	objectPositon3 = glm::vec3(0.0f);
}
