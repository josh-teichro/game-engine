#pragma once

#include "GameEngine/Core/Layer.h"
#include "GameEngine/Core/Core.h"

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
		std::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_layers.rend(); }

	private:
		_LayerStack m_layers;
		unsigned int m_layerInsertIndex;
	};

}
