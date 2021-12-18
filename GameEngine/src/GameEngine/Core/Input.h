#pragma once

#include "gepch.h"

#include "GameEngine/Events/KeyCodes.h"
#include "GameEngine/Events/MouseCodes.h"
#include "GameEngine/Events/MouseEvent.h"

#include <glm/vec2.hpp>

namespace GameEngine {

	class Input
	{
	protected:
		Input() = default;

	public:
		static void Init();
		static void Shutdown();

		static glm::vec2 GetMousePosition();
		static bool GetMouseDown(MouseButton button);
		static bool GetKeyDown(KeyCode keycode);
		static const char* GetClipBoardText(); 

		static void SetMouseCursor(MouseCursor cursor); 
		static void SetClipBoardText(const char* text); 

		static void LockMouseCursor(); 
		static void UnlockMouseCursor(); 

	private:
		friend class Window;
	};

}

