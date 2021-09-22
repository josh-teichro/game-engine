#pragma once

#include "Transform.h"
#include "glm/glm.hpp"

namespace GameEngine
{

	/**
	* Camera
	*/
	class Camera
	{
	public:
		void ComputeMatrices();
		void LookAt(glm::vec3 point, glm::vec3 up);

		glm::mat4 V() { if (m_needToComputeMatrices) { ComputeMatrices(); } return m_V; }
		glm::mat4 P() { if (m_needToComputeMatrices) { ComputeMatrices(); }  return m_P; }
		glm::mat4 VP() { if (m_needToComputeMatrices) { ComputeMatrices(); } return m_VP; }

		const Transform& GetReadOnlyTransform() const { return m_transform; }
		Transform& GetTransform() { return m_transform; m_needToComputeMatrices = true; }

	protected:
		Camera();

	protected:
		Transform m_transform;
		bool m_needToComputeMatrices;

		glm::mat4 m_V;
		glm::mat4 m_P;
		glm::mat4 m_VP;
	};

	/**
	* PerspectiveCamera
	*/
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspect, float clipNear = 0.1f, float clipFar = 100.0f);
	};

	/**
	* OrthographicCamera
	*/
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float top, float bottom, float zNear, float zFar);
	};

}
