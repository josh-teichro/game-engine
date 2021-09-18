#include "gepch.h"
#include "Camera.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace GameEngine {

	// -----------------------------------------------------------------
	// Camera ----------------------------------------------------------
	// -----------------------------------------------------------------

	Camera::Camera() :
		m_V(0.0f),
		m_P(0.0f),
		m_VP(0.0f)
	{
	}

	void Camera::ComputeMatrices()
	{
		m_V = glm::toMat4(glm::inverse(m_transform.orientation));
		m_V = glm::translate(m_V, -m_transform.position);
		m_VP = m_P * m_V;
	}

	void Camera::LookAt(glm::vec3 point, glm::vec3 up)
	{
		glm::mat4 camModel = glm::lookAt(m_transform.position, point, up);
		glm::vec3 scale;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(camModel, scale, m_transform.orientation, translation, skew, perspective);
	}

	// -----------------------------------------------------------------
	// PerspectiveCamera -----------------------------------------------
	// -----------------------------------------------------------------

	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float clipNear, float clipFar)
	{
		m_P = glm::perspective(glm::radians(fov), aspect, clipNear, clipFar);
	}

	// -----------------------------------------------------------------
	// OrthographicCamera ----------------------------------------------
	// -----------------------------------------------------------------

	OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom, float zNear, float zFar)
	{
		m_P = glm::ortho(left, right, bottom, top, zNear, zFar);
		m_VP = m_P;
	}

}
