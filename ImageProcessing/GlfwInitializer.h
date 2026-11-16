#pragma once

IP_NAMESPACE_START

class GlfwInitializer final // final class to ensure that it cannot be inherited
{
public:
	GlfwInitializer();
	~GlfwInitializer();

	[[nodiscard]] bool IsInitialized() const noexcept;

private:
	// Can't copy or move this class
	GlfwInitializer(const GlfwInitializer&) = delete;
	GlfwInitializer& operator=(const GlfwInitializer&) = delete;

	GlfwInitializer(GlfwInitializer&&) = delete;
	GlfwInitializer& operator=(GlfwInitializer&&) = delete;

private:
	int m_initResult{};
};

IP_NAMESPACE_END