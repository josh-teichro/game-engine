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
	GE_INFO("ExampleLayer::Update");
}

inline void ExampleLayer::OnEvent()
{
	GE_INFO("ExampleLayer::OnEvent");
}
