#include "pch.h"
#include "MainWindow.h"

IP_NAMESPACE_START

MainWindow::MainWindow(int width, int height, std::string_view windowTitle)
{
	m_pWindow = glfwCreateWindow(width, height, windowTitle.data(), nullptr, nullptr);

	if (!m_pWindow) {
		std::printf("%s", GLFW_WINDOW_ERROR.data());
		throw std::bad_alloc();
	}

	glfwMakeContextCurrent(m_pWindow);
	glfwSwapInterval(1); // Enable VSync
}

MainWindow::~MainWindow()
{
	if (m_pWindow) {
		glfwDestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}
}

GLFWwindow* MainWindow::GetWindow() const noexcept
{
	return m_pWindow;
}

bool MainWindow::IsWindowValid() const noexcept
{
	return m_pWindow != nullptr;
}

bool MainWindow::ShouldClose() const noexcept
{
	return glfwWindowShouldClose(m_pWindow) == GLFW_TRUE;
}

IP_NAMESPACE_END
