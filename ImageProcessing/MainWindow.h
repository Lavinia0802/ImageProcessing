#pragma once

IP_NAMESPACE_START

class MainWindow final // final class to ensure that it cannot be inherited
{
public:
	MainWindow(int width, int height, std::string_view windowTitle);
	~MainWindow();

public:
	[[nodiscard]] GLFWwindow* GetWindow() const noexcept;
	[[nodiscard]] bool IsWindowValid() const noexcept;
	[[nodiscard]] bool ShouldClose() const noexcept;

private:
	// Can't copy or move this class
	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;

	MainWindow(MainWindow&&) = delete;
	MainWindow& operator=(MainWindow&&) = delete;
private:
	GLFWwindow* m_pWindow{};
};

IP_NAMESPACE_END
