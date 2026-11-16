#include "pch.h"
#include "Renderer.h"

IP_NAMESPACE_START

Renderer::Renderer(GLFWwindow* pWindow, ImageDocument& document)
	: m_pWindow(pWindow), m_document(document)
{
	if (!m_pWindow) {
		throw std::invalid_argument(NULL_WINDOW_ERROR.data());
	}
}

void Renderer::StartFrame()
{
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Renderer::RenderEditor()
{
	CompletePendingFilter();
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::Begin(EDITOR_WINDOW_TITLE.data(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

	const float leftWidth = ImGui::GetContentRegionAvail().x * LEFT_PANEL_RATIO;
	ImGui::BeginChild(IMAGE_WORKSPACE_ID.data(), ImVec2(leftWidth, 0), true);
	RenderImageWorkspace();
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(FILTER_PANEL_ID.data(), ImVec2(0, 0), true);
	RenderFilterPanel();
	ImGui::EndChild();
	ImGui::End();
}

void Renderer::CompletePendingFilter()
{
	if (!m_pendingFilter.has_value() ||
		m_pendingFilter->wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
		return;
	}

	m_document.CommitImage(m_pendingFilter->get());
	m_pendingFilter.reset();
	m_status = FILTER_APPLIED_STATUS.data();
	m_uploadedImage = nullptr;
	m_imageSelected = false;
}

void Renderer::StartFilter(FilterType type, float amount)
{
	if (m_pendingFilter.has_value() || !m_document.HasImage()) {
		return;
	}

	PixelImage source = m_document.Current();
	m_status = FILTER_PROCESSING_STATUS.data();
	m_pendingFilter.emplace(std::async(std::launch::async, [source = std::move(source), type, amount]() mutable {
			ImageFilterFactory::Create(type, amount)->Apply(source);
			return source;
	}));
}

void Renderer::OpenImage()
{
	if (m_pendingFilter.has_value()) {
		return;
	}

	std::wstring path;
	if (!Helpers::OpenImageDialog(path)) {
		return;
	}

	std::wstring error;
	if (!m_document.Load(path, error)) {
		Helpers::ShowError(error);
		return;
	} 
	
	m_status = IMAGE_LOADED_STATUS.data();
	m_uploadedImage = nullptr;
	m_imageSelected = false;
	
}

void Renderer::RenderImageWorkspace()
{
	const bool isProcessing = m_pendingFilter.has_value();
	ImGui::BeginDisabled(isProcessing);

	if (ImGui::Button(OPEN_IMAGE_LABEL.data())) {
		OpenImage();
	}

	Helpers::SetHandCursorWhenHovered();

	ImGui::SameLine();

	ImGui::BeginDisabled(!m_document.HasImage());
	if (ImGui::Button(SAVE_IMAGE_LABEL.data())) {
		std::wstring path;
		if (Helpers::SaveImageDialog(path)) {
			std::wstring error;
			if (!m_document.Save(path, error)) {
				Helpers::ShowError(error);
			} else {
				m_status = IMAGE_SAVED_STATUS.data();
			}
		}
	}

	Helpers::SetHandCursorWhenHovered();

	ImGui::SameLine();

	ImGui::BeginDisabled(!m_document.CanUndo());
	if (ImGui::Button(UNDO_LABEL.data())) {
		m_document.Undo();
	}

	Helpers::SetHandCursorWhenHovered();
	ImGui::EndDisabled();

	ImGui::SameLine();
	ImGui::BeginDisabled(!m_document.CanRedo());
	if (ImGui::Button(REDO_LABEL.data())) {
		m_document.Redo();
	}
	Helpers::SetHandCursorWhenHovered();
	ImGui::EndDisabled();
	ImGui::EndDisabled();
	ImGui::EndDisabled();

	ImGui::SameLine();
	ImGui::TextUnformatted(m_status);
	ImGui::Separator();

	if (!m_document.HasImage()) {
		const ImVec2 available = ImGui::GetContentRegionAvail();
		ImGui::SetCursorPos(ImVec2(available.x * 0.5f - EMPTY_IMAGE_CURSOR_OFFSET_X, available.y * 0.5f - EMPTY_IMAGE_CURSOR_OFFSET_Y));
		if (ImGui::Selectable(EMPTY_IMAGE_LABEL.data(), false, ImGuiSelectableFlags_None, ImVec2(EMPTY_IMAGE_SELECTABLE_WIDTH, 0.0f))) {
			OpenImage();
		}
		Helpers::SetHandCursorWhenHovered();
		return;
	}

	if (m_imageSelected != 0 && !isProcessing && ImGui::IsKeyPressed(ImGuiKey_Delete)) {
		m_document.ClearImage();
		m_status = IMAGE_REMOVED_STATUS.data();
		m_uploadedImage = nullptr;
		m_imageSelected = false;
		return;
	}

	if (m_uploadedImage != &m_document.Current()) {
		m_texture.Upload(m_document.Current());
		m_uploadedImage = &m_document.Current();
	}

	const ImageProcessing::PixelImage& image = m_document.Current();
	const ImVec2 available = ImGui::GetContentRegionAvail();

	const float scale = std::min(available.x / static_cast<float>(image.width), available.y / static_cast<float>(image.height));
	const float safeScale = std::max(MIN_IMAGE_SCALE, std::min(scale, MAX_IMAGE_SCALE));
	const ImVec2 imageSize(static_cast<float>(image.width) * safeScale, static_cast<float>(image.height) * safeScale);

	if (ImGui::ImageButton("ImagePreview", ImTextureRef(static_cast<ImTextureID>(m_texture.Get())), imageSize)) {
		m_imageSelected = true;
	}

	Helpers::SetHandCursorWhenHovered();
}

void Renderer::RenderFilterPanel()
{
	const bool hasImage = m_document.HasImage();
	const bool isProcessing = m_pendingFilter.has_value();
	ImGui::TextUnformatted(FILTERS_LABEL.data());
	ImGui::Separator();
	ImGui::BeginDisabled(!hasImage || isProcessing);

	ImVec2 buttonSize(-1, 0);

	if (ImGui::Button(GRAYSCALE_LABEL.data(), buttonSize)) {
		StartFilter(FilterType::Grayscale);
	}
	Helpers::SetHandCursorWhenHovered();

	if (ImGui::Button(INVERT_LABEL.data(), buttonSize)) {
		StartFilter(FilterType::Invert);
	}
	Helpers::SetHandCursorWhenHovered();

	if (ImGui::Button(SEPIA_LABEL.data(), buttonSize)) {
		StartFilter(FilterType::Sepia);
	}
	Helpers::SetHandCursorWhenHovered();

	if (ImGui::Button(BRIGHTEN_LABEL.data(), buttonSize)) {
		StartFilter(FilterType::Brightness);
	}
	Helpers::SetHandCursorWhenHovered();

	ImGui::BeginDisabled(!m_document.CanUndo());
	if (ImGui::Button(RESET_LABEL.data(), buttonSize)) {
		m_document.ResetToOriginal();
	}
	Helpers::SetHandCursorWhenHovered();

	ImGui::EndDisabled();
	ImGui::EndDisabled();
	ImGui::Separator();
	ImGui::TextWrapped("%s", (hasImage ? FILTER_HISTORY_STATUS : NO_IMAGE_STATUS).data());
}

void Renderer::RenderFrame(const std::array<float, 3>& clearColor)
{
	ImGui::Render();
	int displayWidth{};
	int displayHeight{};
	glfwGetFramebufferSize(m_pWindow, &displayWidth, &displayHeight);
	glViewport(0, 0, displayWidth, displayHeight);
	glClearColor(clearColor[0], clearColor[1], clearColor[2], OPENGL_CLEAR_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(m_pWindow);
}

IP_NAMESPACE_END
