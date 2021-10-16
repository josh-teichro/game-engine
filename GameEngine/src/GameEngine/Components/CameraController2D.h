#pragma once

#include "GameEngine/Renderer/Camera.h"
#include "GameEngine/Events/HandlesEvents.h"

namespace GameEngine
{

	class CameraController2D : public HandlesEvents
	{
	public:
		CameraController2D(float zoom, float aspectRatio);
		CameraController2D(float zoom);

		void OnUpdate();

		virtual bool OnMouseScroll(const MouseScrollEvent& e) override;
		virtual bool OnWindowResize(const WindowResizeEvent& e) override;

		Ref<OrthographicCamera> GetCamera() { return m_camera; }
		const Ref<OrthographicCamera> GetCamera() const { return m_camera; }

		void SetZoom(float zoom);
		void SetAspectRatio(float aspectRatio);
		void SetMoveSpeed(float speed) { m_cameraMoveSpeed = speed; }
		void SetZoomSpeed(float speed) { m_cameraZoomSpeed = speed; }
		void SetPosition(glm::vec2 position) { m_camera->GetTransform().position = {position.x, position.y, 1.0f }; }

	private:
		float m_zoom;
		float m_aspectRatio;
		Ref<OrthographicCamera> m_camera;

		float m_cameraMoveSpeed = 1.0f;
		float m_cameraZoomSpeed = 0.25f;

	private:
		float GetCameraMoveSpeedMultiplier();

	};

}
