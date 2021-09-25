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
		virtual ~Camera() = default;

		virtual void RecalculateMatrices();
		virtual void RecalculateVMatrix();
		virtual void RecalculatePMatrix() = 0;

		void LookAt(glm::vec3 point, glm::vec3 up);

		glm::mat4 V() { return m_V; }
		glm::mat4 P() { return m_P; }
		glm::mat4 VP() { return m_VP; }

		Transform& GetTransform() { return m_transform; }

	protected:
		Camera();

	protected:
		Transform m_transform;

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

		virtual void RecalculatePMatrix() override;

		void SetFOV(float fov);
		void SetAspect(float aspect);
		void SetClipNear(float clipNear);
		void SetClipFar(float clipFar);

	private:
		float m_fov;
		float m_aspect;
		float m_clipNear, m_clipFar;
	};

	/**
	* OrthographicCamera
	*/
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float zNear = 0.1f, float zFar = 100.0f);

		virtual void RecalculatePMatrix() override;

		void SetBounds(float left, float right, float bottom, float top, float zNear = 0.1f, float zFar = 100.0f);

	private:
		float m_left, m_right, m_bottom, m_top, m_zNear, m_zFar;
	};

}
