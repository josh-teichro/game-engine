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

		void BeginFrame();
		void EndFrame();
	};

}
