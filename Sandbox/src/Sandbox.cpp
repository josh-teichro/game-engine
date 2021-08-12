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

bool ExampleLayer::OnMouseDown(const GameEngine::MouseDownEvent& e)
{
	GE_INFO("ExampleLayer::OnMouseDown {}", e);
	return false;
}
