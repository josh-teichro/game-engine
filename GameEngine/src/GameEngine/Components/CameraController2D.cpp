#include "gepch.h"
#include "CameraController2D.h"
#include "GameEngine/Events/Input.h"
#include "GameEngine/Core/Time.h"
#include "GameEngine/Application.h"

namespace GameEngine
{
	static float GetWindowAspectRatio()
	{
		//auto window = Application::Get().GetWindow();
		//return (float)window.GetWidth() / (float)window.GetHeight();
		Application& app = Application::Get();
		return (float)app.GetWindow().GetWidth() / app.GetWindow().GetHeight();
	}

	CameraController2D::CameraController2D(float zoom, float aspectRatio) :
		m_zoom(zoom),
		m_aspectRatio(aspectRatio)
	{
		float left = -aspectRatio * zoom;
		float right = aspectRatio * zoom;
		float bottom = -zoom;
		float top = zoom;
		m_camera = MakeRef<OrthographicCamera>(left, right, bottom, top);

		m_camera->GetTransform().position = { 0.0f, 0.0f, 1.0f };
		m_camera->GetTransform().SetEulerAngles({ 0.0f, 0.0f, 0.0f });
	}

	CameraController2D::CameraController2D(float zoom) :
		CameraController2D(zoom, GetWindowAspectRatio()) {}

	void CameraController2D::OnUpdate()
	{
		float deltaTime = Time::GetDeltaTime();
		float moveSpeedMult = GetCameraMoveSpeedMultiplier();

		if (Input::GetKeyDown(KeyCode::W)) {
			m_camera->GetTransform().position += deltaTime * m_camera->GetTransform().Up() * moveSpeedMult;
		}
		else if (Input::GetKeyDown(KeyCode::S)) {
			m_camera->GetTransform().position -= deltaTime * m_camera->GetTransform().Up() * moveSpeedMult;
		}

		if (Input::GetKeyDown(KeyCode::A)) {
			m_camera->GetTransform().position -= deltaTime * m_camera->GetTransform().Right() * moveSpeedMult;
		}
		else if (Input::GetKeyDown(KeyCode::D)) {
			m_camera->GetTransform().position += deltaTime * m_camera->GetTransform().Right() * moveSpeedMult;
		}
	}

	bool CameraController2D::OnMouseScroll(const MouseScrollEvent& e)
	{
		SetZoom(m_zoom - e.offsetY * m_cameraZoomSpeed);
		return false;
	}

	bool CameraController2D::OnWindowResize(const WindowResizeEvent& e)
	{
		float aspectRatio = (float)e.width / (float)e.height;
		SetAspectRatio(aspectRatio);
		return false;
	}

	void CameraController2D::SetZoom(float zoom)
	{
		m_zoom = std::max(zoom, 0.25f);
		float left = -m_aspectRatio * zoom;
		float right = m_aspectRatio * zoom;
		float bottom = -zoom;
		float top = zoom;
		m_camera->SetBounds(left, right, bottom, top);
	}

	void CameraController2D::SetAspectRatio(float aspectRatio)
	{
		m_aspectRatio = aspectRatio;
		float left = -aspectRatio * m_zoom;
		float right = aspectRatio * m_zoom;
		float bottom = -m_zoom;
		float top = m_zoom;
		m_camera->SetBounds(left, right, bottom, top);
	}

	float CameraController2D::GetCameraMoveSpeedMultiplier()
	{
		return m_cameraMoveSpeed * m_zoom;
	}

}
