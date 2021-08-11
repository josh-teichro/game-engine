#pragma once

#include "GameEngine/Layer.h"

namespace GameEngine {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		bool OnEvent(const Event& e) override;

	private:
		float m_time;
	};

}
