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

	void Camera::RecalculateMatrices()
	{
		RecalculatePMatrix();
		RecalculateVMatrix();
		m_VP = m_P * m_V;
	}

	void Camera::RecalculateVMatrix()
	{
		m_V = glm::translate(glm::mat4(1.0f), m_transform.position) * glm::toMat4(m_transform.orientation);
		m_V = glm::inverse(m_V);
	}

	void Camera::LookAt(glm::vec3 point, glm::vec3 up)
	{
		glm::mat4 camModel = glm::lookAt(m_transform.position, point, up);
		glm::vec3 scale;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(camModel, scale, m_transform.orientation, translation, skew, perspective);
		m_transform.orientation = glm::inverse(m_transform.orientation);
	}

	// -----------------------------------------------------------------
	// PerspectiveCamera -----------------------------------------------
	// -----------------------------------------------------------------

	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float clipNear, float clipFar) :
		m_fov(fov),
		m_aspectRatio(aspect),
		m_clipNear(clipNear),
		m_clipFar(clipFar) {}

	void PerspectiveCamera::RecalculatePMatrix()
	{
		m_P = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_clipNear, m_clipFar);
	}

	void PerspectiveCamera::SetFOV(float fov)
	{
		m_fov = fov;
	}

	void PerspectiveCamera::SetAspectRatio(float aspectRatio)
	{
		m_aspectRatio = aspectRatio;
	}

	void PerspectiveCamera::SetClipNear(float clipNear)
	{
		m_clipNear = clipNear;
	}

	void PerspectiveCamera::SetClipFar(float clipFar)
	{
		m_clipFar = clipFar;
	}

	// -----------------------------------------------------------------
	// OrthographicCamera ----------------------------------------------
	// -----------------------------------------------------------------

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar) :
		m_left(left),
		m_right(right),
		m_bottom(bottom),
		m_top(top),
		m_zNear(zNear),
		m_zFar(zFar) {}

	void OrthographicCamera::RecalculatePMatrix()
	{
		m_P = glm::ortho(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
	}

	void OrthographicCamera::SetBounds(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
		m_zNear = zNear;
		m_zFar = zFar;
	}

}
