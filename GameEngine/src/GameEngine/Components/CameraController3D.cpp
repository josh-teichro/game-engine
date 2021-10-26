#include "gepch.h"

#include "GameEngine/Components/CameraController3D.h"
#include "GameEngine/Core/Application.h"
#include "GameEngine/Core/Time.h"
#include "GameEngine/Events/Input.h"

namespace GameEngine
{
	static float GetWindowAspectRatio()
	{
		Application& app = Application::Get();
		return (float)app.GetWindow().GetWidth() / app.GetWindow().GetHeight();
	}

	CameraController3D::CameraController3D(float fov, float aspectRatio) :
		m_fov(fov),
		m_aspectRatio(aspectRatio)
	{
		m_camera = MakeRef<PerspectiveCamera>(m_fov, m_aspectRatio);
		m_camera->GetTransform().SetEulerAngles(m_camRotation);
	}

	CameraController3D::CameraController3D(float fov) :
		CameraController3D(fov, GetWindowAspectRatio())
	{
	}

	void CameraController3D::OnUpdate()
	{
		GE_PROFILE_FUNCTION();

		float deltaTime = Time::GetDeltaTime();

		UpdateCameraPosition(deltaTime);
		UpdateCameraRotation(deltaTime);
	}

	bool CameraController3D::OnMouseDown(const MouseDownEvent& e)
	{
		//GE_CORE_INFO("OnMouseDown: {}", e.button);
		if (m_mode == Mode::Normal)
			m_prevMousePos = Input::GetMousePosition();
		return false;
	}

	bool CameraController3D::OnMouseUp(const MouseUpEvent& e)
	{
		//GE_CORE_INFO("OnMouseUp: {}", e.button);
		return false;
	}

	bool CameraController3D::OnMouseScroll(const MouseScrollEvent& e)
	{
		if (m_isOrthographic)
		{
			SetZoom(m_zoom - e.offsetY * m_cameraZoomSpeed);
		}
		else if (m_mode == Mode::Normal)
		{
			m_camera->GetTransform().position += e.offsetY * m_cameraZoomSpeed * m_camera->GetTransform().Forward();
		}

		return false;
	}

	bool CameraController3D::OnWindowResize(const WindowResizeEvent& e)
	{
		float aspectRatio = (float)e.width / (float)e.height;
		SetAspectRatio(aspectRatio);
		return false;
	}

	void CameraController3D::SetMode(Mode mode)
	{
		if (m_mode == mode)
			return;

		if (mode == Mode::Fly)
		{
			GameEngine::Input::LockMouseCursor();
			m_prevMousePos = Input::GetMousePosition();
		}
		else
			GameEngine::Input::UnlockMouseCursor();

		m_mode = mode;
	}

	void CameraController3D::SetIsOrthographic(bool isOrthographic)
	{
		if (isOrthographic == m_isOrthographic)
			return;

		GameEngine::Transform tempTransform = m_camera->GetTransform();

		if (isOrthographic) {
			float left = -m_aspectRatio * m_zoom;
			float right = m_aspectRatio * m_zoom;
			float bottom = -m_zoom;
			float top = m_zoom;
			m_camera = MakeRef<OrthographicCamera>(left, right, bottom, top, 0.1f, 100.0f);
		}
		else {
			m_camera = MakeRef<PerspectiveCamera>(m_fov, m_aspectRatio);
		}

		m_camera->GetTransform() = tempTransform;
		m_isOrthographic = isOrthographic;
	}

	void CameraController3D::SetFOV(float fov)
	{
		m_fov = fov;

		if (!m_isOrthographic) {
			std::dynamic_pointer_cast<PerspectiveCamera>(m_camera)->SetFOV(m_fov);
		}
	}

	void CameraController3D::SetAspectRatio(float aspectRatio)
	{
		m_aspectRatio = aspectRatio;

		if (m_isOrthographic)
		{
			float left = -aspectRatio * m_zoom;
			float right = aspectRatio * m_zoom;
			float bottom = -m_zoom;
			float top = m_zoom;
			std::dynamic_pointer_cast<OrthographicCamera>(m_camera)->SetBounds(left, right, bottom, top, 0.1f, 100.0f);
		} 
		else
		{
			std::dynamic_pointer_cast<PerspectiveCamera>(m_camera)->SetAspectRatio(aspectRatio);
		}
	}

	void CameraController3D::SetZoom(float zoom)
	{
		m_zoom = std::max(zoom, 0.25f);

		if (m_isOrthographic)
		{
			float left = -m_aspectRatio * zoom;
			float right = m_aspectRatio * zoom;
			float bottom = -zoom;
			float top = zoom;
			std::dynamic_pointer_cast<OrthographicCamera>(m_camera)->SetBounds(left, right, bottom, top, 0.1f, 100.0f);
		}
	}

	void CameraController3D::LookAt(glm::vec3 point, glm::vec3 up)
	{
		if (point == m_camera->GetTransform().position)
			return;

		m_camera->LookAt(point, up);
		m_camRotation = m_camera->GetTransform().GetEulerAngles();
	}

	void CameraController3D::UpdateCameraPosition(float deltaTime)
	{
		if (m_mode == Mode::Normal)
		{
			if (Input::GetMouseDown(MouseButton::Middle))
			{
				glm::vec2 newMousePos = Input::GetMousePosition();
				float dx = newMousePos.x - m_prevMousePos.x;
				float dy = newMousePos.y - m_prevMousePos.y;
				m_camera->GetTransform().position += (-dx * m_camera->GetTransform().Right() + dy * m_camera->GetTransform().Up()) * GetCameraPanSpeedMultiplier();
				m_prevMousePos = newMousePos;
			}
		}

		if (m_mode == Mode::Fly)
		{
			float moveSpeedMult = GetCameraFlySpeedMultiplier();

			if (Input::GetKeyDown(KeyCode::W)) {
				m_camera->GetTransform().position += deltaTime * m_camera->GetTransform().Forward() * moveSpeedMult;
			}
			else if (Input::GetKeyDown(KeyCode::S)) {
				m_camera->GetTransform().position -= deltaTime * m_camera->GetTransform().Forward() * moveSpeedMult;
			}

			if (Input::GetKeyDown(KeyCode::A)) {
				m_camera->GetTransform().position -= deltaTime * m_camera->GetTransform().Right() * moveSpeedMult;
			}
			else if (Input::GetKeyDown(KeyCode::D)) {
				m_camera->GetTransform().position += deltaTime * m_camera->GetTransform().Right() * moveSpeedMult;
			}

			if (Input::GetKeyDown(KeyCode::LeftShift)) {
				m_camera->GetTransform().position -= deltaTime * m_camera->GetTransform().Up() * moveSpeedMult;
			}
			else if (Input::GetKeyDown(KeyCode::Space)) {
				m_camera->GetTransform().position += deltaTime * m_camera->GetTransform().Up() * moveSpeedMult;
			}
		}
	}

	void CameraController3D::UpdateCameraRotation(float deltaTime)
	{
		bool canRotate = false;

		if (m_mode == Mode::Normal)
		{
			canRotate = Input::GetMouseDown(MouseButton::Right);
		}

		if (m_mode == Mode::Fly)
		{
			canRotate = true;
		}

		if (canRotate) {
			glm::vec2 newMousePos = Input::GetMousePosition();
			float dx = newMousePos.x - m_prevMousePos.x;
			float dy = newMousePos.y - m_prevMousePos.y;
			m_camRotation.y -= (m_invertCameraX ? -1.0f : 1.0f) * deltaTime * dx * m_cameraRotationSpeed;
			m_camRotation.x -= (m_invertCameraY ? -1.0f : 1.0f) * deltaTime * dy * m_cameraRotationSpeed;
			m_prevMousePos = newMousePos;
			m_camera->GetTransform().SetEulerAngles(m_camRotation);
		}
	}

	float CameraController3D::GetCameraFlySpeedMultiplier()
	{
		return m_cameraFlySpeed * m_zoom;
	}

	float CameraController3D::GetCameraPanSpeedMultiplier()
	{
		return m_cameraPanSpeed * m_zoom;
	}

}
