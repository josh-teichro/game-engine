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

private:
	std::shared_ptr<GameEngine::Camera> m_camera;
	glm::vec3 m_camRotation;
	float m_cameraSpeed = 0.1f;
	float m_cameraRotationSpeed = 0.1f;
	bool m_lookAtObject = false;
	bool m_isOrthographic = false;
	bool m_moveCamera = true;
	bool m_invertCameraX = false;
	bool m_invertCameraY = false;
	glm::vec2 m_prevMousePos = { 0.0f, 0.0f };

	std::shared_ptr<GameEngine::Shader> m_shader;
	std::shared_ptr<GameEngine::VertexArray> m_vertexArray;
	glm::vec3 objectPositon;

	std::shared_ptr<GameEngine::Shader> m_shader2;
	std::shared_ptr<GameEngine::VertexArray> m_vertexArray2;
	glm::vec3 objectPositon2;

	std::shared_ptr<GameEngine::Shader> m_shader3;
	std::shared_ptr<GameEngine::VertexArray> m_vertexArray3;
	glm::vec3 objectPositon3;

private:
	void CreateScene();
	void ResetScene();

};
