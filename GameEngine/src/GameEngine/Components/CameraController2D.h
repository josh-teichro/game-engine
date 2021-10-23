#pragma once

#include "GameEngine/Renderer/Camera.h"
#include "GameEngine/Events/HandlesEvents.h"

namespace GameEngine
{

	class CameraController2D : public HandlesEvents
	{
	public:
		enum class Mode
		{
			Normal, Walk
		};

	public:
		CameraController2D(float zoom, float aspectRatio);
		CameraController2D(float zoom);

		void OnUpdate();

		virtual bool OnMouseDown(const MouseDownEvent& e) override;
		virtual bool OnMouseScroll(const MouseScrollEvent& e) override;
		virtual bool OnWindowResize(const WindowResizeEvent& e) override;

		Ref<OrthographicCamera> GetCamera() { return m_camera; }
		const Ref<OrthographicCamera> GetCamera() const { return m_camera; }

		void SetMode(Mode mode);

		void SetZoom(float zoom);
		void SetAspectRatio(float aspectRatio);
		void SetWalkSpeed(float speed) { m_cameraWalkSpeed = speed; }
		void SetPanSpeed(float speed) { m_cameraPanSpeed = speed; }
		void SetZoomSpeed(float speed) { m_cameraZoomSpeed = speed; }
		void SetPosition(glm::vec2 position) { m_camera->GetTransform().position = {position.x, position.y, 0.0f }; }

	private:
		float m_zoom;
		float m_aspectRatio;
		Ref<OrthographicCamera> m_camera;

		Mode m_mode = Mode::Normal;

		float m_cameraWalkSpeed = 1.0f;
		float m_cameraPanSpeed = 0.01f;
		float m_cameraZoomSpeed = 0.25f;

		glm::vec2 m_prevMousePos = { 0.0f, 0.0f };

	private:
		float GetCameraWalkSpeedMultiplier();
		float GetCameraPanSpeedMultiplier();

	};

}
