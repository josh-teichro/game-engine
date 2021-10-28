#include "gepch.h"

#include "GameEngine/Core/Input.h"
#include "Platform/Windows/WindowsInput.h"

namespace GameEngine
{
	Scope<Input> Input::s_instance = Input::Create();

	Scope<Input> Input::Create()
	{
		return MakeScope<WindowsInput>();
	}

}
