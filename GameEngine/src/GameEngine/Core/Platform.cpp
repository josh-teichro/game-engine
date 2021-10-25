#include "gepch.h"

#include "GameEngine/Core/Platform.h"
#include "Platform/Windows/WindowsPlatformAPI.h"

namespace GameEngine
{
	PlatformAPI* Platform::s_api = new WindowsPlatformAPI();
}
