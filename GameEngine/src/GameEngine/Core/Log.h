#pragma once

#include "GameEngine/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

// glm logging
#include <glm/gtx/io.hpp>


namespace GameEngine {

	/**
	* Logging abstraction.
	*/
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger> GetCoreLogger() { return s_coreLogger; };
		inline static Ref<spdlog::logger> GetClientLogger() { return s_clientLogger; };

	private:
		static Ref<spdlog::logger> s_coreLogger;
		static Ref<spdlog::logger> s_clientLogger;
	};

}

// Logging macros
#define GE_CORE_TRACE(...)		GameEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_CORE_INFO(...)		GameEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GE_CORE_WARN(...)		GameEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_CORE_ERROR(...)		GameEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GE_CORE_CRITICAL(...)	GameEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)
								
#define GE_TRACE(...)			GameEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GE_INFO(...)			GameEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define GE_WARN(...)			GameEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GE_ERROR(...)			GameEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define GE_CRITICAL(...)		GameEngine::Log::GetClientLogger()->critical(__VA_ARGS__)

