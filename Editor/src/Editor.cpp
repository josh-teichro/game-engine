#include "Editor.h"
#include "EditorLayer.h"

#include <GameEngine/Core/EntryPoint.h>

/**
* Editor.
*/
Editor::Editor()
{
	PushLayer(new EditorLayer());
}

Editor::~Editor()
{
}

GameEngine::Application* GameEngine::CreateApplication() {
	return new Editor();
}
