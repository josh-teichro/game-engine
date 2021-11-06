#pragma once

#include <GameEngine.h>

class ExampleLayer2D : public GameEngine::Layer
{
public:
	ExampleLayer2D();
	virtual ~ExampleLayer2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate() override;
	virtual void OnImGuiUpdate() override;

	virtual bool OnEvent(const GameEngine::Event& e) override;
	virtual bool OnKeyUp(const GameEngine::KeyUpEvent& e) override;

private:
	GameEngine::CameraController2D m_cameraController;

	GameEngine::ShaderLibrary m_shaderLibrary;

	GameEngine::Renderer2D::RectTransform m_checkerboardTransform;
	GameEngine::Renderer2D::RectMaterial m_checkerboardMaterial;

	GameEngine::Renderer2D::RectTransform m_squareTransform;
	float m_squareRotationDeg;
	GameEngine::Renderer2D::RectMaterial m_squareMaterial;

	glm::vec4 m_gradientStart, m_gradientStop;
	int32_t m_gradientSteps;

	// red hood character
	const glm::vec2 c_characterSheetSize = { 1344, 1463 };
	const glm::vec2 c_characterSheetNumCells = { 12.0f, 11.0f };
	const glm::vec2 c_characterSheetCellSize = c_characterSheetSize / c_characterSheetNumCells;

	GameEngine::Renderer2D::RectTransform m_characterTransform;
	GameEngine::Renderer2D::RectMaterial m_characterMaterial;
	int32_t m_characterAnimationIndex;
	int32_t m_characterAnimationFrameIndex;
	float m_characterAnimationSpeed;
	float m_characterAnimationTime;

	bool m_walk = false;

private:
	void CreateScene();
	void ResetScene();
};
