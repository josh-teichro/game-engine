#include "gepch.h"

#include "LayerStack.h"

namespace GameEngine {

	/**
	* Create a LayerStack.
	*/
	LayerStack::LayerStack()
	{
		m_layerInsert = m_layers.begin();
	}

	/**
	* Destroy this LayerStack.
	*
	* Deallocates all Layers.
	*/
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
			delete layer;
	}

	/**
	* Push a Layer onto the top of the stack, but below overlays.
	*
	* Layer memory allocation will be handled by the LayerStack. Layers
	* will exist until the LayerStack has been destroyed.
	*/
	void LayerStack::PushLayer(Layer* layer)
	{
		m_layers.insert(m_layerInsert, layer);
	}

	/**
	* Push an Overlay onto the top of the stack.
	*
	* Layer memory allocation will be handled by the LayerStack. Layers
	* will exist until the LayerStack has been destroyed.
	*/
	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_layers.push_back(overlay);
	}

	/**
	* Remove the given Layer from the LayerStack.
	*
	* Layer memory allocation is handled by the LayerStack. Layers will
	* exist until the LayerStack has been destroyed.
	*/
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			m_layerInsert--;
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
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end())
			m_layers.erase(it);
	}

}
