#pragma once

#include "Core.h"
#include "Layer.h"

namespace GameEngine {

	/**
	* Stores Layers for rendering and receiving events. Layers are to be
	* rendered in the order that they are pushed onto the LayerStack. 
	* Events are to be pased down in the opposite order.
	*/
	class LayerStack
	{
	private:
		using _LayerStack = std::vector<Layer*>;

	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		_LayerStack::iterator begin() { return m_layers.begin(); }
		_LayerStack::iterator end() { return m_layers.end(); }

	private:
		_LayerStack m_layers;
		unsigned int m_layerInsertIndex;
	};

}
