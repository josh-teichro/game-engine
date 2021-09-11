#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/HandlesEvents.h"

namespace GameEngine {

	/**
	* A Layer defines a set of objects, etc. that will be rendered and will
	* receive events. Layers are rendered in a specific order which determines
	* which Layer will be rendered or receive events first. 
	*/
	class GE_API Layer : public HandlesEvents
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		/**
		* Called when the Layer is added to the Application.
		*/
		virtual void OnAttach() {};

		/**
		* Called when the Layer is removed from the Application.
		*/
		virtual void OnDetach() {};

		/**
		* Called every frame. Draw any objects, etc. here.
		*/
		virtual void OnUpdate() {};

		/**
		* Called every ImGui frame. Render any ImGui components here.
		*/
		virtual void OnImGuiUpdate() {};

		/**
		* Get the Layer name (for debugging).
		*/
		inline const std::string& GetName() const { return m_debugName; }

	private:
		std::string m_debugName;
	};

}
