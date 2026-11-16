#include "pch.h"
#include "ImGuiContext.h"

IP_NAMESPACE_START

ImGuiContext::ImGuiContext(GLFWwindow* pWindow, std::string_view glslVersion, bool installCallbacks) : m_pWindow(pWindow)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    m_pIO = &ImGui::GetIO();
	if (!m_pIO) {
		throw std::runtime_error(IMGUI_IO_ERROR.data());
	}

    m_pIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_pWindow, installCallbacks);
    ImGui_ImplOpenGL3_Init(glslVersion.data());
}

ImGuiContext::~ImGuiContext()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

ImGuiIO& ImGuiContext::GetIO() const noexcept
{
	return *m_pIO; // Safe to dereference because m_pIO is initialized in the constructor and should always point to a valid ImGuiIO instance as long as the ImGui context is alive.
}

float ImGuiContext::GetFramerate() const noexcept
{
	return m_pIO->Framerate; // Can safely access Framerate because m_pIO is guaranteed to be valid as long as the ImGui context exists.
}

IP_NAMESPACE_END
