#pragma once

#include "GameEngine/Core/Platform.h"

namespace GameEngine
{

	class WindowsPlatformAPI : public PlatformAPI
	{
	public:
		virtual float GetTime() override;
	};

}
