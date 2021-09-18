#pragma once

#include "glm/glm.hpp"

namespace GameEngine
{

	class Transform
	{
	public:
		glm::vec3 position;
		glm::quat orientation;
	public:
		Transform() : position(0.0f), orientation() {}

		void SetEulerAngles(glm::vec3 eulerAngles) {
			orientation = glm::radians(eulerAngles);
		}

		glm::vec3 GetEulerAngles() {
			return glm::degrees(glm::eulerAngles(orientation));
		}
	};

}
