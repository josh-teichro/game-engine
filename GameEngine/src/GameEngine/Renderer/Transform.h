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

		glm::vec3 Forward() {
			return glm::normalize(orientation * glm::vec3(0.0f, 0.0f, -1.0f));
		}

		glm::vec3 Right() {
			return glm::normalize(orientation * glm::vec3(1.0f, 0.0f, 0.0f));
		}

		glm::vec3 Up() {
			return glm::normalize(orientation * glm::vec3(0.0f, 1.0f, 0.0f));
		}
	};

}
