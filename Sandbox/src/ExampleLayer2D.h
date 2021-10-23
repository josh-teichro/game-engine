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

	GameEngine::Transform m_squareTransform;
	glm::vec3 m_squareRotation;
	glm::vec4 m_squareColor;

	bool m_walk = false;

private:
	void CreateScene();
	void ResetScene();
};
