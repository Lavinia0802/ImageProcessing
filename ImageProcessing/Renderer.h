#pragma once

#include "ImageDocument.h"

IP_NAMESPACE_START

class Renderer final
{
public:
	Renderer(GLFWwindow* pWindow, ImageDocument& document);
	~Renderer() = default;
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	void StartFrame();
	void RenderEditor();
	void RenderFrame(const std::array<float, 3>& clearColor);

private:
	void OpenImage();
	void CompletePendingFilter();
	void StartFilter(FilterType type, float amount = FILTER_BRIGHTNESS_AMOUNT);
	void RenderImageWorkspace();
	void RenderFilterPanel();

	GLFWwindow* m_pWindow{}; // borrowed pointer to the GLFW window, not owned by this class
	ImageDocument& m_document;
	OpenGlTexture m_texture;
	const PixelImage* m_uploadedImage{};
	const char* m_status{INITIAL_STATUS.data()};
	std::optional<std::future<PixelImage>> m_pendingFilter;
	std::uint64_t m_imageSelected{};
};

IP_NAMESPACE_END
