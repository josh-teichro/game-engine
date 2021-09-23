#pragma once

namespace GameEngine {

	class Time
	{
	public:
		Time();

		static float GetDeltaTime() { return s_instance->m_deltaTime; }
		static float GetFixedTime() { return s_instance->m_fixedTime; }

		void OnUpdate();

	private:
		static Time* s_instance;

		float m_deltaTime;
		float m_fixedTime;
	};

}
