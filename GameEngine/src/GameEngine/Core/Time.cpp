#include "gepch.h"
#include "Time.h"
#include "Platform.h"

namespace GameEngine {

	Time* Time::s_instance = nullptr;

	Time::Time() :
		m_deltaTime(0.0f),
		m_fixedTime(0.0f)
	{
		GE_CORE_ASSERT(!s_instance, "Time object already exists!");
		s_instance = this;
	}

	void Time::OnUpdate() {
		float newTime = Platform::GetTime();
		m_deltaTime = newTime - m_fixedTime;
		m_fixedTime = newTime;
	}

}