#pragma once

#include <glm/glm.hpp>

namespace GameEngine
{

	class Transform
	{
	public:
		glm::vec3 position;
		glm::vec3 scale;
		glm::quat orientation;
	public:
		Transform() : position(0.0f), scale(1.0f), orientation() {}

		void Reset() {
			position = glm::vec3(0.0f);
			scale = glm::vec3(1.0f);
			orientation = glm::quat();
		}

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

		const glm::mat4 ToMat4() const {
			return glm::translate(glm::mat4(1.0), position) * glm::toMat4(orientation) * glm::scale(glm::mat4(1.0), scale);
		}
	};

}
