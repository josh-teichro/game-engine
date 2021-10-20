#pragma once

#include "GameEngine/Core/Layer.h"
#include "GameEngine/Events/MouseCodes.h"

namespace GameEngine {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void BeginFrame();
		void EndFrame();
	};

}
