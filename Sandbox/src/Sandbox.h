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

	void OnUpdate() override;
	bool OnEvent(const GameEngine::Event& e) override;
	bool OnMouseDown(const GameEngine::MouseDownEvent& e);

};
