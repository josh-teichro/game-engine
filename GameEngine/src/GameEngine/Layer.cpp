#include "gepch.h"

#include "Layer.h"

namespace GameEngine {

	/**
	* Create a new Layer.
	*
	* name: name used for debugging
	*/
	GameEngine::Layer::Layer(const std::string& name) :
		m_debugName(name)
	{
	}

	/**
	* Destroy this Layer.
	*/
	GameEngine::Layer::~Layer()
	{
	}

}