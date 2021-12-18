#pragma once

#include <GameEngine.h>

namespace GameEngine
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate() override;
		virtual void OnImGuiUpdate() override;

		virtual bool OnEvent(const Event& e) override;
		virtual bool OnKeyUp(const KeyUpEvent& e) override;
		virtual bool OnWindowResize(const WindowResizeEvent& e) override;

	private:
		CameraController2D m_cameraController;

		Ref<FrameBuffer> m_frameBuffer;
		glm::vec2 m_viewportPanelSize;
		bool m_isViewportFocused;
		bool m_isViewportHovered;

	private:
		void RenderEditorWindows();

	};

}
