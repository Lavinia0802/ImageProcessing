#include "pch.h"
#include "GlfwInitializer.h"

IP_NAMESPACE_START

GlfwInitializer::GlfwInitializer() : m_initResult(glfwInit())
{
	if (m_initResult != GLFW_TRUE) {
		std::printf("%s", GLFW_INIT_ERROR.data());
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}

GlfwInitializer::~GlfwInitializer()
{
	if (m_initResult == GLFW_TRUE) { // no need to call it if initialization failed; it was already called in glfwInit()
		glfwTerminate();
	}
}

bool GlfwInitializer::IsInitialized() const noexcept
{
	return m_initResult == GLFW_TRUE;
}

IP_NAMESPACE_END