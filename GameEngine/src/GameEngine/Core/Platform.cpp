#include "gepch.h"
#include "Platform.h"
#include "Platform/Windows/WindowsPlatformAPI.h"

namespace GameEngine
{
	PlatformAPI* Platform::s_api = new WindowsPlatformAPI();
}
