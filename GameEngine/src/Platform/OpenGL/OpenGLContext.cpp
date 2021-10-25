#include "gepch.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace GameEngine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :
		m_windowHandle(windowHandle)
	{
		GE_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);

		// init Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GE_CORE_ASSERT(status, "Failed to initialize Glad!");

		// log renderer info
		GE_CORE_INFO("OpenGL Info:");
		GE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		GE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		GE_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

#ifdef GE_ENABLE_ASSERTS
		// need to find out what version I support
		//int versionMajor;
		//int versionMinor;
		//glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		//glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		//GE_CORE_ASSERT(versionMajor > _ || (versionMajor == _ && versionMinor >= _), "Hazel requires at least OpenGL version ___!");
#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}
