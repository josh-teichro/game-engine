#pragma once

#include "GameEngine/Core/PlatformAPI.h"

namespace GameEngine
{

	class Platform
	{
	public:
		static float GetTime() { return s_api->GetTime(); }

	private:
		static PlatformAPI* s_api;

	};

}
