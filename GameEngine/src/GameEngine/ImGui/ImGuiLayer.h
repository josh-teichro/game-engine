#pragma once

#include "GameEngine/Layer.h"
#include "GameEngine/Events/MouseButtonCodes.h"

namespace GameEngine {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;

	private:
		bool OnMouseMove(const MouseMoveEvent& e) override;
		bool OnMouseDown(const MouseDownEvent& e) override;
		bool OnKeyDown(const KeyDownEvent& e) override;
		bool OnKeyUp(const KeyUpEvent& e) override;
		bool OnMouseScroll(const MouseScrollEvent& e) override;
		bool OnCharTyped(const CharTypedEvent& e) override;

		void UpdateMouseButtons();
		void UpdateMouseCursor();

		inline MouseButton MapMouseButton(int button);

	private:
		float m_time;
		bool m_mouseJustPressed[(int)MouseButton::NumMouseButtons] = { false };
	};

}
