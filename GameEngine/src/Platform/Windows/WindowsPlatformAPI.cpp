#include "gepch.h"
#include "WindowsPlatformAPI.h"

#include <GLFW/glfw3.h>

namespace GameEngine
{

	float WindowsPlatformAPI::GetTime()
	{
		return (float)glfwGetTime();
	}

}
