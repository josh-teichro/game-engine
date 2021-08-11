#include "Sandbox.h"

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
}

inline void ExampleLayer::OnUpdate()
{
	//GE_INFO("ExampleLayer::Update");
}

bool ExampleLayer::OnEvent(const GameEngine::Event& e)
{
	return GameEngine::EventDispatcher(e).Dispatch<GameEngine::MouseDownEvent>(std::bind(&ExampleLayer::OnMouseDown, this, std::placeholders::_1));
}

bool ExampleLayer::OnMouseDown(const GameEngine::MouseDownEvent& e)
{
	GE_INFO("ExampleLayer::OnMouseDown {}", e);
	return false;
}
