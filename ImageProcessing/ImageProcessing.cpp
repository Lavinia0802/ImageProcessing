#include "pch.h"
#include "GlfwInitializer.h"
#include "Helpers.h"
#include "ImGuiContext.h"
#include "MainWindow.h"
#include "Renderer.h"

int main()
{
	ImageProcessing::GdiplusSession gdiplusSession;
	if (!gdiplusSession.IsInitialized()) {
		return -1;
	}

	ImageProcessing::GlfwInitializer initializer;
	if (!initializer.IsInitialized()) {
		return -1;
	}

	ImageProcessing::MainWindow mainWindow(ImageProcessing::WINDOW_WIDTH, ImageProcessing::WINDOW_HEIGHT, ImageProcessing::WINDOW_TITLE);
	if (!mainWindow.IsWindowValid()) {
		return -1;
	}

	ImageProcessing::ImGuiContext imguiContext(mainWindow.GetWindow(), ImageProcessing::GLSL_VERSION);
	ImageProcessing::ImageDocument document;
	ImageProcessing::Renderer renderer(mainWindow.GetWindow(), document);

	while (!mainWindow.ShouldClose()) {
		renderer.StartFrame();
		renderer.RenderEditor();
		renderer.RenderFrame(ImageProcessing::APP_BACKGROUND_COLOR);
	}

	return 0;
}
