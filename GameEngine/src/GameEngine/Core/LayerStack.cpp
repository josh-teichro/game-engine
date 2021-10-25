#include "gepch.h"

#include "GameEngine/Core/LayerStack.h"

namespace GameEngine {

	/**
	* Create a LayerStack.
	*/
	LayerStack::LayerStack()
	{
		m_layerInsertIndex = 0;
	}

	/**
	* Destroy this LayerStack.
	*
	* Deallocates all Layers.
	*/
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	/**
	* Push a Layer onto the top of the stack, but below overlays.
	*
	* Layer memory allocation will be handled by the LayerStack. Layers
	* will exist until the LayerStack has been destroyed.
	*/
	void LayerStack::PushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
		m_layerInsertIndex++;
		layer->OnAttach();
	}

	/**
	* Push an Overlay onto the top of the stack.
	*
	* Layer memory allocation will be handled by the LayerStack. Layers
	* will exist until the LayerStack has been destroyed.
	*/
	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	/**
	* Remove the given Layer from the LayerStack.
	*
	* Layer memory allocation is handled by the LayerStack. Layers will
	* exist until the LayerStack has been destroyed.
	*/
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
		if (it != m_layers.begin() + m_layerInsertIndex)
		{
			layer->OnDetach();
			m_layers.erase(it);
			m_layerInsertIndex--;
		}
	}

	/**
	* Remove the given overlay from the LayerStack.
	*
	* Layer memory allocation is handled by the LayerStack. Layers will
	* exist until the LayerStack has been destroyed.
	*/
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), overlay);
		if (it != m_layers.end()) {
			overlay->OnDetach();
			m_layers.erase(it);
		}
	}

}
