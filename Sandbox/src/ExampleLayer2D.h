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

	GameEngine::Renderer2D::RectTransform m_backgroundTransform;
	GameEngine::Renderer2D::RectMaterial m_backgroundMaterial;

	GameEngine::Renderer2D::RectTransform m_squareTransform;
	float m_squareRotationDeg;
	GameEngine::Renderer2D::RectMaterial m_squareMaterial;

	bool m_walk = false;

private:
	void CreateScene();
	void ResetScene();
};
