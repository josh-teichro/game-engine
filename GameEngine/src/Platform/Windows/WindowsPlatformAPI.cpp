#include "gepch.h"

#include "Platform/Windows/WindowsPlatformAPI.h"

#include <GLFW/glfw3.h>

namespace GameEngine
{

	float WindowsPlatformAPI::GetTime()
	{
		GE_PROFILE_FUNCTION();

		return (float)glfwGetTime();
	}

}
