#pragma once

#include "gepch.h"

namespace GameEngine
{

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Ref = std::shared_ptr<T>;


	template<typename T, typename...Args>
	std::unique_ptr<T> MakeScope(Args&&...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename...Args>
	std::shared_ptr<T> MakeRef(Args&&...args) 
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
