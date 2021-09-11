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
	bool OnKeyDown(const GameEngine::KeyDownEvent& e) override;

};
