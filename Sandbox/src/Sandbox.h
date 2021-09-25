#pragma once

#include <GameEngine.h>

/**
* Sandbox application for testing.
*/
class Sandbox : public GameEngine::Application {
public:
	Sandbox();
	virtual ~Sandbox();
};

/**
* Example of a Layer.
*/
class ExampleLayer : public GameEngine::Layer
{
public:
	ExampleLayer();

	virtual void OnUpdate() override;
	virtual void OnImGuiUpdate() override;

	bool OnMouseDown(const GameEngine::MouseDownEvent& e) override;
	bool OnKeyUp(const GameEngine::KeyUpEvent& e) override;
	bool OnWindowResize(const GameEngine::WindowResizeEvent& e) override;

private:
	GameEngine::Ref<GameEngine::Camera> m_camera;
	glm::vec3 m_camRotation;
	float m_cameraSpeed = 5.0f;
	float m_cameraRotationSpeed = 5.0f;
	bool m_lookAtObject = false;
	bool m_isOrthographic = false;
	bool m_moveCamera;
	bool m_invertCameraX = false;
	bool m_invertCameraY = false;
	glm::vec2 m_prevMousePos = { 0.0f, 0.0f };

	GameEngine::Ref<GameEngine::Shader> m_shader;
	GameEngine::Ref<GameEngine::VertexArray> m_vertexArray;
	GameEngine::Transform objectTransform;

	GameEngine::Ref<GameEngine::Shader> m_shader2;
	GameEngine::Ref<GameEngine::VertexArray> m_vertexArray2;
	GameEngine::Transform objectTransform2;

	GameEngine::Ref<GameEngine::Shader> m_shader3;
	GameEngine::Ref<GameEngine::VertexArray> m_vertexArray3;
	GameEngine::Transform objectTransform3;

private:
	void CreateScene();
	void ResetScene();

};
