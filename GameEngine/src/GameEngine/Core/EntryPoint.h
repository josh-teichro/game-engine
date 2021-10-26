#pragma once

#include "GameEngine/Core/Log.h"

#ifdef GE_PLATFORM_WINDOWS

extern GameEngine::Application* GameEngine::CreateApplication();

int main(int argc, char** argv) {
	GameEngine::Log::Init();

	GE_PROFILE_BEGIN_SESSION("Startup", "GameEngineProfile-Startup.json");
	auto app = GameEngine::CreateApplication();
	GE_PROFILE_END_SESSION();

	GE_PROFILE_BEGIN_SESSION("Startup", "GameEngineProfile-Runtime.json");
	app->Run();
	GE_PROFILE_END_SESSION();

	GE_PROFILE_BEGIN_SESSION("Startup", "GameEngineProfile-Shutdown.json");
	delete app;
	GE_PROFILE_END_SESSION();
}

#endif
