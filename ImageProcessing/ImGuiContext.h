#pragma once

IP_NAMESPACE_START

class ImGuiContext final
{
public:
	ImGuiContext(GLFWwindow* pWindow, std::string_view glslVersion, bool installCallbacks = true);
	~ImGuiContext();

public:
	[[nodiscard]] ImGuiIO& GetIO() const noexcept;
	[[nodiscard]] float GetFramerate() const noexcept;

private:
	// Can't copy or move this class
	ImGuiContext(const ImGuiContext&) = delete;
	ImGuiContext& operator=(const ImGuiContext&) = delete;

	ImGuiContext(ImGuiContext&&) = delete;
	ImGuiContext& operator=(ImGuiContext&&) = delete;

private:
	GLFWwindow* m_pWindow{}; // borrowed pointer to the GLFW window, not owned by this class
	ImGuiIO* m_pIO{}; // borrowed pointer to the ImGuiIO structure, not owned by this class
};

IP_NAMESPACE_END
