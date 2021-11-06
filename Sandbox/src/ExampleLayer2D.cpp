#include "ExampleLayer2D.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer2D::ExampleLayer2D() : 
	m_cameraController(1.0f)
{
	CreateScene();
}

void ExampleLayer2D::OnAttach()
{
	ResetScene();
}

void ExampleLayer2D::OnDetach()
{
}

void ExampleLayer2D::OnUpdate()
{
	GE_PROFILE_FUNCTION();

	float deltaTime = GameEngine::Time::GetDeltaTime();

	m_cameraController.OnUpdate();

	// red hood character
	m_characterAnimationTime += deltaTime;
	while (m_characterAnimationTime > 1 / m_characterAnimationSpeed)
	{
		m_characterAnimationFrameIndex = (m_characterAnimationFrameIndex + 1) % (int)c_characterSheetNumCells[1];
		m_characterAnimationTime -= 1 / m_characterAnimationSpeed;
	}

	GameEngine::Renderer2D::RectTexture& characterTexture = m_characterMaterial.texture;
	characterTexture.textureCoords[0] = glm::vec2(m_characterAnimationFrameIndex, m_characterAnimationIndex) / c_characterSheetNumCells;
	characterTexture.textureCoords[1] = glm::vec2(m_characterAnimationFrameIndex + 1, m_characterAnimationIndex + 1) / c_characterSheetNumCells;

	// draw scene
	GameEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	GameEngine::RenderCommand::Clear();

	GameEngine::Renderer2D::ResetStats();
	GameEngine::Renderer2D::BeginScene(m_cameraController.GetCamera());

	m_squareTransform.rotation = glm::radians(m_squareRotationDeg);
	GameEngine::Renderer2D::DrawRect(m_squareTransform, m_squareMaterial);
	GameEngine::Renderer2D::DrawRect(m_checkerboardTransform, m_checkerboardMaterial);

	// draw gradient
	GameEngine::Renderer2D::RectTransform transform;
	GameEngine::Renderer2D::RectMaterial material;
	float minX = -2.0f, maxX = 2.0f, minY = -2.0f, maxY = 2.0f;
	transform.size = { (maxX - minX) / m_gradientSteps, (maxY - minY) / m_gradientSteps }; 
	transform.zIndex = -0.1f;

	for (int32_t i = 0; i < m_gradientSteps; i++)
	{
		for (int32_t j = 0; j < m_gradientSteps; j++)
		{
			float tx = ((float)i + 0.5f) / (float)m_gradientSteps;
			float ty = ((float)j + 0.5f) / (float)m_gradientSteps;
			transform.position = { minX + tx * (maxX - minX), minY + ty * (maxY - minY) };
			material.color = m_gradientStart + glm::vec4(tx, tx + ty / 2.0f, ty, 1.0f) * (m_gradientStop - m_gradientStart);
			GameEngine::Renderer2D::DrawRect(transform, material);
		}
	}

	// draw character
	GameEngine::Renderer2D::DrawRect(m_characterTransform, m_characterMaterial);

	GameEngine::Renderer2D::EndScene();
}

void ExampleLayer2D::OnImGuiUpdate()
{
	GE_PROFILE_FUNCTION();

	ImGui::Text("Square");
	ImGui::DragFloat2("Square Position", glm::value_ptr(m_squareTransform.position), 0.1f);
	ImGui::DragFloat("Square Rotation", &m_squareRotationDeg, 1.0f, 0.0f, 360.0f);
	ImGui::DragFloat2("Square Size", glm::value_ptr(m_squareTransform.size), 0.1f, 0.0f);
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareMaterial.color));

	ImGui::Text("Checkerboard");
	ImGui::DragFloat2("Checkerboard Tiling Offset", &m_checkerboardMaterial.textureOffset[0], 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat2("Checkerboard Tiling Scale", &m_checkerboardMaterial.textureScale[0], 0.01f, 0.0f, 10.0f);

	ImGui::Text("Gradient");
	ImGui::ColorEdit4("Gradient Start", glm::value_ptr(m_gradientStart));
	ImGui::ColorEdit4("Gradient Stop", glm::value_ptr(m_gradientStop));
	ImGui::DragInt("Gradient Steps", &m_gradientSteps, 1.0f, 1, 10000);

	ImGui::Text("Character");
	ImGui::DragFloat2("Position", glm::value_ptr(m_characterTransform.position), 0.1f);
	if (ImGui::BeginCombo("Animation", std::to_string(m_characterAnimationIndex).c_str()))
	{
		for (int i = 0; i < (int)c_characterSheetNumCells[0]; i++)
		{
			bool is_selected = (m_characterAnimationIndex == i);
			if (ImGui::Selectable(std::to_string(i).c_str(), is_selected)) {
				m_characterAnimationIndex = i;
				m_characterAnimationFrameIndex = 0;
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();  
		}
		ImGui::EndCombo();
	}
	ImGui::DragFloat("Animation Speed", &m_characterAnimationSpeed, 1.0f, 0.0f, 60.0f);

	ImGui::BeginDisabled(m_walk);
	if (ImGui::Button("Walk"))
	{
		m_walk = true;
		m_cameraController.SetMode(GameEngine::CameraController2D::Mode::Walk);
	}
	ImGui::EndDisabled();

	if (ImGui::Button("Reset"))
	{
		ResetScene();
	}

	auto stats = GameEngine::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.vertexCount);
	ImGui::Text("Indices: %d", stats.indexCount);
	ImGui::Text("Max Texture Slots Used: %d", stats.maxTextureSlotsUsed);
}

bool ExampleLayer2D::OnEvent(const GameEngine::Event& e)
{
	GE_PROFILE_FUNCTION();

	bool result = false;
	result |= m_cameraController.OnEvent(e);
	result |= HandlesEvents::OnEvent(e);
	return result;
}

bool ExampleLayer2D::OnKeyUp(const GameEngine::KeyUpEvent& e)
{
	if (e.keyCode == GameEngine::KeyCode::Escape)
	{
		m_walk = false;
		m_cameraController.SetMode(GameEngine::CameraController2D::Mode::Normal);
	}
	else if (e.keyCode == GameEngine::KeyCode::W)
	{
		m_walk = true;
		m_cameraController.SetMode(GameEngine::CameraController2D::Mode::Walk);
	}

	return false;
}

void ExampleLayer2D::CreateScene()
{
	GE_PROFILE_FUNCTION();

	GameEngine::Renderer2D::RectTexture checkerboardTexture;
	checkerboardTexture.baseTexture = GameEngine::Texture2D::Create("./res/textures/checkerboard.png");
	checkerboardTexture.baseTexture->SetWrapMode(GameEngine::Texture::WrapMode::Repeat);
	m_checkerboardMaterial.texture = checkerboardTexture;
	m_checkerboardTransform.position = { 0.0f, 0.0f };
	m_checkerboardTransform.size = { 2.0f, 2.0f };

	GameEngine::Renderer2D::RectTexture characterTexture;
	characterTexture.baseTexture = GameEngine::Texture2D::Create("./res/textures/red-hood-character-sheet.png");
	characterTexture.baseTexture->SetFilter(GameEngine::Texture::Filter::Nearest);
	m_characterMaterial.texture = characterTexture;
	m_characterTransform.zIndex = 0.2f;
	m_characterTransform.size = { 2.0f, 2.0f };
}

void ExampleLayer2D::ResetScene()
{
	GE_PROFILE_FUNCTION();

	m_cameraController.SetZoom(1.0f);
	m_cameraController.SetPosition({ 0.0f, 0.0f });

	m_checkerboardMaterial.textureOffset = { 0.0f, 0.0f };
	m_checkerboardMaterial.textureScale = { 1.0f, 1.0f };

	m_squareTransform.position = { 0.0f, 0.0f };
	m_squareTransform.size = { 1.0f, 1.0f };
	m_squareTransform.rotation = m_squareRotationDeg = 0.0f;
	m_squareTransform.zIndex = 0.1f;
	m_squareMaterial.color = { 0.0f, 0.0f, 0.0f, 1.0f };

	m_gradientStart = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_gradientStop = { 0.28f, 0.28f, 0.39f, 1.0f };
	m_gradientSteps = 100;

	m_characterAnimationIndex = 9;
	m_characterAnimationFrameIndex = 0;
	m_characterAnimationSpeed = 30;
	m_characterAnimationTime = 0.0f;
	m_characterTransform.position = { 0.0f, 0.0f };
}
