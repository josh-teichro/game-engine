#pragma once

#include "Core.h"

namespace GameEngine {

	/**
	* A Layer defines a set of objects, etc. that will be rendered and will
	* receive events. Layers are rendered in a specific order which determines
	* which Layer will be rendered or receive events first. 
	*/
	class GE_API Layer
	{
	public:
		Layer(const std::string& name);
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
		* Event callback. Handle events here.
		*/
		virtual void OnEvent() {};

		/**
		* Get the Layer name (for debugging).
		*/
		inline const std::string& GetName() const { return m_debugName; }

	private:
		std::string m_debugName;
	};

}
