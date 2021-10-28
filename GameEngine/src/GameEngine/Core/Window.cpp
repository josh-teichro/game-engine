#include "gepch.h"

#include "GameEngine/Core/Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace GameEngine
{

	Scope<Window> Window::Create(const WindowProps& props) {
		return MakeScope<WindowsWindow>(props);
	}

}
