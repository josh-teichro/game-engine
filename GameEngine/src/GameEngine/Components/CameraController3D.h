#pragma once

#include "GameEngine/Renderer/Camera.h"
#include "GameEngine/Events/HandlesEvents.h"

namespace GameEngine
{

	class CameraController3D : public HandlesEvents
	{
	public:
		enum class Mode
		{
			Normal, Fly
		};

	public:
		CameraController3D(float fov, float aspectRatio);
		CameraController3D(float fov = 45.0f);

		void OnUpdate();

		virtual bool OnMouseDown(const MouseDownEvent& e) override;
		virtual bool OnMouseUp(const MouseUpEvent& e) override;
		virtual bool OnMouseScroll(const MouseScrollEvent& e) override;
		virtual bool OnWindowResize(const WindowResizeEvent& e) override;

		Ref<Camera> GetCamera() { return m_camera; }
		const Ref<Camera> GetCamera() const { return m_camera; }
		bool GetInvertCameraX() { return m_invertCameraX; }
		bool GetInvertCameraY() { return m_invertCameraY; }
		glm::vec3 GetPosition() { return m_camera->GetTransform().position; }

		void SetMode(Mode mode);
		void SetIsOrthographic(bool isOrthographic);

		void SetFOV(float fov);
		void SetAspectRatio(float aspectRatio);
		void SetZoom(float zoom);
		void SetFlySpeed(float speed) { m_cameraFlySpeed = speed; }
		void SetPanSpeed(float speed) { m_cameraPanSpeed = speed; }
		void SetZoomSpeed(float speed) { m_cameraZoomSpeed = speed; }
		void SetInvertCameraX(bool invert) { m_invertCameraX = invert; }
		void SetInvertCameraY(bool invert) { m_invertCameraY = invert; }
		void SetPosition(glm::vec3 position) { m_camera->GetTransform().position = position; }
		void SetRotation(glm::vec3 eulerAngles) { m_camera->GetTransform().SetEulerAngles(eulerAngles); m_camRotation = eulerAngles; }

		void LookAt(glm::vec3 point, glm::vec3 up = { 0.0f, 1.0f, 0.0f });

	private:
		Ref<Camera> m_camera;
		float m_zoom = 1.0f, m_fov = 45.0f, m_aspectRatio;

		Mode m_mode = Mode::Normal;
		bool m_isOrthographic = false;

		float m_cameraFlySpeed = 3.0f;
		float m_cameraPanSpeed = 0.01f;
		float m_cameraRotationSpeed = 7.0f;
		float m_cameraZoomSpeed = 0.25f;

		bool m_invertCameraX = false;
		bool m_invertCameraY = false;

		glm::vec2 m_prevMousePos = { 0.0f, 0.0f };
		glm::vec3 m_camRotation = { 0.0f, 0.0f, 0.0f };

	private:
		void UpdateCameraPosition(float deltaTime);
		void UpdateCameraRotation(float deltaTime);

		float GetCameraFlySpeedMultiplier();
		float GetCameraPanSpeedMultiplier();
	};

}
