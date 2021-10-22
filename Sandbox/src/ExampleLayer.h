#pragma once

#include <GameEngine.h>

/**
* Example of a Layer.
*/
class ExampleLayer : public GameEngine::Layer
{
public:
	ExampleLayer();

	virtual void OnUpdate() override;
	virtual void OnImGuiUpdate() override;

	virtual bool OnEvent(const GameEngine::Event& e) override;
	bool OnMouseDown(const GameEngine::MouseDownEvent& e) override;
	bool OnKeyUp(const GameEngine::KeyUpEvent& e) override;
	bool OnWindowResize(const GameEngine::WindowResizeEvent& e) override;

private:
	bool m_lookAtObject = false;
	bool m_fly = false;
	bool m_walk = false;
	bool m_isOrthographic = false;
	bool m_invertCameraX = false;
	bool m_invertCameraY = false;
	GameEngine::CameraController2D m_cameraController2D;
	GameEngine::CameraController3D m_cameraController3D;

	bool m_is2D = false;

	GameEngine::ShaderLibrary m_shaderLibrary;

	GameEngine::Ref<GameEngine::VertexArray> m_vertexArray;
	GameEngine::Ref<GameEngine::Texture2D> m_texture;
	GameEngine::Transform m_objectTransform;

	GameEngine::Ref<GameEngine::VertexArray> m_vertexArray3;
	GameEngine::Transform m_objectTransform3;

private:
	void CreateScene();
	void ResetScene();

};
