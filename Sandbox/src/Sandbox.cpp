#include "Sandbox.h"
#include "ExampleLayer.h"
#include "ExampleLayer2D.h"

#include <GameEngine/Core/EntryPoint.h>

/**
* Sandbox.
*/
Sandbox::Sandbox()
{
	//PushLayer(new ExampleLayer());
	PushLayer(new ExampleLayer2D());
}

Sandbox::~Sandbox()
{
}

GameEngine::Application* GameEngine::CreateApplication() {
	return new Sandbox();
}
