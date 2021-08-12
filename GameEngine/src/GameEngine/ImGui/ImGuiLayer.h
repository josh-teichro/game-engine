#pragma once

#include "GameEngine/Layer.h"

#include "imgui.h"

namespace GameEngine {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;

		bool OnMouseDown(const MouseDownEvent& e) override;

	private:
		float m_time;
		bool m_mouseJustPressed[ImGuiMouseButton_COUNT];
	};

}
