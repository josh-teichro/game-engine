#pragma once

#include "GameEngine/Renderer/RenderContext.h"

struct GLFWwindow;

namespace GameEngine {

	/**
	* OpenGL render context.
	*/
	class OpenGLContext : public RenderContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_windowHandle;

	};

}